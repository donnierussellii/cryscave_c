// Crystal Caverns
// (c) 1982 by Daniel Kitchen
// C port and parser (c) 2021 by Donnie Russell II

// This source code is provided for personal, educational use only.
// You are welcome to use this source code to develop your own works,
// but the story-related content belongs to the original author(s).



#ifdef __DJGPP__
  #include <unistd.h>
  #include <conio.h> // getch
#elif defined _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <conio.h> // getch
#elif defined __unix__
  #include <unistd.h>
#elif defined __APPLE__
  #include <unistd.h>
#endif



#include "_def.h"
#include "_tables.h"



#define MAX_INVENTORY  7
#define MAX_CAPACITY   1  //of each container



FILE *InputStream;

#ifndef NO_STATUS_LINE
int VTMode;
char StatusLineText[80];
#endif

int PrintDelayFlag;

#define MAX_PRINTED_LINES  23

int NumPrintedLines;

int CursorColumn;

int NumStrWords;
int CurWord;

char *StrWord[80]; //pointers to words in str; writing to data pointed-to here is prohibited

enum
{
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

int Verbosity = V_BRIEF;

int ItObj, PrevItObj; //the obj "it" refers to

char *StrAnd = "and";   //point to this string when comma is encountered in input
char *StrThen = "then"; //point to this string when period is encountered in input

unsigned char TimePassed; //flag: time passed during action
unsigned char GameOver; //flag, but with special value 2: restart was requested



//from game.c
extern int NumMoves;
extern unsigned char StateVar[NUM_STATE_VARS];
extern struct DOMISCWITH_STRUCT DoMiscWith[];
extern struct DOMISC_STRUCT DoMisc[];
extern struct DOMISCTO_STRUCT DoMiscTo[];
extern struct GOFROM_STRUCT GoFrom[];

//from compdata.c
extern unsigned char *RoomDesc[];
extern unsigned char *BlockMsg[];
extern unsigned char *ObjectDesc[];



//*****************************************************************************
int LineWidth = 79;



#ifndef NO_STATUS_LINE

int EnableVTMode(void)
{

#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;

  if (hOut == INVALID_HANDLE_VALUE) return 0;
  if (!GetConsoleMode(hOut, &dwMode)) return 0;
  dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
  if (!SetConsoleMode(hOut, dwMode)) return 0;

  fputs("\x1b" "[2r", stdout); // set scrolling region to line 2; limits scrollback buffer

  return 1;
#elif __DJGPP__
  return 1;
#else
  return 0;
#endif  

}



void PrintStatusLine(void)
{
  char buf[128];
  int len, i;

  if (VTMode == 0) return;

  fputs("\x1b" "[s" "\x1b" "[1;1H", stdout); // save yx; set yx to (1,1)
  fputs("\x1b" "[7m", stdout); // swap foreground/background colors

  sprintf(buf, "   Score: %i   Moves: %i", GetScore(), NumMoves); // score can go down: add extra leading spaces
  len = strlen(buf);
  if (len > 39) len = 39;
  memcpy(StatusLineText + (79 - len), buf, len);

  for (i=0; i<80; i++)
    fputc(StatusLineText[i], stdout);

  fputs("\x1b" "[0m", stdout); // restore default colors
  fputs("\x1b" "[u", stdout); // restore xy
}

#endif



void PrintMore(int erase)
{
  int i;

  if (erase)
  {
    i = 4; while (i-- > 0) putchar('\b');
    i = 4; while (i-- > 0) putchar(' ');
    i = LineWidth; while (i-- > 0) putchar('\b');
  }
  else
  {
    i = LineWidth - 4; while (i-- > 0) putchar(' ');
    putchar('M'); putchar('O'); putchar('R'); putchar('E');
  }
}



void PrintNewLine(void)
{
  CursorColumn = 0;
  NumPrintedLines++;

  if (NumPrintedLines == MAX_PRINTED_LINES)
  {
    NumPrintedLines = 0;

    if (InputStream == stdin)
    {
#ifdef _WIN32
      putchar('\n');
      PrintMore(0);
      while (getch() == 0) {} // conio.h
      PrintMore(1);
#elif __DJGPP__
      putchar('\n');
      PrintMore(0);
      while (getch() == 0) {} // conio.h
      PrintMore(1);
#else
      getchar();
#endif  
    }
    else putchar('\n');
  }
  else putchar('\n');
}



void PrintDelay(void)
{
#ifndef NO_SLOW_PRINT
  if (InputStream == stdin)
  {
    fflush(stdout);
#ifdef __DJGPP__
    usleep(3*20 * 1000);
#elif defined _WIN32
    Sleep(3*20);
#elif defined __unix__
    usleep(3*20 * 1000);
#elif defined __APPLE__
    usleep(3*20 * 1000);
#endif
  }
#endif
}



//does word wrapping; recognizes newline char
//print terminated by '^' or nullchar
void PrintText(char *p)
{
  int width;
  char *q;

  for (;;) //outer loop
  {
    for (;;) //inner loop
    {
      q = p;
      width = LineWidth - CursorColumn; //try to print rest of current line
      while (width > 0 && *q != 0 && *q != '^' && *q != '\n') //find actual width we can print
      {
        width--;
        q++;
      }
      if (width == 0) break; //we can print all so break into outer loop

      while (*p != 0 && *p != '^' && *p != '\n') //print up until end of string or newline
      {
        putchar(*p++); if (PrintDelayFlag) PrintDelay();
        CursorColumn++;
      }
      if (*p == 0 || *p == '^') return; //end of string; done
      PrintNewLine(); //it was a newline
      p++; //skip newline char and repeat inner loop
    }

    while (q > p && *q != ' ') q--; //try to find a space char to break line
    width = q - p;
    if (width < 1) width = LineWidth - CursorColumn; //didn't find a space so break it ourselves
    while (width > 0) //print number of characters we calculated
    {
      width--;
      putchar(*p++); if (PrintDelayFlag) PrintDelay();
    }
    PrintNewLine(); //go to next line
    while (*p == ' ') p++; //skip any spaces and repeat outer loop
  }
}



//prints a newline automatically after text
void PrintLine(char *p)
{
  PrintText(p);
  PrintNewLine();
}



void PrintCompText(char *comp_text)
{
  char *uncomp_text;
  int comp_size;

  comp_size = strlen(comp_text) + 1;
  uncomp_text = malloc(GetDecompressTextSize(comp_text, comp_size));
  DecompressText(comp_text, comp_size, uncomp_text);

  if (*uncomp_text != 0)
    PrintText(uncomp_text);

  free(uncomp_text);
}



//prints a newline automatically after text
void PrintCompLine(char *p)
{
  PrintCompText(p);
  PrintNewLine();
}



void PrintBlockMsg(int i)
{
  char *compressed_text, *p;
  int compressed_size;

  compressed_text = BlockMsg[i];
  compressed_size = strlen(compressed_text)+1;
  p = malloc(GetDecompressTextSize(compressed_text, compressed_size));
  DecompressText(compressed_text, compressed_size, p);

  if (*p != 0) PrintLine(p);

  free(p);
}



void PrintObjectDesc(int obj, int desc_flag)
{
  char *compressed_text, *decompressed_text, *p;
  int compressed_size;

  compressed_text = ObjectDesc[obj];
  compressed_size = strlen(compressed_text)+1;
  decompressed_text = p = malloc(GetDecompressTextSize(compressed_text, compressed_size));
  DecompressText(compressed_text, compressed_size, decompressed_text);

  if (desc_flag == 0)
  {
    if (*p != '^' && *p != 0) PrintText(p);
  }
  else
  {
    p = strchr(p, '^');
    if (p != NULL)
    {
      p++;
      if (*p != '^' && *p != 0) PrintText(p);
    }
  }

  free(decompressed_text);
}



void PrintInteger(int num)
{
  int neg = 0, size = 0, i;
  char buf[80];

  if (num == 0)
  {
    putchar('0');
    CursorColumn++;
    return;
  }

  if (num < 0)
  {
    num = -num;
    neg = 1;
  }

  while (num)
  {
    buf[size++] = '0' + (num % 10);
    num /= 10;
  }

  if (neg)
  {
    putchar('-');
    CursorColumn++;
  }

  for (i=size-1; i>=0; i--)
  {
    putchar(buf[i]);
    CursorColumn++;
  }
}



void PrintScore(void)
{
  PrintText(GameOver ? "\nYou scored " : "Your score is ");
  PrintInteger(GetScore());
  PrintText(" points out of a maximum of ");
  PrintInteger(GetMaxScore());
  PrintText(", using ");
  PrintInteger(NumMoves);
  PrintText((NumMoves == 1) ? " move.\n" : " moves.\n");

  if (GameOver)
  {
    PrintText("\nThat gives you a rank of ");
    PrintText(GetRankName());
    PrintText(".\n");
  }
}



//get input from player

void GetWords(char *prompt)
{
  char *p;

  static char str[80];

  NumStrWords = 0;
  CurWord = 0;

  memset(str, 0, 80);

  PrintText(prompt);

  CursorColumn = 0; // this is due to fgets below
  NumPrintedLines = 0;

#ifndef NO_STATUS_LINE
  PrintStatusLine();
#endif

  //get input line from stdin or file
  fgets(str, 80, InputStream);
  if (InputStream != stdin && (feof(InputStream) || ferror(InputStream)))
  {
    fclose(InputStream);
    InputStream = stdin;
    fgets(str, 80, InputStream);
  }
  if (InputStream != stdin) PrintText(str);


  //convert upper case chars to lower case, replace whitespace chars with null char, replace ! and ? with .
  p = str;
  while (p < str+80)
  {
    if (isupper(*p)) *p = tolower(*p);
    else if (isspace(*p)) *p = 0;
    else if (*p == '!' || *p == '?') *p = '.';
    p++;
  }

  //fill array of pointers to words in str, or to string literals if replacing punctuation
  p = str;
  for (;;)
  {
    while (*p == 0 && p < str+80) p++;
    if (p == str+80) break;

    if (*p != ',' && *p != '.')
      StrWord[NumStrWords++] = p;

    while (*p != 0 && p < str+80)
    {
      if (*p == ',' || *p == '.')
      {
        StrWord[NumStrWords++] = (*p == ',') ? StrAnd : StrThen;
        *p++ = 0;
        break;
      }
      p++;
    }
    if (p == str+80) break;
  }
}
//*****************************************************************************



//*****************************************************************************

//call only for:  OBJ_YOU < obj < NUM_OBJECTS

//objects inside containers are only visible one level deep

int IsObjVisible(int obj)
{
  if (Obj[obj].loc == 2048 + OBJ_YOU) return 1;   //obj is in your inventory
  if (Obj[obj].loc == Obj[OBJ_YOU].loc) return 1; //obj is in same room as you
  if (Obj[obj].loc >= 2048) //obj is inside something
  {
    int container;

    container = Obj[obj].loc - 2048; //obj is inside container
    if (Obj[container].prop & PROP_OPEN) //container is open
    {
      if (Obj[container].loc == 2048 + OBJ_YOU) return 1;   //container is in your inventory
      if (Obj[container].loc == Obj[OBJ_YOU].loc) return 1; //container is in same room as you
    }
  }
  return 0;
}



int IsPlayerInDarkness(void)
{
  int i, obj;

  if ((Room[Obj[OBJ_YOU].loc].prop & R_DARK) == 0) return 0; //room not naturally dark

  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;
    if (IsObjVisible(obj) && (Obj[obj].prop & PROP_LIT)) return 0; //a lit obj is visible
  }

  return 1;
}



//move order of "obj" to last in printing order
void MoveObjOrderToLast(int obj)
{
  int i, j;

  for (i=2; i<NUM_OBJECTS; i++)
    if (obj == Obj[i].order)
  {
    for (j=i; j<NUM_OBJECTS-1; j++)
      Obj[j].order = Obj[j+1].order;
    Obj[j].order = obj;
    break;
  }
}



//returns number of objects in location
//location can be a room, player's inventory, or inside object
int GetNumObjects(int loc)
{
  int count, i, obj;

  count = 0;
  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;
    if (Obj[obj].loc == loc) count++;
  }
  return count;
}



//print objects at "location"; location can be player's inventory
void PrintPresentObjects(int location)
{
  int i, j, obj, obj_inside, flag1, flag2;

  flag1 = 0;
  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;
    if (Obj[obj].loc == location && (Obj[obj].prop & PROP_NODESC) == 0)
    {
      if (flag1 == 0)
      {
        if (location == 2048 + OBJ_YOU) PrintLine("You're carrying:");
        flag1 = 1;
      }

      if (location == 2048 + OBJ_YOU) PrintObjectDesc(obj, 0);
      else PrintObjectDesc(obj, 1);
      PrintText("\n");

      if (Obj[obj].prop & PROP_OPEN) //only display objects inside something else one level deep
      {
        flag2 = 0;
        for (j=2; j<NUM_OBJECTS; j++)
        {
          obj_inside = Obj[j].order;
          if (Obj[obj_inside].loc == 2048 + obj)
          {
            if (flag2 == 0) {PrintLine("  (which contains)"); flag2 = 1;}

            PrintText("  ");
            PrintObjectDesc(obj_inside, 0);
            PrintText("\n");
          }
        }
      }
    }
  }

  if (location == 2048 + OBJ_YOU && flag1 == 0) PrintLine("You're not carrying anything.");
}
//*****************************************************************************



//*****************************************************************************
void PrintRoomDesc(int room, int force_description)
{
  char *compressed_text, *decompressed_text, *p;
  int compressed_size, state, i, ch;

  state = RoomPassages[15*room + 10];

  compressed_text = RoomDesc[room];
  compressed_size = strlen(compressed_text)+1;
  decompressed_text = p = malloc(GetDecompressTextSize(compressed_text, compressed_size));
  DecompressText(compressed_text, compressed_size, decompressed_text);


  if (*p != '^' && *p != 0)
  {

#ifndef NO_STATUS_LINE
    memset(StatusLineText, ' ', 80);
    for (i=0; i<39; i++)
    {
      ch = p[i];
      if (ch == 0 || ch == '^') break;
      StatusLineText[1+i] = ch;
    }
#endif

    PrintLine(p); // print room name
  }


  if (force_description ||
      Verbosity != V_SUPERBRIEF)
  {
    p = strchr(p, '^');
    if (p != NULL)
    {
      p++;
  
      if (force_description ||
          Verbosity == V_VERBOSE ||
          (Room[room].prop & R_DESCRIBED) == 0)
        if (*p != '^' && *p != 0) PrintLine(p);

      p = strchr(p, '^');
      if (p != NULL)
      {
        p++;
        if (state != 0)
        {
          if (StateVar[state] == 0)
            {if (*p != '^' && *p != 0) PrintLine(p);}
          else
          {
            p = strchr(p, '^');
            if (p != NULL)
              {p++; if (*p != '^' && *p != 0) PrintLine(p);}
          }
        }
      }
    }
  }

  Room[room].prop |= R_DESCRIBED;

  free(decompressed_text);
}



void PrintPlayerRoomDesc(int force_description)
{
  if (IsPlayerInDarkness())
    PrintLine("It is now completely dark. Continuing without a light could be hazardous to your health.");
  else
  {
    PrintRoomDesc(Obj[OBJ_YOU].loc, force_description);
    if (force_description || Verbosity != V_SUPERBRIEF) PrintPresentObjects(Obj[OBJ_YOU].loc);
  }
}
//*****************************************************************************



//*****************************************************************************
//returns 1 if successful
int DoSave(void)
{
  FILE *f;
  int error = 1, slot, size;
  unsigned char *p, string[80];

  GetWords("Save to which slot (0-9; default=0; q to cancel)>");
  if (NumStrWords == 0) slot = 0;
  else
  {
    for (slot=0; slot<10; slot++)
    {
      string[0] = '0' + slot;
      string[1] = 0;
      if (MatchCurWord(string)) break;
    }
    if (slot == 10) {PrintLine("*** Save cancelled. ***"); return 0;}
  }

  strcpy(string, "cryscav#.sav");
  string[7] = '0' + slot;

  f = fopen(string, "rb");
  if (f != NULL)
  {
    fclose(f);
    for (;;)
    {
      GetWords("Saved game exists. Overwrite? ");
      if (MatchCurWord("y") || MatchCurWord("yes")) break;
      if (MatchCurWord("n") || MatchCurWord("no")) {PrintLine("*** Save cancelled. ***"); return 0;}
      PrintLine("Please answer yes or no.");
    }
  }

  size = GetSaveStateSize();
  p = malloc(size);
  if (p == NULL) goto done;
  ReadSaveState(p);
  f = fopen(string, "wb");
  if (f == NULL) {free(p); goto done;}
  if (fwrite(p, size, 1, f) != 1) {fclose(f); free(p); goto done;}
  fclose(f);
  free(p);

  error = 0;
done:
  if (error)
  {
    PrintLine("*** Save failed. ***");
    return 0;
  }
  else
  {
    PrintLine("*** Save successful. ***");
    return 1;
  }
}



//returns 1 if successful
int DoRestore(void)
{
  FILE *f;
  int error = 1, slot, size;
  unsigned char *p, string[80];

  GetWords("Restore from which slot (0-9; default=0; q to cancel)>");
  if (NumStrWords == 0) slot = 0;
  else
  {
    for (slot=0; slot<10; slot++)
    {
      string[0] = '0' + slot;
      string[1] = 0;
      if (MatchCurWord(string)) break;
    }
    if (slot == 10) {PrintLine("*** Restore cancelled. ***"); return 0;}
  }

  strcpy(string, "cryscav#.sav");
  string[7] = '0' + slot;

  size = GetSaveStateSize();
  p = malloc(size);
  if (p == NULL) goto done;
  f = fopen(string, "rb");
  if (f == NULL) {free(p); PrintLine("File not found."); goto done;}
  if (fread(p, size, 1, f) != 1) {fclose(f); free(p); goto done;}
  fclose(f);
  WriteSaveState(p);
  free(p);

  error = 0;
done:
  if (error)
  {
    PrintLine("*** Restore failed. ***");
    return 0;
  }
  else
  {
    NumStrWords = 0;
    ItObj = 0;
    GameOver = 0;

    PrintLine("*** Restore successful. ***\n");
    PrintPlayerRoomDesc(1); //force description

    return 1;
  }
}
//*****************************************************************************



//*****************************************************************************
//Parse Routines



//if "match" matches current word in input, move to next word and return 1
int MatchCurWord(char *match)
{
  if (NumStrWords >= CurWord+1 && strcmp(StrWord[CurWord], match) == 0)
  {
    CurWord++;
    return 1;
  }
  return 0;
}



//string contains words separated by commas with no whitespace
int IsWordInCommaString(char *p, char *word)
{
  char *q, buffer[80];
  int len;

  if (p != 0)
    for (;;)
  {
    q = strchr(p, ',');
    if (q == 0) q = strchr(p, 0);
    len = q-p;
    if (len > 0 && len < 80)
    {
      memcpy(buffer, p, len);
      buffer[len] = 0;
      if (strcmp(word, buffer) == 0) return 1;
    }

    if (*q == 0) break;
    else p = q+1; //skip past comma
  }

  return 0;
}



int GetActionFromInput(void)
{
  char buffer[80*2], *p, *q;
  int i, j, len, action, temp;

  i = 0;
  for (;;)
  {
    action = VerbToAction[i].action;
    if (action == 0) break; //reached end of list without finding anything


    //fill buffer with up to two words of verb phrase
    memset(buffer, 0, 80*2);
    p = VerbToAction[i].phrase;
    for (j=0; j<2; j++)
    {
      q = strchr(p, ' ');
      if (q == 0) q = strchr(p, 0);
      len = q-p; if (len > 0 && len < 80) memcpy(buffer+80*j, p, len);
      if (*q == 0) break;
      else p = q+1; //skip past space
    }


    temp = CurWord;
    if (NumStrWords >= CurWord+1 && strcmp(StrWord[CurWord], buffer+80*0) == 0)
    {
      CurWord++;
      if (buffer[80*1] != 0)
      {
        if (NumStrWords >= CurWord+1 && strcmp(StrWord[CurWord], buffer+80*1) == 0)
          CurWord++;
        else CurWord = temp;
      }
    }
    if (CurWord > temp) break; //found action

    i++;
  }

  return action;
}



//matches a maximum of three words

//if there are multiple match objects that are visible,
//this function will ask player to be more specific by returning -1

int GetObjFromInput(void)
{
  char buffer[80*3];
  int num_matches, i, j;
  unsigned short match_obj[80];
  unsigned char match_size[80];

  num_matches = 0;

  i = 0;
  for (;;) //look through noun phrase table
  {
    int obj, size;
    char *p;

    obj = NounPhraseToObj[i].obj;
    if (obj == 0) break;

    size = 0;
    p = NounPhraseToObj[i].phrase;
    for (j=0; j<3; j++)
    {
      int len;
      char *q;

      q = strchr(p, ' ');
      if (q == 0) q = strchr(p, 0);
      len = q-p;
      if (len > 0 && len < 80)
      {
        memcpy(buffer+80*j, p, len);
        buffer[80*j+len] = 0;
        size++;
      }
      if (*q == 0) break;
      else p = q+1; //skip past space
    }

    if (size) //size of noun phrase to match with input
    {
      j = 0;
      if (NumStrWords >= CurWord + size) //is input long enough to match
        for (; j<size; j++)
          if (strcmp(StrWord[CurWord+j], buffer+80*j)) break; //break early if words are different

      if (j == size) //all words of phrase matched
      {
        match_obj[num_matches] = obj;
        match_size[num_matches] = size;
        num_matches++;
        if (num_matches == 80) break;
      }
    }

    i++; //try another noun phrase from table
  }

  if (num_matches == 0) return 0;

  //if more than one match, check if objects are both visible at same time
  //if so, return -1 so that player can know to be more specific
  for (i=0; i<num_matches; i++)
    if (IsObjVisible(match_obj[i]))
      for (j=i+1; j<num_matches; j++)
        if (match_obj[j] != match_obj[i] && IsObjVisible(match_obj[j]))
          return -1;

  //watch out: need the longest matched size here
  //first match should always have longest size because
  //phrases must be listed in order of decreasing size in phrase table
  for (i=0; i<num_matches; i++)
    if (IsObjVisible(match_obj[i])) break; //use first visible match, if possible
  if (i == num_matches) i = 0; //if no visible matches, use first one

  CurWord += match_size[i];
  return match_obj[i];
}



int GetFixedObjFromInput(int room)
{
  char buffer[80*2], *p, *q;
  int i, j, len, fobj, fobj_inroom, cw_inroom, fobj_notinroom, cw_notinroom, temp;

  fobj_inroom = 0;    //matched fixed obj that was found in room
  fobj_notinroom = 0; //                           not found in room

  temp = CurWord; //keep track of word position because we will be matching multiple noun phrases

  i = 0;
  for (;;)
  {
    fobj = NounPhraseToFixedObj[i].fobj;
    if (fobj == 0) break; //reached end of list without finding anything


    //fill buffer with up to two words of noun phrase
    memset(buffer, 0, 80*2);
    p = NounPhraseToFixedObj[i].phrase;
    for (j=0; j<2; j++)
    {
      q = strchr(p, ' ');
      if (q == 0) q = strchr(p, 0);
      len = q-p; if (len > 0 && len < 80) memcpy(buffer+80*j, p, len);
      if (*q == 0) break;
      else p = q+1; //skip past space
    }


    CurWord = temp;
    if (NumStrWords >= CurWord+1 && strcmp(StrWord[CurWord], buffer+80*0) == 0)
    {
      CurWord++;
      if (buffer[80*1] != 0)
      {
        if (NumStrWords >= CurWord+1 && strcmp(StrWord[CurWord], buffer+80*1) == 0)
          CurWord++;
        else CurWord = temp;
      }
    }

    if (CurWord > temp) //found noun phrase
    {
      if (NounPhraseToFixedObj[i].room == room) //found a fixed obj in room
      {
        if (fobj_inroom == 0) //only use first one found, though there should be only one anyway
        {
          fobj_inroom = fobj;
          cw_inroom = CurWord; //keep track of word position past this match
        }
      }
      else //found a fixed obj not in room
      {
        if (fobj_notinroom == 0) //only use first one found, though there should be only one anyway
        {
          fobj_notinroom = fobj;
          cw_notinroom = CurWord; //keep track of word position past this match
        }
      }
    }

    i++;
  }

  //return fixed or scenery objects in room first

  //fixed object in room
  if (fobj_inroom)
  {
    CurWord = cw_inroom;
    return fobj_inroom;
  }

  //scenery object in room
  if (NumStrWords >= CurWord+1 && room < NUM_ROOMS && IsWordInCommaString(Room[room].scenery, StrWord[CurWord]))
  {
    CurWord++;
    return FOBJ_SCENERY_VIS;
  }

  //fixed object not in room
  if (fobj_notinroom)
  {
    CurWord = cw_notinroom;
    return FOBJ_NOTVIS;
  }

  //scenery object not in room
  if (NumStrWords >= CurWord+1 && IsWordInCommaString(SceneryNouns, StrWord[CurWord]))
  {
    CurWord++;
    return FOBJ_SCENERY_NOTVIS;
  }

  return 0;
}



//gets obj, fixed obj, or scenery obj, whichever comes first

//returns -1 if player needs to be more specific
//        -2 if player used "it" but it wasn't clear what itobj is

//itobj starts at 0
//if itobj already refers to an object, any additional object invalidates itobj (sets to -1)

int GetAllObjFromInput(int room)
{
  int obj;

  if (MatchCurWord("it") || MatchCurWord("them"))
  {
    //itobj from previous sentence
    if (PrevItObj <= 0)
    {
      PrintLine("I'm not sure what you're referring to with one or more of those nouns.");
      return -2;
    }
    ItObj = PrevItObj;
    return ItObj;
  }

  //skip article (if any) immediately before object
  if (MatchCurWord("the") || MatchCurWord("a") || MatchCurWord("an")) {}

  //convert noun phrase to obj
  obj = GetObjFromInput(); //can return -1 if player needs to be more specific
  if (obj == 0) obj = GetFixedObjFromInput(room);

  if (obj == 0)
  {
    ItObj = -1;
    PrintLine("I didn't recognize one or more of those nouns, or you didn't specify one.");
  }
  else if (obj == -1)
  {
    ItObj = -1;
    PrintLine("You need to be more specific with one or more of those nouns.");
  }
  else
  {
    if (ItObj == 0) ItObj = obj; //first object encountered this sentence; set itobj
    else ItObj = -1; //another obj encountered; invalidate itobj
  }

  return obj;
}



//same as GetAllObjFromInput() above but just skips noun phrase without doing or printing anything
void SkipObjFromInput(int room)
{
  int obj;

  if (MatchCurWord("it") || MatchCurWord("them")) return;

  if (MatchCurWord("the") || MatchCurWord("a") || MatchCurWord("an")) {}

  obj = GetObjFromInput();
  if (obj == 0) obj = GetFixedObjFromInput(room);
}
//*****************************************************************************



//*****************************************************************************
//set TimePassed to 1 when an action completes successfully



//returns 1 if move blocked; if blocked, message printed before return
int MoveBlockedRoutine(int r, int dir)
{
  int i, d, m;

  i = RoomPassages[15*r+10]; //get state var index for room
  if (i == 0) return 0; //no state var for this room: move not blocked
  if (StateVar[i] != 0) return 0; //state var set: move not blocked

  i = 11;
  d = RoomPassages[15*r+(i+0)]; //move dir
  m = RoomPassages[15*r+(i+1)]; //move blocked msg
  if (d == dir)
  {
    if (m >= BL1) PrintBlockMsg(m - BL1);
    return 1; //move blocked
  }

  i = 13;
  d = RoomPassages[15*r+(i+0)]; //move dir
  m = RoomPassages[15*r+(i+1)]; //move blocked msg
  if (d == dir)
  {
    if (m >= BL1) PrintBlockMsg(m - BL1);
    return 1; //move blocked
  }

  return 0; //move not blocked
}



void ParseActionDirection(int action)
{
  int newroom, prev_darkness;

  newroom = RoomPassages[15*Obj[OBJ_YOU].loc + (action-A_NORTH)];
  if (newroom >= BL1)
  {
    PrintBlockMsg(newroom - BL1);
    return;
  }

  if (MoveBlockedRoutine(Obj[OBJ_YOU].loc, 1+(action-A_NORTH))) return; //blocked msg printed in routine

  prev_darkness = IsPlayerInDarkness();
  Obj[OBJ_YOU].loc = newroom;
  TimePassed = 1;
  if (IsPlayerInDarkness() && prev_darkness)
  {
    //kill player that tried to walk from dark to dark
    PrintLine("\n\n\n\n\nYou have just hit your head against an overhanging rock and fallen to the ground unconscious. I think it's all over.");
    GameOver = 1;
    return;
  }

  PrintPlayerRoomDesc(0);
}



void ParseActionTake(void)
{
  int obj;

  if (MatchCurWord("all") || MatchCurWord("everything")) //beginning of "take all"
  {
    int num_exceptions, nothing_taken, container;
    unsigned short exception[80]; //stores objects not to be taken by "take all"

    container = 0;
    num_exceptions = 0;
    if (MatchCurWord("but") || MatchCurWord("except"))
      for (;;)
    {
      obj = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (obj <= 0) return;

      if (obj == FOBJ_SCENERY_NOTVIS || obj == FOBJ_NOTVIS)
      {
        PrintLine("At least one of those objects isn't visible here!");
        return;
      }

      if (obj == FOBJ_AMB)
      {
        PrintLine("You need to be more specific about at least one of those objects.");
        return;
      }

      if (obj == OBJ_YOU || obj >= NUM_OBJECTS)
      {
        PrintLine("At least one of those objects can't be taken!");
        return;
      }

      if (Obj[obj].loc == 2048 + OBJ_YOU)
      {
        PrintLine("You're already holding at least one of those objects!");
        return;
      }

      if (IsObjVisible(obj) == 0)
      {
        PrintLine("At least one of those objects isn't visible here!");
        return;
      }

      exception[num_exceptions++] = obj;
      if (num_exceptions == 80 || CurWord == NumStrWords) break;

      if (MatchCurWord("from") || MatchCurWord("out"))
      {
        if (MatchCurWord("of")) CurWord--;
        CurWord--;                         //back up so from/out (of) can be processed below
        break;
      }

      if (MatchCurWord("then"))
      {
        CurWord--; //end of this turn's command; back up so "then" can be matched later
        break;
      }

      if (MatchCurWord("and") == 0)
      {
        PrintLine("Please use a comma or the word \"and\" between nouns.");
        return;
      }
    }

    if (MatchCurWord("from") || MatchCurWord("out"))
    {
      MatchCurWord("of");

      container = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (container <= 0) return;

      if (container == FOBJ_SCENERY_NOTVIS || container == FOBJ_NOTVIS)
      {
        PrintLine("At least one of those objects isn't visible here!");
        return;
      }

      if (container == OBJ_YOU || container >= NUM_OBJECTS)
      {
        PrintLine("You can't take anything out of that.");
        return;
      }

      if (IsObjVisible(container) == 0)
      {
        PrintObjectDesc(container, 0);
        PrintText(": ");

        PrintLine("You can't see that here.");

        return;
      }

      if ((Obj[container].prop & PROP_OPEN) == 0)
      {
        PrintObjectDesc(container, 0);
        PrintText(": ");

        if (Obj[container].prop & PROP_OPENABLE)
          PrintLine("You need to open it first.");
        else
          PrintLine("You can't take anything out of that.");

        return;
      }
    }

    nothing_taken = 1;
    for (;;)
    {
      int i;

      for (i=2; i<NUM_OBJECTS; i++)
      {
        obj = Obj[i].order;
        if (Obj[obj].loc == (container ? 2048 + container : Obj[OBJ_YOU].loc))
        {
          int j;

          for (j=0; j<num_exceptions; j++)
            if (obj == exception[j]) break;
          if (j == num_exceptions)
          {
            PrintObjectDesc(obj, 0);
            PrintText(": ");

            if (Obj[obj].prop & PROP_NOTTAKEABLE)
            {
              PrintLine("You can't take that.");
              return;
            }

            if (GetNumObjects(2048 + OBJ_YOU) >= MAX_INVENTORY)
            {
              PrintLine("You can't carry any more!");
              return;
            }

            Obj[obj].loc = 2048 + OBJ_YOU;
            MoveObjOrderToLast(obj);

            PrintLine("Okay.");

            TimePassed = 1;

            nothing_taken = 0;
            break;
          }
        }
      }
      if (i == NUM_OBJECTS) break;
    }

    if (nothing_taken)
      PrintLine("There was nothing to take.");
    return;
  } //end of "take all"


  {
    int i, num_takes;
    unsigned short take[80]; //stores objects to be taken

    num_takes = 0;
    for (;;)
    {
      obj = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (obj <= 0) return;

      take[num_takes++] = obj;
      if (num_takes == 80 || CurWord == NumStrWords) break;

      if (MatchCurWord("from") || MatchCurWord("out"))
      {
        PrintLine("You only need to specify a container with \"take all\".");
        return;
      }

      if (MatchCurWord("then"))
      {
        CurWord--; //end of this turn's command; back up so "then" can be matched later
        break;
      }

      if (MatchCurWord("and") == 0)
      {
        PrintLine("Please use a comma or the word \"and\" between nouns.");
        return;
      }
    }

    for (i=0; i<num_takes; i++)
    {
      obj = take[i];

      if (obj == FOBJ_SCENERY_NOTVIS || obj == FOBJ_NOTVIS)
      {
        if (num_takes > 1)
          PrintLine("At least one of those objects isn't visible here!");
        else
          PrintLine("You don't see that here!");
        return;
      }

      if (obj == FOBJ_AMB)
      {
        if (num_takes > 1)
          PrintLine("You need to be more specific about at least one of those objects.");
        else
          PrintLine("You need to be more specific.");
        return;
      }

      if (obj == OBJ_YOU || obj >= NUM_OBJECTS)
      {
        if (num_takes > 1)
          PrintLine("At least one of those objects can't be taken!");
        else
          PrintLine("You can't take that!");
        return;
      }
    }

    for (i=0; i<num_takes; i++)
    {
      obj = take[i];

      if (num_takes > 1)
      {
        PrintObjectDesc(obj, 0);
        PrintText(": ");
      }

      if (Obj[obj].loc == 2048 + OBJ_YOU)
        PrintLine("You're already holding that!");
      else if (IsObjVisible(obj) == 0)
        PrintLine("You can't see that here.");
      else
      {
        if (Obj[obj].prop & PROP_NOTTAKEABLE)
          PrintLine("You can't take that.");
        else if (GetNumObjects(2048 + OBJ_YOU) >= MAX_INVENTORY)
          PrintLine("You can't carry any more!");
        else
        {
          Obj[obj].loc = 2048 + OBJ_YOU;
          MoveObjOrderToLast(obj);
          PrintLine("Okay.");
          TimePassed = 1;
        }
      }
    }
  }
}



void ParseActionDropPut(int put_flag)
{
  int obj;

  if (MatchCurWord("all") || MatchCurWord("everything")) //beginning of "drop/put all"
  {
    int num_exceptions, nothing_dropped, into_flag, container;
    unsigned short exception[80]; //stores objects not to be dropped/put by "drop/put all"

    container = 0;
    num_exceptions = 0;
    if (MatchCurWord("but") || MatchCurWord("except"))
      for (;;)
    {
      obj = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (obj <= 0) return;

      if (obj == OBJ_YOU || obj >= NUM_OBJECTS)
      {
        PrintLine("You're not holding at least one of those objects!");
        return;
      }

      if (Obj[obj].loc != 2048 + OBJ_YOU)
      {
        PrintLine("You're not holding at least one of those objects!");
        return;
      }

      exception[num_exceptions++] = obj;
      if (num_exceptions == 80 || CurWord == NumStrWords) break;

      if (MatchCurWord("in") || MatchCurWord("into") || MatchCurWord("inside"))
      {
        CurWord--; //back up so in/into/inside can be processed below
        break;
      }

      if (MatchCurWord("then"))
      {
        CurWord--; //end of this turn's command; back up so "then" can be matched later
        break;
      }

      if (MatchCurWord("and") == 0)
      {
        PrintLine("Please use a comma or the word \"and\" between nouns.");
        return;
      }
    }

    //change "drop" to "put" (if not already) if container will be specified
    if (MatchCurWord("in") || MatchCurWord("into") || MatchCurWord("inside")) {into_flag = 1; put_flag = 1;}
    else into_flag = 0;

    if (put_flag)
    {
      if (into_flag == 0)
      {
        PrintLine("You need to specify a container.");
        return;
      }

      container = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (container <= 0) return;

      if (container == FOBJ_SCENERY_NOTVIS || container == FOBJ_NOTVIS)
      {
        PrintLine("At least one of those objects isn't visible here!");
        return;
      }

      if (container == OBJ_YOU || container >= NUM_OBJECTS)
      {
        PrintLine("You can't put anything into that.");
        return;
      }

      if (IsObjVisible(container) == 0)
      {
        PrintObjectDesc(container, 0);
        PrintText(": ");

        PrintLine("You can't see that here.");

        return;
      }

      if ((Obj[container].prop & PROP_OPEN) == 0)
      {
        PrintObjectDesc(container, 0);
        PrintText(": ");

        if (Obj[container].prop & PROP_OPENABLE)
          PrintLine("You need to open it first.");
        else
          PrintLine("You can't put anything into that.");

        return;
      }
    }

    nothing_dropped = 1;
    for (;;)
    {
      int i;

      for (i=2; i<NUM_OBJECTS; i++)
      {
        obj = Obj[i].order;
        if (Obj[obj].loc == 2048 + OBJ_YOU && obj != container)
        {
          int j;

          for (j=0; j<num_exceptions; j++)
            if (obj == exception[j]) break;
          if (j == num_exceptions)
          {
            PrintObjectDesc(obj, 0);
            PrintText(": ");

            if (put_flag && GetNumObjects(2048 + container) >= MAX_CAPACITY)
            {
              PrintLine("It won't hold any more!");
              return;
            }

            Obj[obj].loc = put_flag ? 2048 + container : Obj[OBJ_YOU].loc;
            MoveObjOrderToLast(obj);

            PrintLine("Okay.");

            TimePassed = 1;

            nothing_dropped = 0;
            break;
          }
        }
      }
      if (i == NUM_OBJECTS) break;
    }

    if (nothing_dropped)
    {
      if (put_flag)
        PrintLine("There was nothing to put into it.");
      else
        PrintLine("There was nothing to drop.");
    }
    return;
  } //end of "drop/put all"


  {
    int i, num_drops, container;
    unsigned short drop[80]; //stores objects to be dropped

    container = 0;
    num_drops = 0;
    for (;;)
    {
      obj = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (obj <= 0) return;

      drop[num_drops++] = obj;
      if (num_drops == 80 || CurWord == NumStrWords) break;

      if (MatchCurWord("in") || MatchCurWord("into") || MatchCurWord("inside"))
      {
        put_flag = 1; //turn drop into put if not already

        container = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (container <= 0) return;

        if (container == FOBJ_SCENERY_NOTVIS || container == FOBJ_NOTVIS)
        {
          PrintLine("At least one of those objects isn't visible here!");
          return;
        }

        if (container == OBJ_YOU || container >= NUM_OBJECTS)
        {
          PrintLine("You can't put anything into that.");
          return;
        }

        if (IsObjVisible(container) == 0)
        {
          PrintObjectDesc(container, 0);
          PrintText(": ");

          PrintLine("You can't see that here.");

          return;
        }

        if ((Obj[container].prop & PROP_OPEN) == 0)
        {
          PrintObjectDesc(container, 0);
          PrintText(": ");

          if (Obj[container].prop & PROP_OPENABLE)
            PrintLine("You need to open it first.");
          else
            PrintLine("You can't put anything into that.");

          return;
        }

        break;
      }

      if (MatchCurWord("then"))
      {
        CurWord--; //end of this turn's command; back up so "then" can be matched later
        break;
      }

      if (MatchCurWord("and") == 0)
      {
        PrintLine("Please use a comma or the word \"and\" between nouns.");
        return;
      }
    }

    if (put_flag && container == 0)
    {
      PrintLine("You need to specify a container.");
      return;
    }

    for (i=0; i<num_drops; i++)
    {
      obj = drop[i];

      if (obj == OBJ_YOU || obj >= NUM_OBJECTS)
      {
        if (num_drops > 1)
          PrintLine("You're not holding at least one of those objects!");
        else
          PrintLine("You're not holding that!");
        return;
      }
    }

    for (i=0; i<num_drops; i++)
    {
      obj = drop[i];

      if (num_drops > 1)
      {
        PrintObjectDesc(obj, 0);
        PrintText(": ");
      }

      if (Obj[obj].loc != 2048 + OBJ_YOU)
        PrintLine("You're not holding that!");
      else if (obj == container)
        PrintLine("But it would disappear from reality!");
      else
      {
        if (put_flag && GetNumObjects(2048 + container) >= MAX_CAPACITY)
          PrintLine("It won't hold any more!");
        else
        {
          Obj[obj].loc = put_flag ? 2048 + container : Obj[OBJ_YOU].loc;
          MoveObjOrderToLast(obj);
          PrintLine("Okay.");
          TimePassed = 1;
        }
      }
    }
  }
}



void ParseActionWith(int action, char *verb_phrase)
{
  int obj, with, i;

  obj = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (obj <= 0) return;

  if (obj == FOBJ_SCENERY_NOTVIS || obj == FOBJ_NOTVIS)
  {
    PrintLine("At least one of those objects isn't visible here!");
    return;
  }

  if (obj == FOBJ_AMB)
  {
    PrintLine("You need to be more specific about at least one of those objects.");
    return;
  }

  with = 0;
  if (MatchCurWord("with") || MatchCurWord("using"))
  {
    with = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (with <= 0) return;

    if (with == FOBJ_SCENERY_NOTVIS || with == FOBJ_NOTVIS)
    {
      PrintLine("At least one of those objects isn't visible here!");
      return;
    }

    if (with == FOBJ_AMB)
    {
      PrintLine("You need to be more specific about at least one of those objects.");
      return;
    }

    if (with == OBJ_YOU)
    {
      PrintLine("You can't use yourself!");
      return;
    }

    if (with >= NUM_OBJECTS)
    {
      PrintLine("You can't use that!");
      return;
    }

    if (Obj[with].loc != 2048 + OBJ_YOU)
    {
      PrintLine("You're not holding that.");
      return;
    }
  }

  //fixed objects only
  i = 0;
  for (;;)
  {
    if (DoMiscWith[i].f == 0) break;

    if (DoMiscWith[i].action == action && DoMiscWith[i].obj >= NUM_OBJECTS && DoMiscWith[i].obj == obj)
    {
      DoMiscWith[i].f(with);
      return;
    }

    i++;
  }

  if (obj == OBJ_YOU)
  {
    PrintLine("Seriously?!");
    return;
  }

  if (obj >= NUM_OBJECTS)
  {
    PrintText("You can't ");
    PrintText(verb_phrase);
    PrintText(" that.\n");
    return;
  }

  if (IsObjVisible(obj) == 0)
  {
    PrintLine("You can't see that here.");
    return;
  }

  //non-fixed objects only
  i = 0;
  for (;;)
  {
    if (DoMiscWith[i].f == 0) break;

    if (DoMiscWith[i].action == action && DoMiscWith[i].obj < NUM_OBJECTS && DoMiscWith[i].obj == obj)
    {
      DoMiscWith[i].f(with);
      return;
    }

    i++;
  }

  PrintText("You can't ");
  PrintText(verb_phrase);
  PrintText(" that.\n");
}



void ParseActionExamine(void)
{
  int obj, i, flag;

  obj = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (obj <= 0) return;

  if (obj == FOBJ_SCENERY_NOTVIS || obj == FOBJ_NOTVIS)
  {
    PrintLine("That isn't visible here!");
    return;
  }

  if (obj == FOBJ_AMB)
  {
    PrintLine("You need to be more specific.");
    return;
  }

  TimePassed = 1;

  //fixed objects only
  i = 0;
  for (;;)
  {
    if (DoMisc[i].f == 0) break;

    if (DoMisc[i].action == A_EXAMINE && DoMisc[i].obj >= NUM_OBJECTS && DoMisc[i].obj == obj)
    {
      DoMisc[i].f();
      return;
    }

    i++;
  }

  if (obj == OBJ_YOU)
  {
    PrintLine("You look fairly ordinary.");
    return;
  }

  if (obj >= NUM_OBJECTS)
  {
    PrintLine("You don't see anything unusual.");
    return;
  }

  if (IsObjVisible(obj) == 0)
  {
    TimePassed = 0;
    PrintLine("You can't see that here.");
    return;
  }

  //non-fixed objects only
  i = 0;
  for (;;)
  {
    if (DoMisc[i].f == 0) break;

    if (DoMisc[i].action == A_EXAMINE && DoMisc[i].obj < NUM_OBJECTS && DoMisc[i].obj == obj)
    {
      DoMisc[i].f();
      return;
    }

    i++;
  }

  flag = 0;

  if (Obj[obj].prop & PROP_LIGHTABLE)
  {
    flag = 1;

    if (Obj[obj].prop & PROP_LIT)
      PrintLine("It's on.");
    else
      PrintLine("It's off.");
  }

  if (Obj[obj].prop & PROP_OPENABLE)
  {
    flag = 1;

    if (Obj[obj].prop & PROP_OPEN)
    {
      int i;

      PrintLine("It's open.");

      for (i=2; i<NUM_OBJECTS; i++)
      {
        int obj_inside = Obj[i].order;

        if (Obj[obj_inside].loc == 2048 + obj)
        {
          if (flag == 1)
          {
            PrintLine("It contains:");
            flag = 2;
          }

          PrintText("  ");
          PrintObjectDesc(obj_inside, 0);
          PrintText("\n");
        }
      }
    }
    else
      PrintLine("It's closed.");
  }

  if (flag == 0)
    PrintLine("You don't see anything unusual.");
}



void ParseActionGive(void)
{
  int obj, to, flag, swap, i;

  obj = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (obj <= 0) return;
  flag = MatchCurWord("to");
  to = GetAllObjFromInput(Obj[OBJ_YOU].loc); if (to <= 0) return;

  if (flag == 0)
  {
    //if "to" omitted, swap obj and to, as in "give plant water"
    swap = obj;
    obj = to;
    to = swap;
  }

  if (obj == FOBJ_SCENERY_NOTVIS || obj == FOBJ_NOTVIS)
  {
    PrintLine("At least one of those objects isn't visible here!");
    return;
  }

  if (obj == FOBJ_AMB)
  {
    PrintLine("You need to be more specific about at least one of those objects.");
    return;
  }

  if (obj == OBJ_YOU)
  {
    PrintLine("Seriously?!");
    return;
  }

  if (obj >= NUM_OBJECTS)
  {
    PrintLine("You aren't holding that!");
    return;
  }

  if (Obj[obj].loc != 2048 + OBJ_YOU)
  {
    PrintLine("You aren't holding that!");
    return;
  }

  if (to == FOBJ_SCENERY_NOTVIS || to == FOBJ_NOTVIS)
  {
    PrintLine("At least one of those objects isn't visible here!");
    return;
  }

  if (to == FOBJ_AMB)
  {
    PrintLine("You need to be more specific about at least one of those objects.");
    return;
  }

  //fixed objects only
  i = 0;
  for (;;)
  {
    if (DoMiscTo[i].f == 0) break;

    if (DoMiscTo[i].action == A_GIVE && DoMiscTo[i].to >= NUM_OBJECTS && DoMiscTo[i].to == to)
    {
      DoMiscTo[i].f(obj);
      return;
    }

    i++;
  }

  if (to == OBJ_YOU)
  {
    PrintLine("Seriously?!");
    return;
  }

  if (to >= NUM_OBJECTS)
  {
    PrintLine("You can't give something to that!");
    return;
  }

  if (IsObjVisible(to) == 0)
  {
    PrintLine("At least one of those objects isn't visible here!");
    return;
  }

  //non-fixed objects only
  i = 0;
  for (;;)
  {
    if (DoMiscTo[i].f == 0) break;

    if (DoMiscTo[i].action == A_GIVE && DoMiscTo[i].to < NUM_OBJECTS && DoMiscTo[i].to == to)
    {
      DoMiscTo[i].f(obj);
      return;
    }

    i++;
  }

  PrintLine("You can't give something to that!");
}



void ParseActionRestartOrQuit(int action)
{
  for (;;)
  {
    GetWords((action == A_RESTART) ? "Do you want to restart the game? " : "Do you want to quit now? ");

    if (MatchCurWord("y") || MatchCurWord("yes"))
    {
      GameOver = (action == A_RESTART) ? 2 : 1;
      return;
    }

    if (MatchCurWord("n") || MatchCurWord("no")) return;

    PrintLine("Please answer yes or no.");
  }
}



void ActivateObj(int obj)
{
  int prev_darkness;

  if ((Obj[obj].prop & PROP_LIGHTABLE) == 0)
  {
    PrintLine("You can't turn that on!");
    return;
  }

  if (Obj[obj].prop & PROP_LIT)
  {
    PrintLine("It's already on!");
    return;
  }

  prev_darkness = IsPlayerInDarkness();
  Obj[obj].prop |= PROP_LIT;
  PrintLine("It's on.");
  TimePassed = 1;
  if (IsPlayerInDarkness() != prev_darkness && prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}



void DeactivateObj(int obj)
{
  int prev_darkness;

  if ((Obj[obj].prop & PROP_LIGHTABLE) == 0)
  {
    PrintLine("You can't turn that off!");
    return;
  }

  if ((Obj[obj].prop & PROP_LIT) == 0)
  {
    PrintLine("It's already off!");
    return;
  }

  prev_darkness = IsPlayerInDarkness();
  Obj[obj].prop &= ~PROP_LIT;
  PrintLine("It's off.");
  TimePassed = 1;
  if (IsPlayerInDarkness() != prev_darkness && prev_darkness == 0)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}



void OpenObj(int obj)
{
  int prev_darkness;

  if ((Obj[obj].prop & PROP_OPENABLE) == 0)
  {
    PrintLine("You can't open that.");
    return;
  }

  if (Obj[obj].prop & PROP_OPEN)
  {
    PrintLine("It's already open.");
    return;
  }

  prev_darkness = IsPlayerInDarkness();

  Obj[obj].prop |= PROP_OPEN; //open object
  PrintLine("Okay.");
  TimePassed = 1;

  if (IsPlayerInDarkness() == 0)
  {
    int flag = 0, i;

    for (i=2; i<NUM_OBJECTS; i++)
    {
      int obj_inside = Obj[i].order;

      if (Obj[obj_inside].loc == 2048 + obj)
      {
        if (flag == 0)
        {
          PrintLine("It contains:");
          flag = 1;
        }

        PrintText("  ");
        PrintObjectDesc(obj_inside, 0);
        PrintText("\n");
      }
    }
  }

  //did room become lighted when object was opened
  if (IsPlayerInDarkness() != prev_darkness && prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}



void CloseObj(int obj)
{
  int prev_darkness;

  if ((Obj[obj].prop & PROP_OPENABLE) == 0)
  {
    PrintLine("You can't close that.");
    return;
  }

  if ((Obj[obj].prop & PROP_OPEN) == 0)
  {
    PrintLine("It's already closed.");
    return;
  }

  prev_darkness = IsPlayerInDarkness();

  Obj[obj].prop &= ~PROP_OPEN; //close object
  PrintLine("Okay.");
  TimePassed = 1;

  //did room become darkened when object was closed
  if (IsPlayerInDarkness() != prev_darkness && prev_darkness == 0)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}



void LookInObj(int obj)
{
  int flag = 0, i;

  if ((Obj[obj].prop & PROP_OPENABLE) == 0)
  {
    PrintLine("You can't look inside that.");
    return;
  }

  if ((Obj[obj].prop & PROP_OPEN) == 0)
  {
    PrintLine("It's closed.");
    return;
  }

  for (i=2; i<NUM_OBJECTS; i++)
  {
    int obj_inside = Obj[i].order;

    if (Obj[obj_inside].loc == 2048 + obj)
    {
      if (flag == 0)
      {
        PrintLine("It contains:");
        flag = 1;
      }

      PrintText("  ");
      PrintObjectDesc(obj_inside, 0);
      PrintText("\n");
    }
  }

  if (flag == 0)
  {
    PrintLine("It's empty.");
  }
}



//returns 1 if action on obj processed; otherwise returns 0 so calling function can continue processing
//match_hack: skip this word after getting obj; part of hack for commands such as "turn obj on"; ignore if null
//must_hold: if flag set, print msg if player not holding obj

int DoActionOnObject(int action, int *obj, char *match_hack, int must_hold)
{
  int i;

  *obj = GetAllObjFromInput(Obj[OBJ_YOU].loc);
  if (*obj <= 0) return 1; //can just return because msg already printed

  if (*obj == FOBJ_SCENERY_NOTVIS || *obj == FOBJ_NOTVIS)
  {
    PrintLine("That isn't visible here!");
    return 1;
  }

  if (*obj == FOBJ_AMB)
  {
    PrintLine("You need to be more specific.");
    return 1;
  }

  if (match_hack != 0) MatchCurWord(match_hack); //skip specified word

  //fixed objects only
  i = 0;
  for (;;)
  {
    if (DoMisc[i].f == 0) break;

    if (DoMisc[i].action == action && DoMisc[i].obj >= NUM_OBJECTS && DoMisc[i].obj == *obj)
    {
      DoMisc[i].f();
      return 1;
    }

    i++;
  }

  if (*obj == OBJ_YOU)
  {
    PrintLine("Seriously?!");
    return 1;
  }

  if (*obj >= NUM_OBJECTS)
  {
    switch (action)
    {
      case A_ACTIVATE:   PrintLine("You can't turn that on!");              return 1;
      case A_DEACTIVATE: PrintLine("You can't turn that off!");             return 1;
      case A_OPEN:       PrintLine("You can't open that!");                 return 1;
      case A_CLOSE:      PrintLine("You can't close that!");                return 1;
      case A_MOUNT:      PrintLine("You can't get on that!");               return 1;
      case A_DISMOUNT:   PrintLine("You're not on that!");                  return 1;
      case A_EAT:        PrintLine("That does not sound very appetizing!"); return 1;
      case A_DRINK:      PrintLine("That does not sound very appetizing!"); return 1;
      case A_WEAR:       PrintLine("You can't wear that!");                 return 1;
      case A_REMOVE:     PrintLine("You aren't wearing that!");             return 1;
      case A_PLAY:       PrintLine("You can't play that!");                 return 1;
      case A_SLEEPON:    PrintLine("You can't sleep on that!");             return 1;
      case A_JUMPINTO:   PrintLine("You can't jump into that!");            return 1;
      case A_LOOKIN:     PrintLine("You can't look inside that.");          return 1;

      default:           PrintLine("That would be futile.");                return 1;
    }
  }

  if (IsObjVisible(*obj) == 0)
  {
    PrintLine("You can't see that here.");
    return 1;
  }

  if (must_hold && Obj[*obj].loc != 2048 + OBJ_YOU)
  {
    PrintLine("You aren't holding it.");
    return 1;
  }

  //non-fixed objects only
  i = 0;
  for (;;)
  {
    if (DoMisc[i].f == 0) break;

    if (DoMisc[i].action == action && DoMisc[i].obj < NUM_OBJECTS && DoMisc[i].obj == *obj)
    {
      DoMisc[i].f();
      return 1;
    }

    i++;
  }

  return 0;
}



void ParseActionGeneric(int action)
{
  int obj;

  if (DoActionOnObject(action, &obj, 0, 0)) return;
  PrintLine("That would be futile.");
}



void ParseActionActivate(int action)
{
  int obj;

  // "on" is part of hack for "turn object on/off"
  if (DoActionOnObject(action, &obj, "on", 0)) return;
  ActivateObj(obj);
}



void ParseActionDeactivate(int action)
{
  int obj;

  // "off" is part of hack for "turn object on/off"
  if (DoActionOnObject(action, &obj, "off", 0)) return;
  DeactivateObj(obj);
}



void ParseActionOpen(int action)
{
  int obj;

  if (DoActionOnObject(action, &obj, 0, 0)) return;
  OpenObj(obj);
}



void ParseActionClose(int action)
{
  int obj;

  if (DoActionOnObject(action, &obj, 0, 0)) return;
  CloseObj(obj);
}



void ParseActionMount(int action)
{
  int obj;

  if (DoActionOnObject(action, &obj, 0, 0)) return;
  PrintLine("You can't get on that!");
}



void ParseActionDismount(int action)
{
  int obj;

  if (DoActionOnObject(action, &obj, 0, 0)) return;
  PrintLine("You're not on that!");
}



void ParseActionEatDrink(int action)
{
  int obj;

  // 1: must be holding obj
  if (DoActionOnObject(action, &obj, 0, 1)) return;
  PrintLine("That does not sound very appetizing!");
}



void ParseActionWear(int action)
{
  int obj;

  // "on" is part of hack for "put obj on"; 1: must be holding obj
  if (DoActionOnObject(action, &obj, "on", 1)) return;
  PrintLine("You can't wear that!");
}



void ParseActionRemove(int action)
{
  int obj;

  // "off" is part of hack for "take obj off"; 1: must be holding obj
  if (DoActionOnObject(action, &obj, "off", 1)) return;
  PrintLine("You aren't wearing that.");
}



void ParseActionPlay(int action)
{
  int obj;

  // 1: must be holding obj
  if (DoActionOnObject(action, &obj, 0, 1)) return;
  PrintLine("You can't play that!");
}



void ParseActionSleepOn(int action)
{
  int obj;

  if (DoActionOnObject(action, &obj, 0, 0)) return;
  PrintLine("You can't sleep on that!");
}



void ParseActionJumpInto(int action)
{
  int obj;

  if (DoActionOnObject(action, &obj, 0, 0)) return;
  PrintLine("You can't jump into that!");
}



void ParseActionLookIn(int action)
{
  int obj;

  if (DoActionOnObject(action, &obj, 0, 0)) return;
  LookInObj(obj);
}
//*****************************************************************************



//*****************************************************************************
void Parse(void)
{
  int action, i, temp;

  TimePassed = 0;

  if (CurWord == NumStrWords || MatchCurWord("then")) return;

  if (CurWord > 0 && Verbosity != V_SUPERBRIEF) PrintNewLine(); //print blank line between turns

  action = GetActionFromInput();
  if (action == 0)
  {
    PrintLine("I didn't recognize a verb in that sentence.");
    return;
  }

  //hacks to allow "turn obj on/off" "take obj off" "put obj on" to be
  //caught by "activate/deactivate" "remove" and "wear"
  //NOTE: this will allow strange commands like "spin obj on" "remove obj off" "wear obj on"
  temp = CurWord;
  SkipObjFromInput(Obj[OBJ_YOU].loc); //skip object
  if (action == A_TURN && MatchCurWord("on" )) action = A_ACTIVATE;   //turn obj on
  if (action == A_TURN && MatchCurWord("off")) action = A_DEACTIVATE; //turn obj off
  if (action == A_TAKE && MatchCurWord("off")) action = A_REMOVE;     //take obj off
  if (action == A_PUT  && MatchCurWord("on" )) action = A_WEAR;       //put obj on
  CurWord = temp;

  //replace "go dir" with "dir"
  if (action == A_GO)
  {
    for (i=0; i<20; i++)
      if (MatchCurWord(DirName[i])) break;
    if (i == 20)
    {
      PrintLine("I couldn't find a direction to go in that sentence.");
      return;
    }

    action = A_NORTH + (i/2);
  }

  //special movements; executed function can fall through
  i = 0;
  for (;;)
  {
    if (GoFrom[i].f == 0) break;
    if (GoFrom[i].room == Obj[OBJ_YOU].loc && GoFrom[i].action == action)
    {
      if (GoFrom[i].f()) return;
    }
    i++;
  }

  if (action >= A_NORTH && action <= A_DOWN)
  {
    ParseActionDirection(action);
    return;
  }

  switch (action)
  {
    case A_SAVE        : DoSave();                            return;
    case A_RESTORE     : DoRestore();                         return;
    case A_SCORE       : PrintScore();                        return;
    case A_RESTART     : ParseActionRestartOrQuit(action);    return;
    case A_QUIT        : ParseActionRestartOrQuit(action);    return;

    case A_BRIEF       : Verbosity = V_BRIEF;      PrintLine("Brief descriptions.");                             return;
    case A_VERBOSE     : Verbosity = V_VERBOSE;    PrintLine("Verbose descriptions.\n"); PrintPlayerRoomDesc(1); return;
    case A_SUPERBRIEF  : Verbosity = V_SUPERBRIEF; PrintLine("Superbrief descriptions.");                        return;

    case A_LOOK        : PrintPlayerRoomDesc(1);              return;
    case A_WAIT        : PrintLine("Zzz."); TimePassed = 1;   return;
    case A_ACTIVATE    : ParseActionActivate(action);         return;
    case A_DEACTIVATE  : ParseActionDeactivate(action);       return;
    case A_OPEN        : ParseActionOpen(action);             return;
    case A_CLOSE       : ParseActionClose(action);            return;
  }

  //actions above this line work when in darkness
  //NOTE: when in darkness, player is still allowed to open/close because light might be inside container
  if (IsPlayerInDarkness()) {PrintLine("It is too dark to see anything."); return;}
  //actions below this line are not possible when in darkness

  switch (action)
  {
    case A_INVENTORY   : PrintPresentObjects(2048 + OBJ_YOU); return;
    case A_TAKE        : ParseActionTake();                   return;
    case A_DROP        : ParseActionDropPut(0);               return;
    case A_PUT         : ParseActionDropPut(1);               return;
    case A_BREAK       : ParseActionWith(action, "break");    return;
    case A_DIG         : ParseActionWith(action, "dig");      return;
    case A_LOCK        : ParseActionWith(action, "lock");     return;
    case A_UNLOCK      : ParseActionWith(action, "unlock");   return;
    case A_PRY         : ParseActionWith(action, "pry");      return;
    case A_TURN        : ParseActionWith(action, "turn");     return;
    case A_OIL         : ParseActionWith(action, "oil");      return;
    case A_EXAMINE     : ParseActionExamine();                return;
    case A_GIVE        : ParseActionGive();                   return;
    case A_JUMP        : DoJump();                            return;
    case A_SLEEP       : DoSleep();                           return;
    case A_MOVE        : ParseActionGeneric(action);          return;
    case A_PUSH        : ParseActionGeneric(action);          return;
    case A_PULL        : ParseActionGeneric(action);          return;
    case A_KNOCK       : ParseActionGeneric(action);          return;
    case A_LOOKBEHIND  : ParseActionGeneric(action);          return;
    case A_LOOKON      : ParseActionGeneric(action);          return;
    case A_LOOKTHROUGH : ParseActionGeneric(action);          return;
    case A_LOOKUNDER   : ParseActionGeneric(action);          return;
    case A_JUMPOVER    : ParseActionGeneric(action);          return;
    case A_MOUNT       : ParseActionMount(action);            return;
    case A_DISMOUNT    : ParseActionDismount(action);         return;
    case A_EAT         : ParseActionEatDrink(action);         return;
    case A_DRINK       : ParseActionEatDrink(action);         return;
    case A_WEAR        : ParseActionWear(action);             return;
    case A_REMOVE      : ParseActionRemove(action);           return;
    case A_PLAY        : ParseActionPlay(action);             return;
    case A_SLEEPON     : ParseActionSleepOn(action);          return;
    case A_JUMPINTO    : ParseActionJumpInto(action);         return;
    case A_LOOKIN      : ParseActionLookIn(action);           return;

    default: PrintLine("I didn't recognize that command."); return;
  }
  //not reached
}
//*****************************************************************************



//#############################################################################
void Shutdown(void)
{
  if (InputStream != NULL && InputStream != stdin) fclose(InputStream);
}



void RestartGame(void)
{
  int i;

  InitGameState();

  NumStrWords = 0;
  ItObj = 0;
  GameOver = 0;

  // clear screen
  i = MAX_PRINTED_LINES;
  while (i-- > 0) {PrintNewLine(); NumPrintedLines = 0;}

  PrintLine("Welcome to Crystal Caverns!\n\nOriginal Apple ][ version by Daniel Kitchen\n(c) 1982 Imaginative Systems Software\n\n  C port and parser (c) 2021 by Donnie Russell II\n\n");

  PrintPlayerRoomDesc(0);
}



void main(int argc, char *argv[])
{
  //process command line arguments
  atexit(Shutdown);
  if (argc < 2) InputStream = stdin;
  else
  {
    InputStream = fopen(argv[1], "r");
    if (InputStream == NULL) InputStream = stdin;
  }

#ifndef NO_STATUS_LINE
  VTMode = EnableVTMode();
#endif

  srand(time(NULL));

  RestartGame();

  for (;;)
  {
    if (NumStrWords == 0)
    {
      for (;;)
      {
        if (Verbosity != V_SUPERBRIEF) PrintNewLine();
        GetWords(">");
        if (NumStrWords == 0) PrintLine("Please type your command.");
        else break;
      }
    }

    PrevItObj = ItObj;
    ItObj = 0;

    Parse();

    if (TimePassed) NumMoves++;

    if (HasWon())
    {
      PrintLine("\n\n\n\n\nCongratulations! You have just completed Crystal Caverns. In honor of your triumphant conquest, pat yourself on the back. Your successful return of the missing treasures to Crystal Caverns Estate has earned you ownership of the estate.");
      GameOver = 1;
    }

    if (TimePassed && GameOver == 0) RunEventRoutines();

    if (GameOver)
    {
      if (GameOver == 2) {RestartGame(); continue;}

      PrintScore();
      for (;;)
      {
        GetWords("\nDo you want to restart, restore or exit? ");
        if (MatchCurWord("restart")) {RestartGame(); break;}
        if (MatchCurWord("restore")) {if (DoRestore()) break;}
        if (MatchCurWord("exit")) exit(0);
      }
      continue;
    }

    //if no time passed abort parsing any remaining sentences
    if (TimePassed == 0 || MatchCurWord("then") == 0) NumStrWords = 0;

    //"continue"s above take us here
  }
}
//#############################################################################

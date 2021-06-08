// Crystal Caverns
// (c) 1982 by Daniel Kitchen
// C port and parser (c) 2021 by Donnie Russell II

// This source code is provided for personal, educational use only.
// You are welcome to use this source code to develop your own works,
// but the story-related content belongs to the original author(s).



#include "_def.h"
#include "_tables.h"



int NumMoves;

int LampTurnsLeft;
int PackTurnsLeft;
int StandStoolLoc;
int FootbridgeStability;
int WearingParachute;

//all should be zero at game start
unsigned char StateVar[NUM_STATE_VARS];



//from parser.c
extern int PrintDelayFlag;
extern int NumStrWords;
extern int CurWord;
extern int ItObj;
extern unsigned char TimePassed;
extern unsigned char GameOver;



//*****************************************************************************

//get turns left in lamp plus pack, if it is inside lamp

int GetLampAndPackTurnsLeft(void)
{
  int turns;

  turns = LampTurnsLeft;
  if (Obj[OBJ_PACK].loc == 2048 + OBJ_LAMP) turns += PackTurnsLeft;

  return turns;
}
//*****************************************************************************



//*****************************************************************************

//these functions return 1 if action completed; otherwise fall through


int GoFrom_chasm_down(void)
{
  if (WearingParachute)
  {
    //PrintLine("Your parachute refuses to open. (Oh no!)");

    PrintLine("You're falling down into the chasm. Below you is a wide ledge jutting out from the wall of the chasm. The parachute slows you enough for a safe landing on the ledge.\n\n\n\n\n\n\n");
    TimePassed = 1;
    Obj[OBJ_YOU].loc = 138;
    PrintPlayerRoomDesc(0);
    return 1;
  }

  PrintLine("\n\n\n\n\nYou find yourself falling dangerously toward the ground as you realize that this was not the best move you could have made. You hit the ground with a loud thump! It's all over.");
  TimePassed = 1;
  GameOver = 1;
  return 1;
}



int GoFrom_fissure_down(void)
{
  if (WearingParachute)
  {
    //PrintLine("Your parachute refuses to open. (Oh no!)");

    PrintLine("You find yourself falling into the heart of the fissure. Your parachute slows you down as you descend into the mist below.\n\n\n\n\n\n\n");
    TimePassed = 1;
    Obj[OBJ_YOU].loc = 78;
    PrintPlayerRoomDesc(0);
    return 1;
  }

  PrintLine("\n\n\n\n\nYou find yourself falling dangerously toward the ground as you realize that this was not the best move you could have made. You hit the ground with a loud thump! It's all over.");
  TimePassed = 1;
  GameOver = 1;
  return 1;
}



int GoFrom_confusingroom(void)
{
  //75% chance if you don't have compass that...
  //when trying a blocked exit (or east, which leads back to confusing room),
  //instead use a random good exit
  if (Obj[OBJ_COMPASS].loc != 2048 + OBJ_YOU && (rand() % 4) < 3)
  {
    int randomroom[5] = {58, 76, 83, 103, 106};

    TimePassed = 1;
    Obj[OBJ_YOU].loc = randomroom[rand() % 5];
    PrintPlayerRoomDesc(0);
    return 1;
  }

  return 0;
}



struct GOFROM_STRUCT GoFrom[] =
{
  { 18, A_DOWN      , GoFrom_chasm_down    },
  { 60, A_DOWN      , GoFrom_fissure_down  },
  { 75, A_NORTH     , GoFrom_confusingroom },
  { 75, A_EAST      , GoFrom_confusingroom },
  { 75, A_NORTHEAST , GoFrom_confusingroom },
  { 75, A_SOUTHEAST , GoFrom_confusingroom },
  { 75, A_SOUTHWEST , GoFrom_confusingroom },

  { 0, 0, 0 }
};
//*****************************************************************************



//*****************************************************************************
void PrintUsingMsg(int obj)
{
  PrintText("(using ");
  PrintObjectDesc(obj, 0);
  PrintText(")\n");
}



void PrintFutileMsg(int obj)
{
  PrintText("Using ");
  PrintObjectDesc(obj, 0);
  PrintText(" would be futile.\n");
}



void DoMiscWith_break_rockwall(int with)
{
  int need = OBJ_PICKAXE, state = STATE_BOULDERWALL;

  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state]) {PrintLine("At least one of those objects isn't visible here!"); return;}
  PrintLine("The wall of boulders breaks up and falls to the ground, covering the pool of quicksand!");
  StateVar[state] = 1;
  TimePassed = 1;
}


void DoMiscWith_break_globe(int with)
{
  int need = OBJ_PICKAXE, state = STATE_GLOBEHOLE;

  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state]) {PrintLine("There's already a hole in it."); return;}
  PrintLine("You have put a small hole in the bottom of the globe.");
  StateVar[state] = 1;
  TimePassed = 1;
}


void DoMiscWith_pry_floorboards(int with)
{
  int need = OBJ_CROWBAR, state = STATE_SHEDFLOOR;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state]) {PrintLine("A hole has already been made in the floor."); return;}
  PrintLine("As you work on removing the rotted floorboards, they collapse under your weight and you fall into the darkness below.\n\n\n\nFortunately, a large air cushion has broken your fall.\n");
  StateVar[state] = 1;
  Obj[OBJ_YOU].loc = 26;
  Obj[OBJ_WOOD].loc = 26;
  PrintPlayerRoomDesc(0);
  TimePassed = 1;
}


void DoMiscWith_oil_trapdoor(int with)
{
  int need = OBJ_OIL, state = STATE_TRAPDOOROILED;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state]) {PrintLine("It has already been oiled."); return;}
  PrintLine("The oil seeps onto the hinge. The door should now open freely.");
  StateVar[state] = 1;
  TimePassed = 1;
}


void DoMiscWith_dig_clearground(int with)
{
  int need = OBJ_SHOVEL;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[STATE_CLEARINGHOLE]) {PrintLine("There's already a large hole in the ground here."); return;}
  StateVar[STATE_CLEARINGHOLE] = 1;
  PrintLine("The ground gives way under your feet and you find yourself falling down into the bowels of the earth. (Well, not really that far down!)\n\n\n\n");
  Obj[OBJ_YOU].loc = 28;
  PrintPlayerRoomDesc(0);
  TimePassed = 1;
}


void DoMiscWith_dig_pitfloor(int with)
{
  int need = OBJ_SHOVEL, find = OBJ_TUSK;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (Obj[find].loc != 0) {PrintLine("You don't find anything else."); return;}
  PrintLine("Your shovel has unearthed a small ivory tusk.");
  Obj[find].loc = Obj[OBJ_YOU].loc;
  TimePassed = 1;
}


void DoMiscWith_dig_sand(int with)
{
  int need = OBJ_SHOVEL, find = OBJ_MAGAZINE;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (Obj[find].loc != 0) {PrintLine("You don't find anything else."); return;}
  PrintLine("Your digging has uncovered a large sand crab that is reading a magazine. Startled, the crab drops the magazine and disappears in the sand.");
  Obj[find].loc = Obj[OBJ_YOU].loc;
  TimePassed = 1;
}


void DoMiscWith_lock_gate(int with)
{
  int need = OBJ_KEY, state = STATE_GATEUNLOCKED;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state] == 0) {PrintLine("It is already locked."); return;}
  StateVar[state] = 0;
  PrintLine("It is now closed and locked.");
  TimePassed = 1;
}


void DoMiscWith_unlock_gate(int with)
{
  int need = OBJ_KEY, state = STATE_GATEUNLOCKED;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state]) {PrintLine("It is already unlocked."); return;}
  StateVar[state] = 1;
  PrintLine("It is now unlocked and open.");
  TimePassed = 1;
}


void DoMiscWith_lock_steeldoor(int with)
{
  int need = OBJ_KEY, state = STATE_STEELDOOR;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state] == 0) {PrintLine("It is already locked."); return;}
  StateVar[state] = 0;
  PrintLine("It is now closed and locked.");
  TimePassed = 1;
}


void DoMiscWith_unlock_steeldoor(int with)
{
  int need = OBJ_KEY, state = STATE_STEELDOOR;

  if (with == 0 && Obj[need].loc == 2048 + OBJ_YOU) {with = need; PrintUsingMsg(with);}
  if (with == 0) {PrintLine("You'll need to use something."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state]) {PrintLine("It is already unlocked."); return;}
  StateVar[state] = 1;
  PrintLine("It is now unlocked and open.");
  TimePassed = 1;
}


void DoMiscWith_turn_dial(int with)
{
  int need = OBJ_PLIERS, state = STATE_ICEMELTED;

  if (with == 0) {PrintLine("It won't budge."); return;}
  if (with != need) {PrintFutileMsg(with); return;}

  if (StateVar[state]) {PrintLine("The dial won't turn any further."); return;}

  PrintLine("You apparently have turned up a temperature control on the furnace. The pipes connected to the furnace begin to rattle and make noise.");
  StateVar[state] = 1;
  Obj[OBJ_SAPPHIRE].loc = 54;
  TimePassed = 1;
}


void DoMiscWith_turn_globe(int with)
{
  int state = STATE_GLOBEHOLE;

  if (with != 0) {PrintFutileMsg(with); return;}

  TimePassed = 1;

  if (StateVar[state] == 0) {PrintLine("Something rattles from within the globe."); return;}

  if (Obj[OBJ_RUBY].loc != 0) {PrintLine("The globe spins silently."); return;}

  PrintLine("Something falls out of the globe.");
  Obj[OBJ_RUBY].loc = Obj[OBJ_YOU].loc;
}



struct DOMISCWITH_STRUCT DoMiscWith[] =
{
  { A_BREAK  , FOBJ_ROCKWALL    , DoMiscWith_break_rockwall   },
  { A_BREAK  , FOBJ_GLOBE       , DoMiscWith_break_globe      },
  { A_PRY    , FOBJ_FLOORBOARDS , DoMiscWith_pry_floorboards  },
  { A_OIL    , FOBJ_TRAPDOOR    , DoMiscWith_oil_trapdoor     },
  { A_DIG    , FOBJ_CLEARGROUND , DoMiscWith_dig_clearground  },
  { A_DIG    , FOBJ_PITFLOOR    , DoMiscWith_dig_pitfloor     },
  { A_DIG    , FOBJ_SAND        , DoMiscWith_dig_sand         },
  { A_LOCK   , FOBJ_GATE        , DoMiscWith_lock_gate        },
  { A_UNLOCK , FOBJ_GATE        , DoMiscWith_unlock_gate      }, 
  { A_LOCK   , FOBJ_STEELDOOR   , DoMiscWith_lock_steeldoor   },
  { A_UNLOCK , FOBJ_STEELDOOR   , DoMiscWith_unlock_steeldoor }, 
  { A_TURN   , FOBJ_DIAL        , DoMiscWith_turn_dial        },
  { A_TURN   , FOBJ_GLOBE       , DoMiscWith_turn_globe       },

  { 0, 0, 0 }
};
//*****************************************************************************



//*****************************************************************************
void DoMisc_open_gate(void)
{
  if (StateVar[STATE_GATEUNLOCKED]) PrintLine("It's already open.");
  else PrintLine("It's locked.");
}



void DoMisc_open_steeldoor(void)
{
  if (StateVar[STATE_STEELDOOR]) PrintLine("It's already open.");
  else PrintLine("It's locked.");
}



void DoMisc_open_oakdoor(void)
{
  if (StateVar[STATE_OAKDOOR]) PrintLine("It's already open.");
  else PrintLine("It won't budge.");
}



void DoMisc_open_trapdoor(void)
{
  if (StateVar[STATE_TRAPDOOR]) {PrintLine("It's already open."); return;}
  if (StateVar[STATE_TRAPDOOROILED] == 0) {PrintLine("It won't budge."); return;}
  StateVar[STATE_TRAPDOOR] = 1;
  PrintLine("The trapdoor is open.");
  TimePassed = 1;
}



void DoMisc_open_drive(void)
{
  if (Obj[OBJ_DRIVE].prop & PROP_OPEN) PrintLine("It's already open.");
  else PrintLine("It won't budge.");
}



void DoMisc_close_gate(void)
{
  if (StateVar[STATE_GATEUNLOCKED]) PrintLine("It won't stay closed.");
  else PrintLine("It's already closed.");
}



void DoMisc_close_steeldoor(void)
{
  if (StateVar[STATE_STEELDOOR]) PrintLine("It won't stay closed.");
  else PrintLine("It's already closed.");
}



void DoMisc_close_oakdoor(void)
{
  if (StateVar[STATE_OAKDOOR])
  {
    StateVar[STATE_OAKDOOR] = 0;
    PrintLine("The oak door is closed.");
    TimePassed = 1;
  }
  else PrintLine("It's already closed.");
}



void DoMisc_close_trapdoor(void)
{
  if (StateVar[STATE_TRAPDOOR] == 0) {PrintLine("It's already closed."); return;}
  if (StateVar[STATE_TRAPDOOROILED] == 0) {PrintLine("It won't budge."); return;}
  StateVar[STATE_TRAPDOOR] = 0;
  PrintLine("The trapdoor is closed.");
  TimePassed = 1;
}



void DoMisc_activate_switch(void)
{
  int state = STATE_IRONPANEL, flag = StateVar[state];

  if (IsPlayerInDarkness()) {PrintLine("It is too dark to see anything."); return;}
  PrintLine("Click!");
  StateVar[state] = (flag ? 0 : 1);
  TimePassed = 1;
}



void DoMisc_activate_lamp(void)
{
  if (GetLampAndPackTurnsLeft() == 0) PrintLine("It won't turn on!");
  else ActivateObj(OBJ_LAMP);
}



void DoMisc_activate_drive(void)
{
  PrintLine("Please be more explicit about how to do that.");
}



void DoMisc_deactivate_switch(void)
{
  int state = STATE_IRONPANEL, flag = StateVar[state];

  if (IsPlayerInDarkness()) {PrintLine("It is too dark to see anything."); return;}
  PrintLine("Click!");
  StateVar[state] = (flag ? 0 : 1);
  TimePassed = 1;
}



void DoMisc_deactivate_drive(void)
{
  PrintLine("Please be more explicit about how to do that.");
}



void DoMisc_examine_sign(void)
{
  PrintLine("The sign reads \"RETURN ALL BOOTY HERE\".");
}



void DoMisc_examine_woodensign(void)
{
  PrintLine("Welcome to Crystal Caverns.\nPopulation: 1\nEstablished: 1982\nElevation: Flat (It's on a disk, remember?)");
}



void DoMisc_examine_doormsg(void)
{
  PrintLine("The carving reads \"ASK, AND IT SHALL BE GIVEN TO YOU; SEEK, AND YOU SHALL FIND; KNOCK, AND IT SHALL BE OPENED TO YOU\".");
}



void DoMisc_examine_treemsg(void)
{
  PrintLine("The carving reads \"FOLLOW IN MY FOOTPRINTS\".");
}



void DoMisc_examine_wallmsg(void)
{
  PrintLine("HOT OR COLD\nWARM OR COOL\nTHE SAPPHIRE'S FREE\nIF YOU CAN FIND THE TOOL");
}



void DoMisc_examine_trapdoor(void)
{
  PrintLine("The hinges are very rusted.");
}



void DoMisc_examine_computer(void)
{
  PrintLine("The computer is an ISS-1701 make.");
}



void DoMisc_examine_footprints(void)
{
  PrintLine("They appear to have been made by a pair of Pro-Keds.");
}



void DoMisc_examine_icicle(void)
{
  if (StateVar[STATE_ICEMELTED]) {TimePassed = 0; PrintLine("That isn't visible here."); return;}
  PrintLine("There is a sapphire frozen inside the icicle.");
}



void DoMisc_examine_picture(void)
{
  if (StandStoolLoc) PrintLine("The picture is just within your reach.");
  else PrintLine("I hope you're not afraid of heights.");
}



void DoMisc_examine_compass(void)
{
  PrintLine("The needle in the compass is spinning wildly around.");
}



void DoMisc_examine_record(void)
{
  PrintLine("The record is by \"Mophead and the Mello Tones\".");
}



void DoMisc_examine_magazine(void)
{
  PrintLine("This magazine article explains how you can IPL an ISS-1701 system. After system is booted, $$A$SUP2 and ISS must be entered. Then IPL is DPD.");
}



void DoMisc_examine_printout(void)
{
  PrintLine("            MAZE  MAP\n\n    N  S  E  W  NE  NW  SE  SW\n  ------------------------------\n1)  5  E  2  4  X   1   6   X\n2) 13 14  3  1  X   2   X   X\n3) 13 15  4  2  X   3   X   X\n4) 16  8  1  X  X   X   X   X\n5)  1  9  X  X  X   X   X   X\n6)  2  X  1  2 11   X   3   4\n7)  X  X  1  5  2  13  10  12\n8)  4 12  X  X  X   X   X   X\n9)  5 13  X  X  X   X   X   X\n10) 6  X  X  X  X   X   X   X TREASURE\n11) 5  3  1  6  8   7  15  13\n12) 8 16  X  X  X   X   X   X\n13) 1 14 16  X  X   X   X   X\n14) X  2 15 13  X   X   X   X\n15)11  3 16  4  X   X   X   X\n16)12  4 13  X  X   X   E   X\nE=EXIT X=NO DIRECTION");
}



void DoMisc_examine_stool(void)
{
  if (StandStoolLoc) PrintLine("You're standing on it.");
  else PrintLine("You don't see anything unusual.");
}



void DoMisc_examine_parachute(void)
{
  if (WearingParachute) PrintLine("You're wearing it.");
  else PrintLine("You don't see anything unusual.");
}



void DoMisc_push_bluebutton(void)
{
  if (Obj[OBJ_DRIVE].prop & PROP_OPEN)
  {
    Obj[OBJ_DRIVE].prop &= ~PROP_OPEN;
    PrintLine("The cover closes.");
  }
  else
  {
    Obj[OBJ_DRIVE].prop |= PROP_OPEN;
    PrintLine("The cover opens.");
  }

  TimePassed = 1;
}



void DoMisc_push_redbutton(void)
{
  TimePassed = 1;

  if (Obj[OBJ_DRIVE].prop & PROP_LIT)
  {
    Obj[OBJ_DRIVE].prop &= ~PROP_LIT;
    PrintLine("The drive is off.");
  }
  else
  {
    Obj[OBJ_DRIVE].prop |= PROP_LIT;
    PrintLine("The drive is on.");
  }
}



void DoMisc_push_largebutton(void)
{
  TimePassed = 1; //time passed even if input error

  PrintDelayFlag = 1; PrintLine("\n\n\n\n\n     WELCOME TO THE ISS-1701 SYSTEM\n\nOP57A SPECIFY SUPERVISOR NAME:"); PrintDelayFlag = 0;

  GetWords("\n>");
  if (MatchCurWord("$$a$sup2") == 0)
  {
    NumStrWords = 0;
    PrintDelayFlag = 1; PrintLine("\nSUPERVISOR NOT FOUND\n\n\n\n\n"); PrintDelayFlag = 0;
    return;
  }

  PrintDelayFlag = 1; PrintLine("\nOP24A ENTER PASSWORD:"); PrintDelayFlag = 0;

  GetWords("\n>");
  if (MatchCurWord("iss") == 0)
  {
    NumStrWords = 0;
    PrintDelayFlag = 1; PrintLine("\nWRONG PASSWORD\n\n\n\n\n"); PrintDelayFlag = 0;
    return;
  }

  PrintDelayFlag = 1; PrintLine("\nOP23A GIVE IPL CONTROL COMMAND:"); PrintDelayFlag = 0;

  GetWords("\n>");
  if (MatchCurWord("dpd") == 0)
  {
    NumStrWords = 0;
    PrintDelayFlag = 1; PrintLine("\nIPL NOT GIVEN\n\n\n\n\n"); PrintDelayFlag = 0;
    return;
  }

  NumStrWords = 0;

  if (Obj[OBJ_DISK].loc != 2048 + OBJ_DRIVE ||  //disk not in drive
      (Obj[OBJ_DRIVE].prop & PROP_OPEN)     ||  //drive open
      (Obj[OBJ_DRIVE].prop & PROP_LIT) == 0 ||  //drive not turned on
      Obj[OBJ_PRINTOUT].loc != 0                //data already printed
     )
  {
    PrintDelayFlag = 1; PrintLine("\nNO DATA ON DISK\n\n\n\n\n"); PrintDelayFlag = 0;
    return;
  }

  Obj[OBJ_PRINTOUT].loc = 99;

  PrintDelayFlag = 1; PrintLine("\nJOB MAP OUTPUT TO PRINTER 1\n...........................\nPRINTER 1 WAITING FOR WORK\n\n\n\n\n"); PrintDelayFlag = 0;
}



void DoMisc_move_push_statue(void)
{
  int state = STATE_HALLWAYPASSAGE;

  if (StateVar[state]) {PrintLine("It won't move any further."); return;}
  StateVar[state] = 1;
  PrintLine("A loud noise came from somewhere near the stairs.");
  TimePassed = 1;
}



void DoMisc_move_push_picture(void)
{
  int state = STATE_PARLOROPENING;

  if (StandStoolLoc == 0) {PrintLine("You can't reach it."); return;} //not standing on stool
  if (StateVar[state]) {PrintLine("It won't move any further."); return;}
  StateVar[state] = 1;
  PrintLine("A sound came from behind you.");
  TimePassed = 1;
}



void DoMisc_move_push_gboulder(void)
{
  int state = STATE_LARGEBOULDER;

  if (StateVar[state]) {PrintLine("It has already been moved."); return;}
  StateVar[state] = 1;
  PrintLine("With a Samson-like effort, you have managed to move the boulder and sent it rolling across the floor of the quarry.");
  TimePassed = 1;
}



void DoMisc_knock_misc(void)
{
  PrintLine("Knock! Knock!");
  TimePassed = 1;
}



void DoMisc_knock_oakdoor(void)
{
  if (StateVar[STATE_OAKDOOR])
  {
    PrintLine("Knock! Knock!");
    TimePassed = 1;
    return;
  }

  StateVar[STATE_OAKDOOR] = 1;
  PrintLine("The door begins to open slowly.");
  TimePassed = 1;
}



void DoMisc_mount_stool(void)
{
  if (StandStoolLoc) {PrintLine("You're already on it."); return;}
  if (Obj[OBJ_STOOL].loc != Obj[OBJ_YOU].loc) {PrintLine("You need to place it somewhere first."); return;}
  StandStoolLoc = Obj[OBJ_YOU].loc;
  PrintLine("You get on the stool.");
  TimePassed = 1;
}



void DoMisc_dismount_stool(void)
{
  if (StandStoolLoc == 0) {PrintLine("You're not on it."); return;}
  StandStoolLoc = 0;
  PrintLine("You get off the stool.");
  TimePassed = 1;
}



void DoMisc_eat_burger(void)
{
  PrintLine("Thanks! That really hit the spot!");
  Obj[OBJ_BURGER].loc = 0;
  TimePassed = 1;
}



void DoMisc_drink_wine(void)
{
  if ((Obj[OBJ_WINE].prop & PROP_OPEN) == 0) {PrintLine("You need to open it first."); return;}
  PrintLine("Thank you! It was delicious!");
  Obj[OBJ_WINE].loc = 0;
  Obj[OBJ_BOTTLE].loc = 2048 + OBJ_YOU;
  TimePassed = 1;
}



void DoMisc_wear_parachute(void)
{
  if (WearingParachute) {PrintLine("You're already wearing it."); return;}
  PrintLine("You put on the parachute.");
  WearingParachute = 1;
  TimePassed = 1;
}



void DoMisc_remove_parachute(void)
{
  if (WearingParachute == 0) {PrintLine("You aren't wearing it."); return;}
  PrintLine("You take off the parachute.");
  WearingParachute = 0;
  TimePassed = 1;
}



void DoMisc_play_piano(void)
{
  PrintLine("It is very out of tune.");
  TimePassed = 1;
}



void DoMisc_play_record(void)
{
  PrintLine("There's nothing to play it on.");
}



void DoMisc_play_violin(void)
{
  PrintLine("It is very out of tune.");
  TimePassed = 1;
}



void DoMisc_sleepon_bed(void)
{
  PrintLine("I hope you're rested now.");
  TimePassed = 1;
}



void DoMisc_lookthrough_window(void)
{
  PrintLine("You don't see anything unusual.");
  TimePassed = 1;
}



void DoMisc_lookon_shelf(void)
{
  PrintLine("You don't see anything unusual.");
  TimePassed = 1;
}



void DoMisc_lookon_bookshelf(void)
{
  PrintLine("You don't see anything unusual.");
  TimePassed = 1;
}



void DoMisc_lookon_desk(void)
{
  PrintLine("You don't see anything unusual.");
  TimePassed = 1;
}



void DoMisc_lookunder_bed(void)
{
  PrintLine("There's nothing under it.");
  TimePassed = 1;
}



void DoMisc_lookunder_piano(void)
{
  PrintLine("There's nothing under it.");
  TimePassed = 1;
}



void DoMisc_lookunder_desk(void)
{
  PrintLine("There's nothing under it.");
  TimePassed = 1;
}



void DoMisc_jumpover_chasm(void)
{
  PrintLine("It's too wide to jump over.");
}



void DoMisc_jumpover_fissure(void)
{
  PrintLine("It's too wide to jump over.");
}



void DoMisc_jumpinto_chasm(void)
{
  int room = Obj[OBJ_YOU].loc;

  if (room == 18) GoFrom_chasm_down();
  else PrintLine("That would lead to certain doom.");
}



void DoMisc_jumpinto_fissure(void)
{
  int room = Obj[OBJ_YOU].loc;

  if (room == 50 || room == 51 || room == 60) GoFrom_fissure_down();
  else PrintLine("You're at the bottom of the fissure.");
}



struct DOMISC_STRUCT DoMisc[] =
{
  { A_OPEN        , FOBJ_GATE        , DoMisc_open_gate          },
  { A_OPEN        , FOBJ_STEELDOOR   , DoMisc_open_steeldoor     },
  { A_OPEN        , FOBJ_OAKDOOR     , DoMisc_open_oakdoor       },
  { A_OPEN        , FOBJ_TRAPDOOR    , DoMisc_open_trapdoor      },
  { A_OPEN        , OBJ_DRIVE        , DoMisc_open_drive         },
  { A_CLOSE       , FOBJ_GATE        , DoMisc_close_gate         },
  { A_CLOSE       , FOBJ_STEELDOOR   , DoMisc_close_steeldoor    },
  { A_CLOSE       , FOBJ_OAKDOOR     , DoMisc_close_oakdoor      },
  { A_CLOSE       , FOBJ_TRAPDOOR    , DoMisc_close_trapdoor     },
  { A_ACTIVATE    , FOBJ_SWITCH      , DoMisc_activate_switch    },
  { A_ACTIVATE    , OBJ_LAMP         , DoMisc_activate_lamp      },
  { A_ACTIVATE    , OBJ_DRIVE        , DoMisc_activate_drive     },
  { A_DEACTIVATE  , FOBJ_SWITCH      , DoMisc_deactivate_switch  },
  { A_DEACTIVATE  , OBJ_DRIVE        , DoMisc_deactivate_drive   },
  { A_EXAMINE     , FOBJ_SIGN        , DoMisc_examine_sign       },
  { A_EXAMINE     , FOBJ_WOODENSIGN  , DoMisc_examine_woodensign },
  { A_EXAMINE     , FOBJ_DOORMSG     , DoMisc_examine_doormsg    },
  { A_EXAMINE     , FOBJ_TREEMSG     , DoMisc_examine_treemsg    },
  { A_EXAMINE     , FOBJ_WALLMSG     , DoMisc_examine_wallmsg    },
  { A_EXAMINE     , FOBJ_TRAPDOOR    , DoMisc_examine_trapdoor   },
  { A_EXAMINE     , FOBJ_COMPUTER    , DoMisc_examine_computer   },
  { A_EXAMINE     , FOBJ_FOOTPRINTS  , DoMisc_examine_footprints },
  { A_EXAMINE     , FOBJ_ICICLE      , DoMisc_examine_icicle     },
  { A_EXAMINE     , FOBJ_PICTURE     , DoMisc_examine_picture    },
  { A_EXAMINE     , OBJ_COMPASS      , DoMisc_examine_compass    },
  { A_EXAMINE     , OBJ_RECORD       , DoMisc_examine_record     },
  { A_EXAMINE     , OBJ_MAGAZINE     , DoMisc_examine_magazine   },
  { A_EXAMINE     , OBJ_PRINTOUT     , DoMisc_examine_printout   },
  { A_EXAMINE     , OBJ_STOOL        , DoMisc_examine_stool      },
  { A_EXAMINE     , OBJ_PARACHUTE    , DoMisc_examine_parachute  },
  { A_PUSH        , FOBJ_BLUEBUTTON  , DoMisc_push_bluebutton    },
  { A_PUSH        , FOBJ_REDBUTTON   , DoMisc_push_redbutton     },
  { A_PUSH        , FOBJ_LARGEBUTTON , DoMisc_push_largebutton   },
  { A_MOVE        , FOBJ_STATUE      , DoMisc_move_push_statue   },
  { A_PUSH        , FOBJ_STATUE      , DoMisc_move_push_statue   },
  { A_MOVE        , FOBJ_PICTURE     , DoMisc_move_push_picture  },
  { A_PUSH        , FOBJ_PICTURE     , DoMisc_move_push_picture  },
  { A_MOVE        , FOBJ_GBOULDER    , DoMisc_move_push_gboulder },
  { A_PUSH        , FOBJ_GBOULDER    , DoMisc_move_push_gboulder },
  { A_KNOCK       , FOBJ_STEELDOOR   , DoMisc_knock_misc         },
  { A_KNOCK       , FOBJ_PANEL       , DoMisc_knock_misc         },
  { A_KNOCK       , FOBJ_TRAPDOOR    , DoMisc_knock_misc         },
  { A_KNOCK       , FOBJ_FLOORBOARDS , DoMisc_knock_misc         },
  { A_KNOCK       , FOBJ_KNOCKER     , DoMisc_knock_oakdoor      },
  { A_KNOCK       , FOBJ_OAKDOOR     , DoMisc_knock_oakdoor      },
  { A_MOUNT       , OBJ_STOOL        , DoMisc_mount_stool        },
  { A_DISMOUNT    , OBJ_STOOL        , DoMisc_dismount_stool     },
  { A_EAT         , OBJ_BURGER       , DoMisc_eat_burger         },
  { A_DRINK       , OBJ_WINE         , DoMisc_drink_wine         },
  { A_WEAR        , OBJ_PARACHUTE    , DoMisc_wear_parachute     },
  { A_REMOVE      , OBJ_PARACHUTE    , DoMisc_remove_parachute   },
  { A_PLAY        , FOBJ_PIANO       , DoMisc_play_piano         },
  { A_PLAY        , OBJ_RECORD       , DoMisc_play_record        },
  { A_PLAY        , OBJ_VIOLIN       , DoMisc_play_violin        },
  { A_SLEEPON     , FOBJ_BED         , DoMisc_sleepon_bed        },
  { A_LOOKTHROUGH , FOBJ_WINDOW      , DoMisc_lookthrough_window },
  { A_LOOKON      , FOBJ_SHELF       , DoMisc_lookon_shelf       },
  { A_LOOKON      , FOBJ_BOOKSHELF   , DoMisc_lookon_bookshelf   },
  { A_LOOKON      , FOBJ_DESK        , DoMisc_lookon_desk        },
  { A_LOOKUNDER   , FOBJ_BED         , DoMisc_lookunder_bed      },
  { A_LOOKUNDER   , FOBJ_PIANO       , DoMisc_lookunder_piano    },
  { A_LOOKUNDER   , FOBJ_DESK        , DoMisc_lookunder_desk     },
  { A_JUMPOVER    , FOBJ_CHASM       , DoMisc_jumpover_chasm     },
  { A_JUMPOVER    , FOBJ_FISSURE     , DoMisc_jumpover_fissure   },
  { A_JUMPINTO    , FOBJ_CHASM       , DoMisc_jumpinto_chasm     },
  { A_JUMPINTO    , FOBJ_FISSURE     , DoMisc_jumpinto_fissure   },

  { 0, 0, 0 }
};
//*****************************************************************************



//*****************************************************************************
void DoMiscTo_give_flytrap(int obj)
{
  if (obj != OBJ_BURGER) {PrintLine("There's nothing it wants to eat, except maybe you!"); return;}

  Obj[obj].loc = 0;
  Obj[OBJ_STAMP].loc = Obj[OBJ_YOU].loc;
  StateVar[STATE_FLYTRAPAPPEASED] = 1;
  PrintLine("The plant gladly gulps down your burger and slides aside. He burps very loudly and spits up a small object.");
  TimePassed = 1;
}



struct DOMISCTO_STRUCT DoMiscTo[] =
{
  { A_GIVE, FOBJ_FLYTRAP, DoMiscTo_give_flytrap },

  { 0, 0, 0 }
};
//*****************************************************************************



//*****************************************************************************
void DoJump(void)
{
  int room = Obj[OBJ_YOU].loc;

  if (room == 18) GoFrom_chasm_down();
  else if (room == 138) PrintLine("That would lead to certain doom.");
  else if (room == 50 || room == 51 || room == 60) GoFrom_fissure_down();
  else
  {
    PrintLine("Are you enjoying yourself?");
    TimePassed = 1;
  }
}



void DoSleep(void)
{
  int room = Obj[OBJ_YOU].loc;

  if (room == 38)
  {
    PrintLine("(on the bed)");
    PrintLine("I hope you're rested now.");
    TimePassed = 1;
  }
  else PrintLine("There's nothing to sleep on.");
}
//*****************************************************************************



//*****************************************************************************

//execute this after time passes to drain activated lamp
//also drains pack if inside activated lamp

void LampDrainRoutine(void)
{
  int turns;

  if ((Obj[OBJ_LAMP].prop & PROP_LIT) == 0) return; //lamp is off, abort function

  //drain lamp first
  //when lamp is empty, drain pack if it is inside lamp
  if (LampTurnsLeft) LampTurnsLeft--;
  else if (Obj[OBJ_PACK].loc == 2048 + OBJ_LAMP && PackTurnsLeft) PackTurnsLeft--; //drain pack if inside lamp

  turns = GetLampAndPackTurnsLeft(); //combine remaining turns of lamp and pack

  if (IsObjVisible(OBJ_LAMP))
  {
    if (turns == 180) PrintLine("\nYour lamp is getting dim.");
    else if (turns == 40) PrintLine("\nYour lamp is almost out. I think you should find a power pack somewhere!");
    else if (turns == 0) PrintLine("\nYour lamp is off.");
  }

  if (turns == 0) //when no turns left, turn off lamp
  {
    int prev_darkness;

    prev_darkness = IsPlayerInDarkness();
    Obj[OBJ_LAMP].prop &= ~PROP_LIT; //turn lamp off
    if (IsPlayerInDarkness() != prev_darkness && prev_darkness == 0)
    {
      PrintNewLine();
      PrintPlayerRoomDesc(1);
    }
  }
}



void DiskInDriveRoutine(void)
{
  StateVar[STATE_DISKLOADED] = (Obj[OBJ_DISK].loc == 2048 + OBJ_DRIVE) ? 1 : 0;
}



//automatically get off stool if:  stool is not sitting in a room (if it is in container or inventory)
//                                 stool is not in same room as player

void StandStoolRoutine(void)
{
  if (StandStoolLoc && (Obj[OBJ_STOOL].loc >= NUM_ROOMS || StandStoolLoc != Obj[OBJ_YOU].loc))
    StandStoolLoc = 0;
}



void FootbridgeRoutine(void)
{
  if (Obj[OBJ_YOU].loc == 132 && FootbridgeStability)
  {
    FootbridgeStability--;

    if (FootbridgeStability == 1)
      PrintLine("\nThese planks don't sound as if they can hold you much longer!");
    else if (FootbridgeStability == 0)
    {
      PrintLine("\n\n\n\n\nYou fall through the broken planks of the footbridge and land in the middle of the lava river (splash). I don't think you could have swum for it.");
      GameOver = 1;
    }
  }
}



//automatically remove parachute if player not directly holding it
void WearingParachuteRoutine(void)
{
  if (WearingParachute && Obj[OBJ_PARACHUTE].loc != 2048 + OBJ_YOU)
    WearingParachute = 0;
}



//run event routines after each action that set time-passed flag
void RunEventRoutines(void)
{
  LampDrainRoutine();
  DiskInDriveRoutine();
  StandStoolRoutine();
  FootbridgeRoutine();
  WearingParachuteRoutine();
}
//*****************************************************************************



//*****************************************************************************
#define LOOT_ROOM  41



#define NUM_TREASURES  16

unsigned short Treasure[NUM_TREASURES] =
{
  OBJ_CANDELABRA,
  OBJ_BRACELET,
  OBJ_DOUBLOONS,
  OBJ_RUBY,
  OBJ_SAPPHIRE,
  OBJ_TOOTHPICKS,
  OBJ_NECKLACE,
  OBJ_GOBLET,
  OBJ_TUSK,
  OBJ_PAINTING,
  OBJ_WINE,
  OBJ_STAMP,
  OBJ_RING,
  OBJ_RUG,
  OBJ_RECORD,
  OBJ_VIOLIN
};



#define NUM_RANKS  11

char *RankName[NUM_RANKS] =
{
  "Beginner",
  "Amateur",
  "Novice",
  "Explorer",
  "Professional Explorer",
  "Trail Blazer",
  "Subterranean Inhabitant",
  "Guide",
  "Cave Dweller",
  "Adventurer",
  "Crystal Caverns Estate Landlord"
};



int CountLoot(void)
{
  int count = 0, i;

  for (i=0; i<NUM_TREASURES; i++)
    if (Obj[Treasure[i]].loc == LOOT_ROOM) count++;

  //count doubloons if inside chest in lootroom
  if (Obj[OBJ_DOUBLOONS].loc == 2048 + OBJ_CHEST && Obj[OBJ_CHEST].loc == LOOT_ROOM) count++;

  return count;
}



int HasWon(void)
{
  return (CountLoot() == NUM_TREASURES);
}



//some state vars are reversible, reducing score even if player won
int GetScore(void)
{
  int score = 0, i;

  for (i=1; i<NUM_STATE_VARS; i++) //skip null state
    if (StateVar[i]) score += 50;

  score += 50 * CountLoot();

  return score;
}



int GetMaxScore(void)
{
  int score = 0;

  score += 50 * (NUM_STATE_VARS - 1);

  score += 50 * NUM_TREASURES;

  return score;
}



char *GetRankName(void)
{
  int rank = ((NUM_RANKS-1) * GetScore()) / GetMaxScore();

  if (rank < 0) rank = 0;
  if (rank > NUM_RANKS-1) rank = NUM_RANKS-1;
  return RankName[rank];
}
//*****************************************************************************



//*****************************************************************************

#define SAVESTATE  { \
  READWRITE(p, &NumMoves, sizeof(int)); \
  READWRITE(p, &LampTurnsLeft, sizeof(int)); \
  READWRITE(p, &PackTurnsLeft, sizeof(int)); \
  READWRITE(p, &StandStoolLoc, sizeof(int)); \
  READWRITE(p, &FootbridgeStability, sizeof(int)); \
  READWRITE(p, &WearingParachute, sizeof(int)); \
  for (i=0; i<NUM_STATE_VARS; i++) \
    READWRITE(p, &StateVar[i], sizeof(unsigned char)); \
  for (i=0; i<NUM_ROOMS; i++) \
    READWRITE(p, &Room[i].prop, sizeof(unsigned char)); \
  for (i=0; i<NUM_OBJECTS; i++) { \
    READWRITE(p, &Obj[i].loc, sizeof(unsigned short)); \
    READWRITE(p, &Obj[i].order, sizeof(unsigned short)); \
    READWRITE(p, &Obj[i].prop, sizeof(unsigned char)); } }



int GetSaveStateSize(void)
{
  int i, p = 0;

#define READWRITE(p,q,size)  {p += size;}
  SAVESTATE
#undef READWRITE
  return p;
}



void ReadSaveState(char *p)
{
  int i;

#define READWRITE(p,q,size)  {memcpy((p), (q), (size)); p += size;}
  SAVESTATE
#undef READWRITE
}



void WriteSaveState(char *p)
{
  int i;

#define READWRITE(p,q,size)  {memcpy((q), (p), (size)); p += size;}
  SAVESTATE
#undef READWRITE
}



void InitGameState(void)
{
  int i;

  NumMoves = 0;

  LampTurnsLeft = 540;
  PackTurnsLeft = 300;
  StandStoolLoc = 0; //0: player not standing on stool; room: player standing on stool in room
  FootbridgeStability = 3; //number of turns until collapse
  WearingParachute = 0;

  for (i=0; i<NUM_STATE_VARS; i++)
    StateVar[i] = 0;

  for (i=0; i<NUM_ROOMS; i++)
    Room[i].prop &= ~R_DESCRIBED;

  for (i=0; i<NUM_OBJECTS; i++)
  {
    Obj[i].loc = Obj[i].init_loc;
    Obj[i].order = i;
    Obj[i].prop = 0;
  }

  Obj[OBJ_LAMP  ].prop |= PROP_LIGHTABLE | PROP_OPENABLE;
  Obj[OBJ_CHEST ].prop |= PROP_OPENABLE;
  Obj[OBJ_WINE  ].prop |= PROP_OPENABLE;
  Obj[OBJ_BOTTLE].prop |= PROP_OPENABLE | PROP_OPEN;
  Obj[OBJ_DRIVE ].prop |= PROP_NODESC | PROP_NOTTAKEABLE | PROP_LIGHTABLE | PROP_OPENABLE;
}
//*****************************************************************************

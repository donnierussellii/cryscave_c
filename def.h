// Crystal Caverns
// (c) 1982 by Daniel Kitchen
// C port and parser (c) 2021 by Donnie Russell II

// This source code is provided for personal, educational use only.
// You are welcome to use this source code to develop your own works,
// but the story-related content belongs to the original author(s).



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>



#define NUM_ROOMS  139 //including null room 0



//room properties bit flags: maximum of 8

#define R_DARK       1  //dark room
#define R_DESCRIBED  2  //room description already printed



//blocked messages

#define BL1  239  // You can't go that way.
#define BL2  240  // The foliage has become too dense to continue any further.
#define BL3  241  // There is no passage in that direction.
#define BL4  242  // The underbrush is too high to continue.
#define BL5  243  // There's no way over the fence.
#define BL6  244  // There's no way around the chasm.
#define BL7  245  // The wall here is solid sandstone.
#define BL8  246  // If you wish to get off the veranda, I suggest you use the steps.
#define BL9  247  // The door is boarded up.
#define BLA  248  // You can't leave through the window.
#define BLB  249  // Walking into the wall won't help.

#define BLC  250  // The gate is locked.
#define BLD  251  // The oak door is closed.
#define BLE  252  // The trapdoor is closed.
#define BLF  253  // The steel door is locked.
#define BLG  254  // You can't get by the quicksand!
#define BLH  255  // You can't get by the Venus flytrap!



//object bit flags: maximum of 8

#define PROP_OPENABLE      1
#define PROP_OPEN          2
#define PROP_LIGHTABLE     4
#define PROP_LIT           8
#define PROP_NODESC       16
#define PROP_NOTTAKEABLE  32



//game state variables

enum
{
  STATE_NULL,

  STATE_CLEARINGHOLE, //these are state flags used in room passage table
  STATE_GATEUNLOCKED,
  STATE_SHEDFLOOR,
  STATE_OAKDOOR,
  STATE_HALLWAYPASSAGE,
  STATE_PARLOROPENING,
  STATE_TRAPDOOR,
  STATE_BOULDERWALL,
  STATE_ICEMELTED,
  STATE_LARGEBOULDER,
  STATE_FLYTRAPAPPEASED,
  STATE_STEELDOOR,
  STATE_DISKLOADED,
  STATE_IRONPANEL,

  STATE_GLOBEHOLE, //these are not used in table
  STATE_TRAPDOOROILED,

  NUM_STATE_VARS //including null index 0
};



//move directions

enum
{
  DIR_NULL,

  DIR_N,
  DIR_S,
  DIR_E,
  DIR_W,
  DIR_NE,
  DIR_NW,
  DIR_SE,
  DIR_SW,
  DIR_U,
  DIR_D
};



//objects

enum
{
  OBJ_NOTHING,

  OBJ_YOU,

  OBJ_KEY,
  OBJ_OIL,
  OBJ_CROWBAR,
  OBJ_SHOVEL,
  OBJ_LAMP,
  OBJ_CANDELABRA,
  OBJ_BRACELET,
  OBJ_CHEST,
  OBJ_DOUBLOONS,
  OBJ_STOOL,
  OBJ_RUBY,
  OBJ_SAPPHIRE,
  OBJ_TOOTHPICKS,
  OBJ_PICKAXE,
  OBJ_NECKLACE,
  OBJ_GOBLET,
  OBJ_PARACHUTE,
  OBJ_TUSK,
  OBJ_PAINTING,
  OBJ_WINE,
  OBJ_BURGER,
  OBJ_STAMP,
  OBJ_RING,
  OBJ_RUG,
  OBJ_MAGAZINE,
  OBJ_DISK,
  OBJ_COMPASS,
  OBJ_PRINTOUT,
  OBJ_RECORD,
  OBJ_PACK,
  OBJ_WOOD,
  OBJ_VIOLIN,
  OBJ_PLIERS,
  OBJ_BROKENGOBLET,
  OBJ_BOTTLE,
  OBJ_DRIVE,

  NUM_OBJECTS //including null index 0
};



//actions

enum
{
  A_NOTHING,

  A_NORTH, //direction order is important
  A_SOUTH,
  A_EAST,
  A_WEST,
  A_NORTHEAST,
  A_NORTHWEST,
  A_SOUTHEAST,
  A_SOUTHWEST,
  A_UP,
  A_DOWN,

  A_ACTIVATE,
  A_BREAK,
  A_BRIEF,
  A_CLOSE,
  A_DEACTIVATE,
  A_DIG,
  A_DISMOUNT,
  A_DRINK,
  A_DROP,
  A_EAT,
  A_EXAMINE,
  A_GIVE,
  A_GO,
  A_INVENTORY,
  A_JUMP,
  A_JUMPINTO,
  A_JUMPOVER,
  A_KNOCK,
  A_LOCK,
  A_LOOK,
  A_LOOKBEHIND,
  A_LOOKIN,
  A_LOOKON,
  A_LOOKTHROUGH,
  A_LOOKUNDER,
  A_MOUNT,
  A_MOVE,
  A_OIL,
  A_OPEN,
  A_PLAY,
  A_PRY,
  A_PULL,
  A_PUSH,
  A_PUT,
  A_QUIT,
  A_REMOVE,
  A_RESTART,
  A_RESTORE,
  A_SAVE,
  A_SCORE,
  A_SLEEP,
  A_SLEEPON,
  A_SUPERBRIEF,
  A_TAKE,
  A_TURN,
  A_UNLOCK,
  A_VERBOSE,
  A_WAIT,
  A_WEAR
};



//fixed (unmoving) objects

enum
{
  FOBJ_SCENERY_VIS = 2048, //some anonymous scenery object, visible
  FOBJ_SCENERY_NOTVIS,     //                               not visible

  FOBJ_NOTVIS,             //fixed object not visible

  FOBJ_AMB,                //amibiguous (ask for clarification)

  //inter-actable

  FOBJ_DIAL,        //turn to melt icicle in 54
  FOBJ_REDBUTTON,   //push
  FOBJ_BLUEBUTTON,  //push
  FOBJ_LARGEBUTTON, //push
  FOBJ_ICICLE,      //melts when red dial in 110 turned
  FOBJ_SWITCH,      //flip to open iron panel
  FOBJ_STATUE,      //move
  FOBJ_WOODENSIGN,  //read
  FOBJ_SIGN,        //read
  FOBJ_KNOCKER,     //knock to open oak door
  FOBJ_GLOBE,       //break,spin
  FOBJ_PICTURE,     //move while standing on stool
  FOBJ_STEELDOOR,   //open,close; lock,unlock with key
  FOBJ_OAKDOOR,     //open,close,knock
  FOBJ_PANEL,       //open,close,move
  FOBJ_GATE,        //open,close; unlock,lock with key
  FOBJ_DOORMSG,     //read
  FOBJ_TRAPDOOR,    //open,close; oil hinges
  FOBJ_GBOULDER,    //push,move
  FOBJ_SAND,        //dig
  FOBJ_FLYTRAP,     //feed
  FOBJ_CLEARGROUND, //dig to reveal hole
  FOBJ_TREEMSG,     //read
  FOBJ_FLOORBOARDS, //pry
  FOBJ_ROCKWALL,    //pick
  FOBJ_PITFLOOR,    //dig
  FOBJ_WALLMSG,     //read

  //inter-actable but non-essential

  FOBJ_BED,         //sleep on,look under
  FOBJ_PIANO,       //play,look under
  FOBJ_SHELF,       //look on
  FOBJ_BOOKSHELF,   //look on
  FOBJ_DESK,        //look on,look under
  FOBJ_WINDOW,      //look through
  FOBJ_CHASM,       //jump over/into
  FOBJ_FISSURE,     //jump over/into

  //non-inter-actable

  FOBJ_CUSHION,
  FOBJ_NEWEL,
  FOBJ_CARPET,
  FOBJ_FIREPLACE,
  FOBJ_CHANDELIER,
  FOBJ_EXPLORER,
  FOBJ_COMPUTER,
  FOBJ_PRINTER,
  FOBJ_ARTIST,
  FOBJ_DISPLAY,
  FOBJ_GENERATOR,
  FOBJ_CAGE,
  FOBJ_FURNACE,
  FOBJ_FOOTPRINTS
};



struct ROOM_STRUCT
{
  unsigned char prop;
  char *scenery;
};



struct OBJ_STRUCT
{
  unsigned short init_loc;
  unsigned short loc;
  unsigned short order;
  unsigned char prop;
};



struct VERBTOACTION_STRUCT
{
  char *phrase;
  unsigned short action;
};



struct NOUNPHRASETOOBJ_STRUCT
{
  char *phrase;
  unsigned short obj;
};



struct NOUNPHRASETOFIXEDOBJ_STRUCT
{
  char *phrase;
  unsigned short room;
  unsigned short fobj;
};



struct DOMISCWITH_STRUCT
{
  int action;
  int obj; //can be obj or fobj
  void (*f)(int);
};



struct DOMISC_STRUCT
{
  int action;
  int obj; //can be obj or fobj
  void (*f)(void);
};



struct DOMISCTO_STRUCT
{
  int action;
  int to; //can be obj or fobj
  void (*f)(int);
};



struct GOFROM_STRUCT
{
  int room;
  int action; //go-direction action (but technically could be any action)
  int (*f)(void); //function returns 1 if action completed; otherwise fall through
};



//parser.c
void PrintNewLine(void);
void PrintText(char *p);
void PrintLine(char *p);
void PrintCompText(char *comp_text);
void PrintCompLine(char *p);
void PrintObjectDesc(int obj, int desc_flag);
void GetWords(char *prompt);
int IsObjVisible(int obj);
int IsPlayerInDarkness(void);
void PrintPlayerRoomDesc(int force_description);
int MatchCurWord(char *match);
void ActivateObj(int obj);

//game.c
void DoJump(void);
void DoSleep(void);
void RunEventRoutines(void);
int CountLoot(void);
int HasWon(void);
int GetScore(void);
int GetMaxScore(void);
char *GetRankName(void);
int GetSaveStateSize(void);
void ReadSaveState(char *p);
void WriteSaveState(char *p);
void InitGameState(void);

//compression.c
int GetDecompressTextSize(char *text_in, int size_in);
int DecompressText(char *text_in, int size_in, char *text_out);

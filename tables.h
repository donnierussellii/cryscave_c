// Crystal Caverns
// (c) 1982 by Daniel Kitchen
// C port and parser (c) 2021 by Donnie Russell II

// This source code is provided for personal, educational use only.
// You are welcome to use this source code to develop your own works,
// but the story-related content belongs to the original author(s).



extern struct ROOM_STRUCT Room[NUM_ROOMS];
extern char *SceneryNouns;
extern unsigned char RoomPassages[15*NUM_ROOMS];
extern struct OBJ_STRUCT Obj[NUM_OBJECTS];
extern struct VERBTOACTION_STRUCT VerbToAction[];
extern char *DirName[20];
extern struct NOUNPHRASETOOBJ_STRUCT NounPhraseToObj[];
extern struct NOUNPHRASETOFIXEDOBJ_STRUCT NounPhraseToFixedObj[];

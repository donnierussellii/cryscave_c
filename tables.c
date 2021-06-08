// Crystal Caverns
// (c) 1982 by Daniel Kitchen
// C port and parser (c) 2021 by Donnie Russell II

// This source code is provided for personal, educational use only.
// You are welcome to use this source code to develop your own works,
// but the story-related content belongs to the original author(s).



#include "_def.h"



//scenery: single words separated by commas with no whitespace

struct ROOM_STRUCT Room[NUM_ROOMS] =
{
//  prop     scenery

  { 0      , 0                                                                                                     },  // null room

  { 0      , "mansion,house,road"                                                                                  },  //   1
  { 0      , "road"                                                                                                },  //   2
  { 0      , "mansion,house,fence"                                                                                 },  //   3
  { 0      , "road"                                                                                                },  //   4
  { 0      , "mansion,house,forest,woods,trees,clearing"                                                           },  //   5
  { 0      , "path,trail,forest,woods,trees,clearing"                                                              },  //   6
  { 0      , "vines,bushes,foliage,weeds,underbrush,thickets,branches,clearing,stump,hole"                         },  //   7
  { 0      , "path,trail,forest,woods,trees,leaves,vines,bushes,foliage,weeds,underbrush,thickets,branches"        },  //   8
  { 0      , "path,trail,leaves,vines,bushes,foliage,weeds,underbrush,thickets,branches"                           },  //   9
  { 0      , "forest,woods,trees,leaves"                                                                           },  //  10
  { 0      , "mansion,house,fence"                                                                                 },  //  11
  { 0      , "mansion,house,fence"                                                                                 },  //  12
  { 0      , "mansion,house,fence,vines,bushes,foliage,weeds,underbrush,thickets,branches,shed,lawn,yard,backyard" },  //  13
  { 0      , "road,forest,woods,trees,vines,bushes,foliage,weeds,underbrush,thickets,branches"                     },  //  14
  { 0      , "road,vines,bushes,foliage,weeds,underbrush,thickets,branches"                                        },  //  15
  { 0      , "path,trail,vines,bushes,foliage,weeds,underbrush,thickets,branches,clearing"                         },  //  16
  { 0      , "road"                                                                                                },  //  17
  { 0      , "road,caves"                                                                                          },  //  18
  { 0      , "shed,lawn,yard,backyard,trunk,garden"                                                                },  //  19
  { 0      , "shed,hole"                                                                                           },  //  20
  { 0      , "mansion,house,vines,bushes,foliage,weeds,underbrush,thickets,branches,lawn,yard,backyard"            },  //  21
  { 0      , "mansion,house,vines,bushes,foliage,weeds,underbrush,thickets,branches,lawn,yard,backyard"            },  //  22
  { 0      , "mansion,house,stump"                                                                                 },  //  23
  { 0      , "jewels"                                                                                              },  //  24
  { 0      , "mansion,house,shed"                                                                                  },  //  25
  { 0      , "hole"                                                                                                },  //  26
  { 0      , "mansion,house,boards,planks"                                                                         },  //  27
  { 0      , "tunnel,opening,hole"                                                                                 },  //  28
  { 0      , 0                                                                                                     },  //  29
  { 0      , "passage,passageway,steps,stairs,floorboards,bannister"                                               },  //  30
  { 0      , "opening"                                                                                             },  //  31
  { 0      , 0                                                                                                     },  //  32
  { 0      , 0                                                                                                     },  //  33
  { 0      , "steps,stairs"                                                                                        },  //  34
  { 0      , "passage,passageway"                                                                                  },  //  35
  { 0      , "steps,stairs"                                                                                        },  //  36
  { 0      , 0                                                                                                     },  //  37
  { 0      , 0                                                                                                     },  //  38
  { 0      , "ladder,bookcases"                                                                                    },  //  39
  { 0      , "ladder"                                                                                              },  //  40
  { 0      , 0                                                                                                     },  //  41
  { 0      , "passage,passageway,hole,ladder"                                                                      },  //  42
  { R_DARK , 0                                                                                                     },  //  43
  { 0      , "tunnel"                                                                                              },  //  44
  { 0      , "passage,passageway,crawlway"                                                                         },  //  45
  { R_DARK , "passage,passageway,crawlway,walkway,floor"                                                           },  //  46
  { R_DARK , "passage,passageway,hole,walkway,quicksand,pool,sand"                                                 },  //  47
  { R_DARK , "tunnel,stalactites,stalagmites,shape"                                                                },  //  48
  { R_DARK , "steps,stairs,stalactites,stalagmites"                                                                },  //  49
  { R_DARK , "passage,passageway,steps,stairs"                                                                     },  //  50
  { R_DARK , "passage,passageway,tunnel"                                                                           },  //  51
  { R_DARK , "corridor,boulders,rocks,stones,hole"                                                                 },  //  52
  { R_DARK , "passage,passageway"                                                                                  },  //  53
  { R_DARK , "passage,passageway,icicles"                                                                          },  //  54
  { R_DARK , 0                                                                                                     },  //  55
  { R_DARK , "corridor,opening"                                                                                    },  //  56
  { R_DARK , "corridor"                                                                                            },  //  57
  { R_DARK , "passage,passageway,hole"                                                                             },  //  58
  { R_DARK , "passage,passageway"                                                                                  },  //  59
  { R_DARK , "path,trail"                                                                                          },  //  60
  { R_DARK , "passage,passageway"                                                                                  },  //  61
  { R_DARK , "path,trail,boulders,rocks,stones"                                                                    },  //  62
  { R_DARK , "path,trail,boulders,rocks,stones"                                                                    },  //  63
  { R_DARK , "path,trail,boulders,rocks,stones,hole"                                                               },  //  64
  { R_DARK , "opening"                                                                                             },  //  65
  { R_DARK , 0                                                                                                     },  //  66
  { R_DARK , "passage,passageway,path,trail,lava,river"                                                            },  //  67
  { R_DARK , "tunnel"                                                                                              },  //  68
  { R_DARK , "passage,passageway"                                                                                  },  //  69
  { R_DARK , "passage,passageway,hole"                                                                             },  //  70
  { R_DARK , "passage,passageway,stalactites,stalagmites"                                                          },  //  71
  { R_DARK , "passage,passageway,stalactites,stalagmites,pit,ribs,bones"                                           },  //  72
  { R_DARK , "passage,passageway,steps,stairs,boulders,rocks,stones,pit"                                           },  //  73
  { R_DARK , "steps,stairs,pit"                                                                                    },  //  74
  { R_DARK , "passage,passageway"                                                                                  },  //  75
  { R_DARK , "cubbyhole,pictures"                                                                                  },  //  76
  { R_DARK , "cubbyhole"                                                                                           },  //  77
  { R_DARK , "path,trail,lava,river"                                                                               },  //  78
  { R_DARK , "path,trail,lava,river,crack"                                                                         },  //  79
  { R_DARK , "tunnel,lava,river,crack"                                                                             },  //  80
  { R_DARK , "passage,passageway"                                                                                  },  //  81
  { R_DARK , "passage,passageway"                                                                                  },  //  82
  { R_DARK , "passage,passageway,crawlway,roots"                                                                   },  //  83
  { R_DARK , "passage,passageway,crawlway,plants"                                                                  },  //  84
  { R_DARK , "walkway"                                                                                             },  //  85
  { R_DARK , "passage,passageway"                                                                                  },  //  86
  { 0      , 0                                                                                                     },  //  87
  { 0      , 0                                                                                                     },  //  88
  { R_DARK , "passage,passageway,markings,tapestries"                                                              },  //  89
  { R_DARK , "passage,passageway"                                                                                  },  //  90
  { R_DARK , "passage,passageway"                                                                                  },  //  91
  { R_DARK , "walkway,skins,beartraps"                                                                             },  //  92
  { R_DARK , "passage,passageway"                                                                                  },  //  93
  { R_DARK , "passage,passageway"                                                                                  },  //  94
  { R_DARK , "tunnel"                                                                                              },  //  95
  { R_DARK , "tunnel,opening"                                                                                      },  //  96
  { R_DARK , "passage,passageway,ramp"                                                                             },  //  97
  { 0      , "display,lights"                                                                                      },  //  98
  { 0      , "cables"                                                                                              },  //  99
  { R_DARK , "passage,passageway,quicksand,pool,sand"                                                              },  // 100
  { R_DARK , 0                                                                                                     },  // 101
  { R_DARK , 0                                                                                                     },  // 102
  { R_DARK , "passage,passageway"                                                                                  },  // 103
  { R_DARK , "path,trail"                                                                                          },  // 104
  { R_DARK , "corridor"                                                                                            },  // 105
  { R_DARK , "passage,passageway"                                                                                  },  // 106
  { R_DARK , "passage,passageway"                                                                                  },  // 107
  { R_DARK , "passage,passageway"                                                                                  },  // 108
  { R_DARK , "passage,passageway"                                                                                  },  // 109
  { R_DARK , "passage,passageway,pipes"                                                                            },  // 110
  { R_DARK , "passage,passageway"                                                                                  },  // 111
  { R_DARK , "passage,passageway"                                                                                  },  // 112
  { R_DARK , "passage,passageway"                                                                                  },  // 113
  { R_DARK , "passage,passageway"                                                                                  },  // 114
  { R_DARK , "passage,passageway"                                                                                  },  // 115
  { R_DARK , "passage,passageway"                                                                                  },  // 116
  { R_DARK , "passage,passageway"                                                                                  },  // 117
  { R_DARK , "passage,passageway"                                                                                  },  // 118
  { R_DARK , "passage,passageway"                                                                                  },  // 119
  { R_DARK , "passage,passageway"                                                                                  },  // 120
  { R_DARK , "passage,passageway"                                                                                  },  // 121
  { R_DARK , "passage,passageway"                                                                                  },  // 122
  { R_DARK , "passage,passageway"                                                                                  },  // 123
  { R_DARK , "passage,passageway"                                                                                  },  // 124
  { R_DARK , "passage,passageway"                                                                                  },  // 125
  { R_DARK , "passage,passageway"                                                                                  },  // 126
  { R_DARK , 0                                                                                                     },  // 127
  { R_DARK , "passage,passageway,lava,river,bridge,footbridge"                                                     },  // 128
  { 0      , "path,trail,forest,woods,trees"                                                                       },  // 129
  { R_DARK , "path,trail,lava,river,crack,bridge,footbridge,abyss"                                                 },  // 130
  { R_DARK , "passage,passageway"                                                                                  },  // 131
  { R_DARK , "boards,planks,bridge,footbridge"                                                                     },  // 132
  { R_DARK , "passage,passageway"                                                                                  },  // 133
  { R_DARK , "tunnel"                                                                                              },  // 134
  { R_DARK , "paper"                                                                                               },  // 135
  { R_DARK , "passage,passageway,scales"                                                                           },  // 136
  { 0      , "path,trail,forest,woods,trees"                                                                       },  // 137
  { 0      , "path,trail,boulders,rocks,stones"                                                                    }   // 138
};



//collection of scenery nouns from all rooms
//single words separated by commas with no whitespace
//alphabetical order is not necessary

char *SceneryNouns =
  "abyss,backyard,bannister,beartraps,boards,bones,bookcases,boulders,branches,"
  "bridge,bushes,cables,caves,clearing,corridor,crack,crawlway,cubbyhole,"
  "display,fence,floor,floorboards,foliage,footbridge,forest,garden,"
  "hole,house,icicles,jewels,ladder,lava,lawn,leaves,lights,mansion,markings,opening,"
  "paper,passage,passageway,path,pictures,pipes,pit,planks,plants,pool,quicksand,"
  "ramp,ribs,river,road,rocks,roots,sand,scales,shape,shed,skins,stairs,stalactites,"
  "stalagmites,steps,stones,stump,tapestries,thickets,trail,trees,trunk,tunnel,"
  "underbrush,vines,walkway,weeds,woods,yard"
;



unsigned char RoomPassages[15*NUM_ROOMS] =
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //null room

//  N    S    E    W   NE   NW   SE   SW    U    D               State Var       Blocked Dirs & Msgs        Room #
//---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ----------------------    ----------------------        ------

   14,   2,   4,  11,  10,  12, BL2,   3, BL1, BL1,                      0,       0,   0,     0,   0,     //  1
    1, BL2, BL2, BL3, BL2,   3, BL2, BL2, BL1, BL1,                      0,       0,   0,     0,   0,     //  2
  BL5, BL4, BL1, BL1,   1,  13,   2, BL4, BL1, BL1,                      0,       0,   0,     0,   0,     //  3
   14,   2,   5,   1,  10,  12, BL2,   3, BL1, BL1,                      0,       0,   0,     0,   0,     //  4
   10, BL3,   6,   4,  10,  14, BL2, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //  5
  BL3, BL4,   7,   5, BL3,  10, BL4, BL2, BL1, BL1,                      0,       0,   0,     0,   0,     //  6
  BL2, BL1, BL2,   6,   8, BL2, BL2, BL4, BL1,  28,     STATE_CLEARINGHOLE,   DIR_D, BL1,     0,   0,     //  7
  BL3, BL2,   9,   7, BL3, BL2, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //  8
  BL2, BL3, BL2,   8, BL1, BL1, BL3, BL2, BL1, BL1,                      0,       0,   0,     0,   0,     //  9
  BL2,   5, BL2,  14, BL2,  14,   6,   4, BL1, BL1,                      0,       0,   0,     0,   0,     // 10
   14,   2,   1,  21,  14,  12,   2,   3, BL1, BL1,     STATE_GATEUNLOCKED,   DIR_W, BLC,     0,   0,     // 11
   16, BL5, BL3, BL1,  14, BL2,   1,  13, BL1, BL1,                      0,       0,   0,     0,   0,     // 12
  BL1, BL1, BL5, BL2,  12, BL2,   3, BL2, BL1, BL1,                      0,       0,   0,     0,   0,     // 13
   15,   1, BL3, BL2, BL3, BL2,  10,  12, BL1, BL1,                      0,       0,   0,     0,   0,     // 14
   17,  14, BL2,  16, BL3, BL2, BL3, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 15
  BL3,  12,  15, BL2,  15, BL2, BL2, BL2, BL1, BL1,                      0,       0,   0,     0,   0,     // 16
   18,  15, BL2, BL2, BL1, BL2, BL1, BL2, BL1, BL1,                      0,       0,   0,     0,   0,     // 17
  BL6,  17, BL1, BL2, BL6, BL6, BL1, BL1, BL1, BL6,                      0,       0,   0,     0,   0,     // 18
   20,  25,  27, BL5,  22, BL5,  25, BL5, BL1, BL1,                      0,       0,   0,     0,   0,     // 19
  BL1,  19, BL1, BL1, BL1, BL1, BL1, BL1, BL1,  26,        STATE_SHEDFLOOR,   DIR_D, BL1,     0,   0,     // 20
  BL3, BL3,  11,  24, BL5,  22, BL5,  23, BL1, BL1,     STATE_GATEUNLOCKED,   DIR_E, BLC,     0,   0,     // 21
  BL5, BL1, BL1, BL1, BL5, BL5,  21,  25, BL1, BL1,                      0,       0,   0,     0,   0,     // 22
  BL1, BL5, BL1, BL1,  21,  25, BL5, BL5, BL1, BL1,                      0,       0,   0,     0,   0,     // 23
  BL8, BL8,  21,  29,  21, BL8,  21, BL8, BL1, BL1,          STATE_OAKDOOR,   DIR_W, BLD,     0,   0,     // 24
  BL5, BL5,  27, BL5,  22,  19,  23, BL5, BL1, BL1,                      0,       0,   0,     0,   0,     // 25
  130, BL1, 131, BL7, BL1, BL1, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 26
  BL5, BL5, BL9,  25,  22,  19,  23,  25, BL1, BL1,                      0,       0,   0,     0,   0,     // 27
   44, BL7, 101, BL7, BL7, BL7, BL1, BL1,   7, BL1,                      0,       0,   0,     0,   0,     // 28
  BLB, BLB,  24,  30, BL1, BL1, BLB, BL1, BL1, BL1,          STATE_OAKDOOR,   DIR_E, BLD,     0,   0,     // 29
   34,  31,  29,  33,  35, BL1, BLB, BLB,  34, BL1,   STATE_HALLWAYPASSAGE,  DIR_NE, BL1,     0,   0,     // 30
   30, BLB,  42,  32, BLB, BLB, BL1, BL1, BL1, BL1,    STATE_PARLOROPENING,   DIR_E, BL1,     0,   0,     // 31
   33, BLB,  31, BLB, BLB, BLB, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 32
  BLB,  32,  30, BLB, BL1, BL1, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 33
  BLB,  30, BLB,  36, BLB, BLB, BL1, BL1,  36,  30,                      0,       0,   0,     0,   0,     // 34
  BLB,  30, BLB, BLB, BLB, BLB, BLB, BLB, BLB, BLB,                      0,       0,   0,     0,   0,     // 35
  BLB,  38,  37, BLA, BLB, BLB, BL1, BL1, BL1,  34,                      0,       0,   0,     0,   0,     // 36
  BLB,  39, BLA,  36, BLB, BLB, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 37
   36, BLB, BLB, BLB, BLB, BLB, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 38
   37, BLB, BLB, BLB, BLB, BLB, BLB, BLB,  40, BL1,                      0,       0,   0,     0,   0,     // 39
  BL1, BL1, BL1, BL1, BL1, BL1, BL1, BL1,  41,  39,         STATE_TRAPDOOR,   DIR_U, BLE,     0,   0,     // 40
  BLA, BLB, BLB, BLB, BLB, BLB, BL1, BL1, BL1,  40,         STATE_TRAPDOOR,   DIR_D, BLE,     0,   0,     // 41
  BLB, BLB, BLB,  31, BLB, BL1, BL1, BL1, BL1,  43,                      0,       0,   0,     0,   0,     // 42
  BLB, BLB, BLB, BLB, BLB, BLB, BLB, BLB,  42, BL1,                      0,       0,   0,     0,   0,     // 43
   45,  28, BL7, BL7, BL7, BL1, BL7, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 44
   46,  44, BL7, BL7, BL7, BL1, BL7, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 45
   47,  45,  58,  53, BL7, BL7, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     // 46
  100,  46, BL7,  52, BL7, BL7, BL1, BL1, BL1, BL1,      STATE_BOULDERWALL,   DIR_W, BL1, DIR_N, BLG,     // 47
  BL7, 100, BL7,  96,  49, BL7, BL1, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     // 48
  BL7,  48, BL7, BL7, BL7, BL7, BL1,  48, BL1,  50,                      0,       0,   0,     0,   0,     // 49
  BL7, BL1,  51, BL1, BL1, BL7, BL1, BL1,  49, BL1,                      0,       0,   0,     0,   0,     // 50
   68, BL1,  67,  50, BL1, BL1, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 51
  BL7, BL1,  47,  57, BL1, BL1, BL7, BL7, BL1, BL1,      STATE_BOULDERWALL,   DIR_E, BL1,     0,   0,     // 52
  BL7, BL7,  46,  54, BL1, BL1, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     // 53
   56, BL1,  53,  55, BL1, BLB, BLB, BLB, BL1, BL1,        STATE_ICEMELTED,   DIR_N, BL1,     0,   0,     // 54
   89, BL7,  54, BL7, BL7, BL1, BL7, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 55
  BL7,  54,  57, BL7, BL7, BL1, BL1, BL7, BL1, BL1,        STATE_ICEMELTED,   DIR_S, BL1,     0,   0,     // 56
  BL7,  56,  52, BL7, BL7, BL7, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 57
  BL7, BL7,  59,  46, BL7, BL7, BL1, BL7, BL1,  75,                      0,       0,   0,     0,   0,     // 58
   60, BL7,  61,  58, BL7, BL7, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     // 59
  BL1,  59, BL7, BL7, BL7, BL7, BL7, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 60
  BL7, BL7,  62,  59, BL1, BL1, BL1, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     // 61
  BL7, BL1,  63,  61, BL7, BL7, BL1, BL1, BL1,  63,                      0,       0,   0,     0,   0,     // 62
  BL1, BL1, BL1,  62, BL1, BL1,  64, BL1,  62, BL1,                      0,       0,   0,     0,   0,     // 63
  BL1, BL1, BL1, BL1, BL7,  63, BL1, BL1, BL1,  65,     STATE_LARGEBOULDER,   DIR_D, BL1,     0,   0,     // 64
  BL7, BL7, BL7, BL7, BL1, BL1, BL1,  66,  64, BL1,                      0,       0,   0,     0,   0,     // 65
  BL7, BL7, BL7, BL7,  65, BL1, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 66
  BL7, BL1, BL1,  51, BL7, BL7, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 67
  BL7,  51, BL7, BL7, BL7, BL3, BL7, BL1, BL1,  69,                      0,       0,   0,     0,   0,     // 68
  BL7, BL7, BL1, BL1, BL1, BL7, BL7, BL3,  68,  70,                      0,       0,   0,     0,   0,     // 69
   71, BL7, BL1, BL3, BL7, BL7, BL1, BL1,  69, BL1,                      0,       0,   0,     0,   0,     // 70
   72,  70, BL1, BL3, BL1, BL1, BL3, BL1, BL1, BL3,                      0,       0,   0,     0,   0,     // 71
  BL1,  71, BL3, BL3, BL1,  73, BL1, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     // 72
  BL1,  72, BL1, 133, BL1, BL3,  72, BL3, BL1,  74,                      0,       0,   0,     0,   0,     // 73
  BL7, BL7, BL1, BL3, BL3, BL7, BL7, BL1,  73, BL3,                      0,       0,   0,     0,   0,     // 74
  BL1, 103,  75,  83, BL7, 106, BL3, BL7,  58,  76,                      0,       0,   0,     0,   0,     // 75
  BL7, BL7, BL1, BL3, BL3, BL1,  77, BL1,  75, BL1,                      0,       0,   0,     0,   0,     // 76
  BL1, BL1, BL1, BL3, BL7,  76, BL7, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 77
  BL1, BL7,  79,  80, BL1, BL1, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     // 78
  BL3, BL1, BL1,  78, BL1, BL1, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     // 79
  BL1,  81,  79, BL1, BL1, BL1, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     // 80
   80,  82, BL7, BL7, BL1, BL1, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     // 81
  BL7, BL7, BL7, BL3, BL1, BL1, BL7, BL3,  75,  81,                      0,       0,   0,     0,   0,     // 82
  BL7, BL7,  75,  84, BL1, BL1, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     // 83
  BL1, BL1,  83,  85, BL2, BL2, BL2, BL1, BL1, BL1,  STATE_FLYTRAPAPPEASED,   DIR_W, BLH,     0,   0,     // 84
  BL7,  86,  84, BL3, BL7, BL1, BL7,  86, BL1, BL1,                      0,       0,   0,     0,   0,     // 85
  BL7,  97, BL1, BL3,  85, BL1, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     // 86
   97,  98,  88,  99, BL3, BL3, BL1, BL1, BL1, BL1,        STATE_STEELDOOR,   DIR_N, BLF,     0,   0,     // 87
  BL7, BL1, BL7,  87, BL7,  98, BL3,  87, BL1, BL1,       STATE_DISKLOADED,       0,   0,     0,   0,     // 88
  BL7,  55, BL1, BL3,  90, BL1, BL3, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 89
   91,  89, BL7, BL1, BL1, BL1, BL1,  89, BL1, BL1,                      0,       0,   0,     0,   0,     // 90
  BL7,  90, BL7, BL7,  92,  93, BL1, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     // 91
  BL7, BL3, BL1, BL1,  95, BL1, BL3,  91, BL1, BL1,                      0,       0,   0,     0,   0,     // 92
  BL7, BL7, BL7, BL1, BL3,  94,  91, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 93
  BL1, BL3, BL7, BL1, BL3, BL3,  93, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 94
  BL7, BL7, BL1, BL1,  96, BL1, BL3,  92, BL1, BL1,                      0,       0,   0,     0,   0,     // 95
  134, BL1,  48, BL7, BL1, BL3, BL1,  95, BL1, BL1,                      0,       0,   0,     0,   0,     // 96
   86,  87, BL7, BL7, BL1, BL1, BL3, BL3, BL1, BL1,        STATE_STEELDOOR,   DIR_S, BLF,     0,   0,     // 97
   87, BL1, BL1, BL1, BL1, BL3,  88,  99, BL1, BL1,                      0,       0,   0,     0,   0,     // 98
  BL7, BL7,  87, BL1,  98, BL1,  87, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     // 99
   48,  47, BL7, BL7, BL1, BL1, BL3, BL3, BL1, BL1,      STATE_BOULDERWALL,   DIR_S, BLG,     0,   0,     //100
  BL7, BL7, BL1,  28, BL7, BL3, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //101
  BL7, BL1, 109, BL3, BL1, BL7, BL3, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //102
   75, BL7, BL1, 104, BL1, BL7, BL1, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     //103
  BL1, BL7, 103, 105, BL1, BL3, BL3, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     //104
  BL7, BL1, 104, BL1, BL3, 110, BL3, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //105
  BL7, BL1, BL3, 107, BL3, BL3,  75, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //106
  108, BL7, 106, 109, BL7, BL3, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //107
  BL7, 107, BL3, BL3, BL1, BL1, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //108
  BL7, BL7, 107, 102, BL7, BL3, BL7, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //109
  BL7, BL1, BL3, BL3, BL3, BL1, 105, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //110
  115, 128, 112, 114, BL7, 111, 116, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     //111
  123, 124, 113, 111, BL7, 112, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     //112
  123, 125, 114, 112, BL7, 113, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     //113
  126, 118, 111, 113, BL7, BL7, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     //114
  111, 119, BL7, BL7, BL7, BL3, BL1, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //115
  112, BL7, 111, 112, 121, BL7, 113, 114, BL1, BL1,                      0,       0,   0,     0,   0,     //116
  BL7, BL3, 111, 115, 112, 123, 120, 122, BL1, BL1,                      0,       0,   0,     0,   0,     //117
  114, 122, BL7, BL3, BL1, BL3, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     //118
  115, 123, BL7, BL7, BL3, BL3, BL1, BL1, 111, 119,                      0,       0,   0,     0,   0,     //119
  116, BL7, BL7, BL7, BL7, BL7, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //120
  115, 113, 111, 116, 118, 117, 125, 123, BL1, BL1,                      0,       0,   0,     0,   0,     //121
  118, 126, BL1, BL1, BL1, BL1, BL7, BL7, 122, 122,                      0,       0,   0,     0,   0,     //122
  123, 111, 124, 126, BL7, BL7, BL7, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //123
  BL7, 112, 125, 123, BL1, BL1, BL1, BL7, BL1, 124,                      0,       0,   0,     0,   0,     //124
  121, 113, 126, 124, BL7, BL7, BL1, BL1, BL1, 121,                      0,       0,   0,     0,   0,     //125
  122, 114, 123, BL7, BL7, 126, 127, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //126
  BL7, BL1, BL1, BL3, BL3, 126, BL7, BL7, BL1, BL1,                      0,       0,   0,     0,   0,     //127
  111, 132, BL3, BL3, BL1, BL1, BL7, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //128
  BL2,  10, BL2, BL3, 137, BL1, BL2, BL2, BL1, BL1,                      0,       0,   0,     0,   0,     //129
  132,  26, BL3, BL3, BL1, BL1, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //130
  BL7, BL1, 136,  26, BL7, BL7, BL1, BL3, BL1, BL1,        STATE_IRONPANEL,   DIR_E, BL1,     0,   0,     //131
  128, 130, BL1, BL1, BL1, BL3, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //132
  BL7, BL7,  73, BL1, BL3, BL3, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //133
  BL7,  96, BL1, BL1, BL7, 135, BL3, BL3, BL1, BL1,                      0,       0,   0,     0,   0,     //134
  BL7, BL1, BL3, BL7, BL7, 136, 134, BL1, BL1, BL1,                      0,       0,   0,     0,   0,     //135
  BL7, BL7, BL1, BL1, BL1, 131, 135, BL7, BL1, BL1,        STATE_IRONPANEL,  DIR_NW, BL1,     0,   0,     //136
  BL2, BL2, BL3, BL1, 138, BL2, BL3, 129, BL1, BL1,                      0,       0,   0,     0,   0,     //137
  BL6, BL2, BL1,  18, BL6, BL6, BL2, 137, BL1, BL1,                      0,       0,   0,     0,   0      //138
};



//loc is room except when +2048, which indicates inside obj

struct OBJ_STRUCT Obj[NUM_OBJECTS] =
{
  {   0,              0, 0, 0 },
  {   1,              0, 0, 0 },
  {   9,              0, 0, 0 },
  {  20,              0, 0, 0 },
  {  49,              0, 0, 0 },
  {  20,              0, 0, 0 },
  {  41,              0, 0, 0 },
  {  43,              0, 0, 0 },
  {  35,              0, 0, 0 },
  { 101,              0, 0, 0 },
  { 2048 + OBJ_CHEST, 0, 0, 0 },
  {  35,              0, 0, 0 },
  {   0,              0, 0, 0 },
  {   0,              0, 0, 0 },
  {  59,              0, 0, 0 },
  {  64,              0, 0, 0 },
  {  66,              0, 0, 0 },
  {  52,              0, 0, 0 },
  {  10,              0, 0, 0 },
  {   0,              0, 0, 0 },
  {  77,              0, 0, 0 },
  {  80,              0, 0, 0 },
  {  33,              0, 0, 0 },
  {   0,              0, 0, 0 },
  {  89,              0, 0, 0 },
  {  92,              0, 0, 0 },
  {   0,              0, 0, 0 },
  {  16,              0, 0, 0 },
  { 106,              0, 0, 0 },
  {   0,              0, 0, 0 },
  { 135,              0, 0, 0 },
  { 127,              0, 0, 0 },
  {   0,              0, 0, 0 },
  { 120,              0, 0, 0 },
  {  23,              0, 0, 0 },
  {   0,              0, 0, 0 },
  {   0,              0, 0, 0 },
  {  88,              0, 0, 0 }
};



//phrase must contain a maximum of two words

//for matching to work, verb with adverb must come before same verb with no adverb

//words must be separated by exactly one space

//verbs don't need to (but should) be alphabetized

struct VERBTOACTION_STRUCT VerbToAction[] =
{
  { "climb on"     , A_MOUNT       }, //two words (verb-adverb)
  { "dig in"       , A_DIG         },
  { "get off"      , A_DISMOUNT    },
  { "get on"       , A_MOUNT       },
  { "jump across"  , A_JUMPOVER    },
  { "jump into"    , A_JUMPINTO    },
  { "jump over"    , A_JUMPOVER    },
  { "knock on"     , A_KNOCK       },
  { "knock with"   , A_KNOCK       },
  { "l at"         , A_EXAMINE     },
  { "l behind"     , A_LOOKBEHIND  },
  { "l in"         , A_LOOKIN      },
  { "l inside"     , A_LOOKIN      },
  { "l into"       , A_LOOKIN      },
  { "l on"         , A_LOOKON      },
  { "l through"    , A_LOOKTHROUGH },
  { "l under"      , A_LOOKUNDER   },
  { "lay on"       , A_SLEEPON     },
  { "look at"      , A_EXAMINE     },
  { "look behind"  , A_LOOKBEHIND  },
  { "look in"      , A_LOOKIN      },
  { "look inside"  , A_LOOKIN      },
  { "look into"    , A_LOOKIN      },
  { "look on"      , A_LOOKON      },
  { "look through" , A_LOOKTHROUGH },
  { "look under"   , A_LOOKUNDER   },
  { "pick up"      , A_TAKE        },
  { "pry up"       , A_PRY         },
  { "put down"     , A_DROP        },
  { "put on"       , A_WEAR        },
  { "put out"      , A_DEACTIVATE  },
  { "sleep on"     , A_SLEEPON     },
  { "stand on"     , A_MOUNT       },
  { "take off"     , A_REMOVE      },
  { "turn off"     , A_DEACTIVATE  },
  { "turn on"      , A_ACTIVATE    },

  { "acquire"      , A_TAKE        }, //one word
  { "activate"     , A_ACTIVATE    },
  { "advance"      , A_GO          },
  { "break"        , A_BREAK       },
  { "brief"        , A_BRIEF       },
  { "close"        , A_CLOSE       },
  { "climb"        , A_GO          },
  { "d"            , A_DOWN        },
  { "deactivate"   , A_DEACTIVATE  },
  { "dig"          , A_DIG         },
  { "discard"      , A_DROP        },
  { "dislodge"     , A_MOVE        },
  { "down"         , A_DOWN        },
  { "drink"        , A_DRINK       },
  { "drop"         , A_DROP        },
  { "e"            , A_EAST        },
  { "east"         , A_EAST        },
  { "eat"          , A_EAT         },
  { "examine"      , A_EXAMINE     },
  { "excavate"     , A_DIG         },
  { "feed"         , A_GIVE        },
  { "flick"        , A_ACTIVATE    },
  { "flip"         , A_ACTIVATE    },
  { "get"          , A_TAKE        },
  { "give"         , A_GIVE        },
  { "go"           , A_GO          },
  { "grab"         , A_TAKE        },
  { "head"         , A_GO          },
  { "hit"          , A_BREAK       },
  { "i"            , A_INVENTORY   },
  { "inspect"      , A_EXAMINE     },
  { "inventory"    , A_INVENTORY   },
  { "journey"      , A_GO          },
  { "jump"         , A_JUMP        },
  { "knock"        , A_KNOCK       },
  { "l"            , A_LOOK        },
  { "leap"         , A_JUMP        },
  { "leave"        , A_DROP        },
  { "light"        , A_ACTIVATE    },
  { "lock"         , A_LOCK        },
  { "look"         , A_LOOK        },
  { "loosen"       , A_MOVE        },
  { "lubricate"    , A_OIL         },
  { "mount"        , A_MOUNT       },
  { "move"         , A_MOVE        },
  { "n"            , A_NORTH       },
  { "ne"           , A_NORTHEAST   },
  { "north"        , A_NORTH       },
  { "northeast"    , A_NORTHEAST   },
  { "northwest"    , A_NORTHWEST   },
  { "nw"           , A_NORTHWEST   },
  { "obtain"       , A_TAKE        },
  { "offer"        , A_GIVE        },
  { "oil"          , A_OIL         },
  { "open"         , A_OPEN        },
  { "pick"         , A_BREAK       },
  { "play"         , A_PLAY        },
  { "press"        , A_PUSH        },
  { "proceed"      , A_GO          },
  { "pry"          , A_PRY         },
  { "pull"         , A_PULL        },
  { "push"         , A_PUSH        },
  { "put"          , A_PUT         },
  { "q"            , A_QUIT        },
  { "quit"         , A_QUIT        },
  { "read"         , A_EXAMINE     },
  { "remove"       , A_REMOVE      },
  { "restart"      , A_RESTART     },
  { "restore"      , A_RESTORE     },
  { "rotate"       , A_TURN        },
  { "run"          , A_GO          },
  { "s"            , A_SOUTH       },
  { "save"         , A_SAVE        },
  { "score"        , A_SCORE       },
  { "se"           , A_SOUTHEAST   },
  { "secure"       , A_TAKE        },
  { "shut"         , A_CLOSE       },
  { "sleep"        , A_SLEEP       },
  { "south"        , A_SOUTH       },
  { "southeast"    , A_SOUTHEAST   },
  { "southwest"    , A_SOUTHWEST   },
  { "spin"         , A_TURN        },
  { "superbrief"   , A_SUPERBRIEF  },
  { "sw"           , A_SOUTHWEST   },
  { "take"         , A_TAKE        },
  { "travel"       , A_GO          },
  { "turn"         , A_TURN        },
  { "u"            , A_UP          },
  { "unlock"       , A_UNLOCK      },
  { "up"           , A_UP          },
  { "verbose"      , A_VERBOSE     },
  { "w"            , A_WEST        },
  { "wait"         , A_WAIT        },
  { "walk"         , A_GO          },
  { "wear"         , A_WEAR        },
  { "west"         , A_WEST        },
  { "x"            , A_EXAMINE     },
  { "z"            , A_WAIT        },

  {0, 0}
};



char *DirName[20] =
{
  "n", //direction order is important
  "north",
  "s",
  "south",
  "e",
  "east",
  "w",
  "west",
  "ne",
  "northeast",
  "nw",
  "northwest",
  "se",
  "southeast",
  "sw",
  "southwest",
  "u",
  "up",
  "d",
  "down"
};



//phrase must contain a maximum of three words

//for matching to work, phrases must be listed in order of decreasing word size

//different objects can have the same phrase, but there must
//be larger phrases for them, so player can be more specific when needed

//words must be separated by exactly one space

//NOTE: if there is a phrase like "disk drive" and then phrases "disk" and drive",
//which represent two different objects, parser will ask player to be more specific,
//so it is best to avoid "disk drive" and use something like "storage drive"

struct NOUNPHRASETOOBJ_STRUCT NounPhraseToObj[] =
{
  { "can of oil"         , OBJ_OIL          }, //three words
  { "piece of wood"      , OBJ_WOOD         },

  { "odd-shaped key"     , OBJ_KEY          }, //two words
  { "oil can"            , OBJ_OIL          },
  { "small crowbar"      , OBJ_CROWBAR      },
  { "rusty shovel"       , OBJ_SHOVEL       },
  { "brass lamp"         , OBJ_LAMP         },
  { "silver candelabra"  , OBJ_CANDELABRA   },
  { "diamond bracelet"   , OBJ_BRACELET     },
  { "wooden chest"       , OBJ_CHEST        },
  { "gold doubloons"     , OBJ_DOUBLOONS    },
  { "wooden stool"       , OBJ_STOOL        },
  { "precious ruby"      , OBJ_RUBY         },
  { "sparkling sapphire" , OBJ_SAPPHIRE     },
  { "gold toothpicks"    , OBJ_TOOTHPICKS   },
  { "worn pickaxe"       , OBJ_PICKAXE      },
  { "pearl necklace"     , OBJ_NECKLACE     },
  { "crystal goblet"     , OBJ_GOBLET       },
  { "small parachute"    , OBJ_PARACHUTE    },
  { "ivory tusk"         , OBJ_TUSK         },
  { "rare painting"      , OBJ_PAINTING     },
  { "rare wine"          , OBJ_WINE         },
  { "rare stamp"         , OBJ_STAMP        },
  { "jade ring"          , OBJ_RING         },
  { "bearskin rug"       , OBJ_RUG          },
  { "hard disk"          , OBJ_DISK         },
  { "small compass"      , OBJ_COMPASS      },
  { "short printout"     , OBJ_PRINTOUT     },
  { "platinum record"    , OBJ_RECORD       },
  { "power pack"         , OBJ_PACK         },
  { "priceless violin"   , OBJ_VIOLIN       },
  { "rusted pliers"      , OBJ_PLIERS       },
  { "broken goblet"      , OBJ_BROKENGOBLET },
  { "empty bottle"       , OBJ_BOTTLE       },
  { "storage drive"      , OBJ_DRIVE        },

  { "me"                 , OBJ_YOU          }, //one word
  { "myself"             , OBJ_YOU          },
  { "key"                , OBJ_KEY          },
  { "can"                , OBJ_OIL          },
  { "oil"                , OBJ_OIL          },
  { "crowbar"            , OBJ_CROWBAR      },
  { "shovel"             , OBJ_SHOVEL       },
  { "lamp"               , OBJ_LAMP         },
  { "candelabra"         , OBJ_CANDELABRA   },
  { "bracelet"           , OBJ_BRACELET     },
  { "chest"              , OBJ_CHEST        },
  { "doubloons"          , OBJ_DOUBLOONS    },
  { "stool"              , OBJ_STOOL        },
  { "ruby"               , OBJ_RUBY         },
  { "sapphire"           , OBJ_SAPPHIRE     },
  { "toothpicks"         , OBJ_TOOTHPICKS   },
  { "pickaxe"            , OBJ_PICKAXE      },
  { "pick-axe"           , OBJ_PICKAXE      },
  { "pick"               , OBJ_PICKAXE      },
  { "necklace"           , OBJ_NECKLACE     },
  { "goblet"             , OBJ_GOBLET       },
  { "parachute"          , OBJ_PARACHUTE    },
  { "tusk"               , OBJ_TUSK         },
  { "painting"           , OBJ_PAINTING     },
  { "wine"               , OBJ_WINE         },
  { "burger"             , OBJ_BURGER       },
  { "stamp"              , OBJ_STAMP        },
  { "ring"               , OBJ_RING         },
  { "rug"                , OBJ_RUG          },
  { "magazine"           , OBJ_MAGAZINE     },
  { "disk"               , OBJ_DISK         },
  { "compass"            , OBJ_COMPASS      },
  { "printout"           , OBJ_PRINTOUT     },
  { "record"             , OBJ_RECORD       },
  { "pack"               , OBJ_PACK         },
  { "wood"               , OBJ_WOOD         },
  { "violin"             , OBJ_VIOLIN       },
  { "pliers"             , OBJ_PLIERS       },
  { "goblet"             , OBJ_BROKENGOBLET },
  { "bottle"             , OBJ_BOTTLE       },
  { "drive"              , OBJ_DRIVE        },
  { "cover"              , OBJ_DRIVE        },

  { 0 , 0 }
};



struct NOUNPHRASETOFIXEDOBJ_STRUCT NounPhraseToFixedObj[] =
{
  //two-word noun phrases must come before one-word, for matching to work properly

  { "red dial"        , 110, FOBJ_DIAL        }, //two words
  { "red button"      ,  88, FOBJ_REDBUTTON   },
  { "blue button"     ,  88, FOBJ_BLUEBUTTON  },
  { "large button"    ,  98, FOBJ_LARGEBUTTON },
  { "large icicle"    ,  54, FOBJ_ICICLE      },
  { "small switch"    , 131, FOBJ_SWITCH      },
  { "small statue"    ,  30, FOBJ_STATUE      },
  { "wooden sign"     ,  11, FOBJ_WOODENSIGN  },
  { "iron knocker"    ,  24, FOBJ_KNOCKER     },
  { "large globe"     ,  39, FOBJ_GLOBE       },
  { "steel door"      ,  87, FOBJ_STEELDOOR   },
  { "steel door"      ,  97, FOBJ_STEELDOOR   },
  { "oak door"        ,  24, FOBJ_OAKDOOR     },
  { "oak door"        ,  29, FOBJ_OAKDOOR     },
  { "iron panel"      , 131, FOBJ_PANEL       },
  { "iron panel"      , 136, FOBJ_PANEL       },
  { "control panel"   ,  98, FOBJ_COMPUTER    },
  { "trap door"       ,  40, FOBJ_TRAPDOOR    },
  { "trap door"       ,  41, FOBJ_TRAPDOOR    },
  { "granite boulder" ,  64, FOBJ_GBOULDER    },
  { "white sand"      ,  96, FOBJ_SAND        },
  { "venus flytrap"   ,  84, FOBJ_FLYTRAP     },

  { "dial"            , 110, FOBJ_DIAL        }, //one word
  { "button"          ,  88, FOBJ_AMB         },
  { "button"          ,  98, FOBJ_LARGEBUTTON },
  { "icicle"          ,  54, FOBJ_ICICLE      },
  { "switch"          , 131, FOBJ_SWITCH      },
  { "statue"          ,  30, FOBJ_STATUE      },
  { "sign"            ,  11, FOBJ_WOODENSIGN  },
  { "sign"            ,  41, FOBJ_SIGN        },
  { "knocker"         ,  24, FOBJ_KNOCKER     },
  { "globe"           ,  39, FOBJ_GLOBE       },
  { "picture"         ,  31, FOBJ_PICTURE     },
  { "door"            ,  87, FOBJ_STEELDOOR   },
  { "door"            ,  97, FOBJ_STEELDOOR   },
  { "door"            ,  24, FOBJ_OAKDOOR     },
  { "door"            ,  29, FOBJ_OAKDOOR     },
  { "panel"           , 131, FOBJ_PANEL       },
  { "panel"           , 136, FOBJ_PANEL       },
  { "gate"            ,  11, FOBJ_GATE        },
  { "gate"            ,  21, FOBJ_GATE        },
  { "door"            ,  25, FOBJ_DOORMSG     },
  { "message"         ,  25, FOBJ_DOORMSG     },
  { "carving"         ,  25, FOBJ_DOORMSG     },
  { "door"            ,  27, FOBJ_DOORMSG     },
  { "message"         ,  27, FOBJ_DOORMSG     },
  { "carving"         ,  27, FOBJ_DOORMSG     },
  { "door"            ,  40, FOBJ_TRAPDOOR    },
  { "trapdoor"        ,  40, FOBJ_TRAPDOOR    },
  { "hinges"          ,  40, FOBJ_TRAPDOOR    },
  { "door"            ,  41, FOBJ_TRAPDOOR    },
  { "trapdoor"        ,  41, FOBJ_TRAPDOOR    },
  { "boulder"         ,  64, FOBJ_GBOULDER    },
  { "rock"            ,  64, FOBJ_GBOULDER    },
  { "stone"           ,  64, FOBJ_GBOULDER    },
  { "sand"            ,  96, FOBJ_SAND        },
  { "floor"           ,  96, FOBJ_SAND        },
  { "ground"          ,  96, FOBJ_SAND        },
  { "flytrap"         ,  84, FOBJ_FLYTRAP     },
  { "plant"           ,  84, FOBJ_FLYTRAP     },
  { "floor"           ,   7, FOBJ_CLEARGROUND },
  { "ground"          ,   7, FOBJ_CLEARGROUND },
  { "dirt"            ,   7, FOBJ_CLEARGROUND },
  { "hole"            ,   7, FOBJ_CLEARGROUND },
  { "message"         ,  10, FOBJ_TREEMSG     },
  { "tree"            ,  10, FOBJ_TREEMSG     },
  { "bark"            ,  10, FOBJ_TREEMSG     },
  { "carving"         ,  10, FOBJ_TREEMSG     },
  { "boards"          ,  20, FOBJ_FLOORBOARDS },
  { "floorboards"     ,  20, FOBJ_FLOORBOARDS },
  { "floor"           ,  20, FOBJ_FLOORBOARDS },
  { "wall"            ,  47, FOBJ_ROCKWALL    },
  { "boulders"        ,  47, FOBJ_ROCKWALL    },
  { "rocks"           ,  47, FOBJ_ROCKWALL    },
  { "stones"          ,  47, FOBJ_ROCKWALL    },
  { "floor"           ,  74, FOBJ_PITFLOOR    },
  { "ground"          ,  74, FOBJ_PITFLOOR    },
  { "dirt"            ,  74, FOBJ_PITFLOOR    },
  { "pit"             ,  74, FOBJ_PITFLOOR    },
  { "message"         ,  81, FOBJ_WALLMSG     },
  { "rock"            ,  81, FOBJ_WALLMSG     },
  { "stone"           ,  81, FOBJ_WALLMSG     },
  { "wall"            ,  81, FOBJ_WALLMSG     },
  { "shelf"           ,  20, FOBJ_SHELF       },
  { "cushion"         ,  26, FOBJ_CUSHION     },
  { "newel"           ,  30, FOBJ_NEWEL       },
  { "bookshelf"       ,  31, FOBJ_BOOKSHELF   },
  { "carpet"          ,  31, FOBJ_CARPET      },
  { "fireplace"       ,  31, FOBJ_FIREPLACE   },
  { "chandelier"      ,  32, FOBJ_CHANDELIER  },
  { "bed"             ,  38, FOBJ_BED         },
  { "mattress"        ,  38, FOBJ_BED         },
  { "desk"            ,  39, FOBJ_DESK        },
  { "skeleton"        ,  49, FOBJ_EXPLORER    },
  { "explorer"        ,  49, FOBJ_EXPLORER    },
  { "mainframe"       ,  87, FOBJ_COMPUTER    },
  { "computer"        ,  87, FOBJ_COMPUTER    },
  { "mainframe"       ,  98, FOBJ_COMPUTER    },
  { "computer"        ,  98, FOBJ_COMPUTER    },
  { "panel"           ,  98, FOBJ_COMPUTER    },
  { "artist"          ,  94, FOBJ_ARTIST      },
  { "display"         ,  98, FOBJ_DISPLAY     },
  { "printer"         ,  99, FOBJ_PRINTER     },
  { "generator"       , 102, FOBJ_GENERATOR   },
  { "cage"            , 102, FOBJ_CAGE        },
  { "furnace"         , 110, FOBJ_FURNACE     },
  { "contraption"     , 110, FOBJ_FURNACE     },
  { "cushion"         , 131, FOBJ_CUSHION     },
  { "piano"           , 135, FOBJ_PIANO       },
  { "footprints"      ,  59, FOBJ_FOOTPRINTS  },
  { "footprints"      ,  60, FOBJ_FOOTPRINTS  },
  { "prints"          ,  59, FOBJ_FOOTPRINTS  },
  { "prints"          ,  60, FOBJ_FOOTPRINTS  },
  { "window"          ,   3, FOBJ_WINDOW      },
  { "window"          ,  12, FOBJ_WINDOW      },
  { "window"          ,  22, FOBJ_WINDOW      },
  { "window"          ,  23, FOBJ_WINDOW      },
  { "window"          ,  32, FOBJ_WINDOW      },
  { "window"          ,  33, FOBJ_WINDOW      },
  { "window"          ,  36, FOBJ_WINDOW      },
  { "window"          ,  37, FOBJ_WINDOW      },
  { "window"          ,  41, FOBJ_WINDOW      },
  { "chasm"           ,  18, FOBJ_CHASM       },
  { "chasm"           , 138, FOBJ_CHASM       },
  { "fissure"         ,  50, FOBJ_FISSURE     },
  { "fissure"         ,  51, FOBJ_FISSURE     },
  { "fissure"         ,  60, FOBJ_FISSURE     },
  { "fissure"         ,  78, FOBJ_FISSURE     },
  { "fissure"         ,  79, FOBJ_FISSURE     },
  { "fissure"         ,  80, FOBJ_FISSURE     },

  {0, 0, 0}
};

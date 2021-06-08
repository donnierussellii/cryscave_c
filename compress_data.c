// Crystal Caverns
// (c) 1982 by Daniel Kitchen
// C port and parser (c) 2021 by Donnie Russell II

// This source code is provided for personal, educational use only.
// You are welcome to use this source code to develop your own works,
// but the story-related content belongs to the original author(s).



#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int TotalUncompressed;
int TotalCompressed;



//from compression.c
int CompressText(char *text_in, int size_in, char *text_out);



#define NUM_ROOMS  139 //including null room 0



// Room Name^Always Print This^Print This If Room Var Is 0^Otherwise Print This
// "Print This" can be empty

char *RoomDesc[NUM_ROOMS] =
{
  /*   0 */  "", //null room

  /*   1 */  "Gravel Road^You are standing on a gravel road in front of an old Victorian mansion. An eerie stillness hangs in the air.",
  /*   2 */  "End of Road^You are at the end of the road. There appears to be no other way to go from here except back.",
  /*   3 */  "South of Mansion^You are now facing the south side of the mansion. There are a few windows on this side. A high fence encircles the area around the mansion.",
  /*   4 */  "Forest Overlook^You are standing on the edge of the road overlooking a small wooded area.",
  /*   5 */  "Clearing^You have entered a small clearing surrounded by some woods. In the distance you can see a ramshackle Victorian mansion. A curl of smoke rises from the chimney.",
  /*   6 */  "Trail^You are following a dirt trail going through the trees. Ahead of you is another clearing. A warm breeze whispers softly through the trees.",
  /*   7 */  "Near Stump^You have come upon a second clearing some distance from the road. The ground is soft under foot and seems to have been disturbed at some earlier time. To the northeast is a break in the vines. Nearby is the rotted stump of a dead tree.^^There is a large hole in the ground here.",
  /*   8 */  "Leafy Path^You have discovered a little-used path hidden from view by vines. The path bends ahead of you. A carpet of dead leaves covers the path. They rustle under your feet as you move into the woods.",
  /*   9 */  "End of Path^The path ends at a tangle of bushes and vines. Droplets of dew on the leaves sparkle with an eerie green glow, making the leaves appear luminescent.",
  /*  10 */  "Near Tree^You find yourself in a small forest of trees where many leaves litter the ground. There appears to be something carved into the bark of a nearby tree.",
  /*  11 */  "Before Gate^You are now before a rusted iron fence surrounding the old mansion. A crudely-lettered wooden sign hangs from the fence near the gate.^The gate is closed and locked.^The gate is unlocked and open.",
  /*  12 */  "North of Mansion^You are now at the north side of the house. There are a number of windows on this side of the mansion. Unfortunately, the iron fence, with its bars topped with spikes, stands between you and the house.",
  /*  13 */  "Rear of Mansion^You are at the rear of the mansion. The siding on the house is old and rotted with age. The yard is overgrown with weeds. Through the fence you can see an ancient shed in one corner of the yard.",
  /*  14 */  "Gravel Road^You are walking along the gravel road. Dense thickets shield the woods on both sides of the road. The tree tops sway slowly in the breeze. A small translucent bit of smoke rises from the chimney, filling the air with a strange aroma.",
  /*  15 */  "Gravel Road^You are on the gravel road wandering through a wooded area. To one side you can see a break in the bushes. The road at this point widens.",
  /*  16 */  "Rocky Path^You are on a rocky path winding through the foliage. Up ahead, on one side, the path appears to lead into a large clearing.",
  /*  17 */  "Gravel Road^You are traveling upon the road which leads to the house. A strange aroma fills the air. The area is quite misty, with a thin layer of blue fog.",
  /*  18 */  "Ridge of Chasm^You are on the north ridge of a wide chasm which extends for miles in either direction. The road ends here. There are many caves hewed into the south wall of the chasm, which is about a mile wide here. The minerals embedded in the cave rock glisten from the light reflecting off the north wall. A small bit of blue mist drifts up from the unknown, below.",
  /*  19 */  "Beside Shed^You are by the edge of the backyard at the door of an old shed. The peeling paint and webs move slightly in the wind. A large dead tree trunk is lying here amid an overgrown garden. The shed lies north.",
  /*  20 */  "Tool Shed^You are in what appears to have been a tool shed. The only shelf is bare and dusty. The shed is quite small and smells very musty. A few of the floorboards are loose and quite rotted. You can hear a faint rumbling noise in the distance.^^There is a large hole in the floor.",
  /*  21 */  "Front Lawn^You are now on the front lawn of the mansion. The lawn is overgrown with weeds and underbrush. Except for the foliage, the lawn appears bare.^The gate is closed and locked.^The gate is unlocked and open.",
  /*  22 */  "Side of Mansion^You have walked around to the side of the house. There are a few windows here but none are on the ground floor. The house is a grayish blue, with a tint of age. The lawn here is covered with dead branches.",
  /*  23 */  "Side of Mansion^You are at the side of the mansion that faces south. A decrepit shutter hangs from the edge of a window as if it were floating. The air here is quite cold and dry. Around the back of the house you can see a dead tree stump.",
  /*  24 */  "Veranda^You are standing on a wide veranda. In front of you is a large oak door. Small red jewels are embedded around the edge of the door. The top of the veranda has a few holes and cracks. A large iron knocker hangs silently waiting.^The oak door is closed.^The oak door is open.",
  /*  25 */  "Back of Mansion^You are at the back of the mansion. In one corner there is a small shed. At its back is a boarded door. It appears to have something carved on it.",
  /*  26 */  "Beneath Shed, West^You are in a dim room far below the floor of the shed. Faint sunlight filters down through the hole above. A large air cushion lies almost directly beneath the hole. The room extends farther east, and there seems to be a faint rumbling noise coming from the north.",
  /*  27 */  "Near Door^You are now in front of a boarded up door at the back of the house. The boards are thick and nailed on securely. The door has some sort of message carved in it.",
  /*  28 */  "Hole^You find yourself at the bottom of a large hole. The opening of the hole is about two feet above your head, just enough to climb out. A small tunnel leads north and there is an opening in the eastern wall.",
  /*  29 */  "Hallway, First Floor^You have entered a small dimly lit hallway. The hallway is bare except for some dust. The wallpaper is faded and torn in places. The air is musty and still.^The oak door is closed.^The oak door is open.",
  /*  30 */  "Bottom of Stairs^You are in the hallway of the mansion. To the north, heavy wooden stairs rise steeply to the dark reaches of the upper floor. The wooden bannister ends here at a tall newel. A small statue stands atop the metal newel. To the south there is a large room. The hallway continues west.^The floorboards creak as you move.^To the northeast there is a small passage.",
  /*  31 */  "Parlor^You are now standing in the parlor. A thick colorful carpet covers the floor and a chandelier hangs motionless from the ceiling. A brick fireplace covers the south wall and an empty bookshelf stands against the eastern wall. A picture hangs over the fireplace.^^There is a small opening in the east wall.",
  /*  32 */  "Drawing Room^You are now in the drawing room. The room seems bare except for some webs and dust. A small chandelier, its six gas mantles casting a golden glow over the room, hangs from the ceiling. The windows here are boarded up.",
  /*  33 */  "Kitchen^You are in the kitchen of the old mansion. The counter is bare and very dusty. There is a small window in the west wall. Thin cobwebs hang from the ceiling.",
  /*  34 */  "Landing^You are on a landing at the top of the stairs. Another flight of stairs goes up from here to the second floor.",
  /*  35 */  "Beneath Stairs^You are in a secret room hidden beneath the stairs. The only exit is a small passageway in the southern wall.",
  /*  36 */  "Hallway, Second Floor^You are now in a short hallway on the second floor of the mansion. To the west there is a small window. Behind you is a flight of stairs going down to the first floor. In front of you, to the south, is a small bedroom. There is another room to the east, at the end of the hall.",
  /*  37 */  "End of Hall^You are at the end of the hall. To the east is a small window. There is another small window at the other end of the hall. To the south is a doorway which leads to a small room.",
  /*  38 */  "Bedroom^You are in a small bedroom about the size of a small closet. There is a bed here with a wooden frame and a straw mattress. The ceiling is covered with cobwebs.",
  /*  39 */  "Study^You are now in what appears to have been a study. Empty bookcases line the wall. A large wooden desk sits in one corner of the room. A large globe sits in another corner of the room. On the wall next to you is a wooden ladder fastened to the wall.",
  /*  40 */  "On Ladder^You are standing on a decrepit wooden ladder. Above you is a small trapdoor. Its hinges are very rusty and corroded.^The trapdoor is closed.^The trapdoor is open.",
  /*  41 */  "Cupola^You are now in the cupola of the old mansion. The room is round and very small. Dust covers the floor. In the north wall there is a window. A sign is nailed to the south wall. There is a trapdoor in the floor here.^The trapdoor is closed.^The trapdoor is open.",
  /*  42 */  "Behind Parlor^You are in a low damp passage behind the wall of the parlor. There is a hole in the floor here with a ladder leading down into the darkness below.",
  /*  43 */  "Cellar^You are now in the cellar of the old mansion. The air is damp and musty. Cobwebs cover the walls and ceiling. The floor here is hard dirt. You can hear the floorboards creak above your head.",
  /*  44 */  "North-South Tunnel^You are traveling in a small north-south tunnel. To the north the tunnel appears to narrow. A shaft of light is visible to the south.",
  /*  45 */  "Damp Passage^You are in a small damp passage. The ceiling is quite low here. A crawlway continues north. To the south, a faint light can be seen.",
  /*  46 */  "Junction^You have reached the junction of four passages. To the south, there is a small crawlway. A wide passage leads east and west from here. A walkway leads north. The ceiling here is quite damp and the floor is rather soft.",
  /*  47 */  "Sloping Walkway^You're heading along a small north-south walkway. The passage slopes downward here.^The western wall of the walkway is made up of loosely stacked boulders. To the north, a pool of quicksand blocks your way.^There is a hole in the western wall.",
  /*  48 */  "Cavernous Chamber^You are now in a huge, cavernous chamber where immense stalactites hang from the ceiling. The stalactites cast ominous shadows throughout the chamber. Your light reflects off a long, eerie shape in the northeast corner of the chamber. A low tunnel in the western wall exits the area.",
  /*  49 */  "Chamber Corner^You're in the northeast corner of the chamber. Here, your light dispels the shadows of the stalactites. Lying here amid fallen stalactites is the battered skeleton of a long-dead explorer. Wide stone steps lead down into the darkness of the lower caverns.",
  /*  50 */  "Fissure Steps^You are on the ridge of a wide fissure. To the east, along the ridge of the fissure, runs a narrow passage. Wide stone steps lead up from here.",
  /*  51 */  "Ridge of Fissure^You are traveling along the ridge of the fissure. The passage leads east-west. From far below you can hear a loud rumbling noise. A mist rises up from the depths of the fissure. There is a low tunnel in the wall to the north.",
  /*  52 */  "Rocky Room^You have discovered a room in which the floor is covered with scattered rocks and boulders. There is a small rocky corridor leading west.^The ceiling is quite low here.^There is a huge hole in the eastern wall.",
  /*  53 */  "Cool Passage^You are traveling along a wide east-west passage. The air is rapidly becoming cooler. A slight splashing sound originates from the passage toward the west.",
  /*  54 */  "Icy Room^^You've entered a long room whose walls are ice. Many large icicles hang from the ceiling. In the center of the room hangs a particularly large icicle. There are exits to the east and west.^You've entered a long room whose walls are solid rock. On the floor there are scattered puddles of water. There is a small passage leading north.",
  /*  55 */  "Cracked Room^You are in a room in which the walls are cracked due to frequent temperature changes. Exits lead north and east.",
  /*  56 */  "Winding Corridor^You are in a small winding corridor.^^There is an opening at the southern end of the corridor.",
  /*  57 */  "Rocky Corridor^You have entered a small rocky corridor. There are exits to the east and south.",
  /*  58 */  "East-West Passage^You are walking along a wide east-west passageway. There is a small hole in the floor here.",
  /*  59 */  "Near Footprints^You have entered a small room in which someone seems to have been recently. Footprints lead into a cave to the north. There is also a passage to the east.",
  /*  60 */  "Cave in Fissure^You have emerged into a cave on the southern wall of the fissure. The footprints end here. Across the fissure, through the mist which rises from below, you can see the faint outline of a path along the northern ridge. You can hear a faint rumbling noise from the depths of the fissure. There appears to be no way to descend.",
  /*  61 */  "Blasted Room^You are traveling along a narrow east-west passage which appears to have been blasted out of solid granite. To the east, the passage opens into a large area.",
  /*  62 */  "Quarry Overlook^You have reached a point which overlooks a large granite quarry. Large boulders lay on the floor of the quarry. A path leads down to the floor.",
  /*  63 */  "Floor of Quarry^You are on the floor of the quarry. Immense boulders litter the floor. A path through the boulders leads to the southeast. Another path leads up from here.",
  /*  64 */  "Quarry Corner^You are in the southeast corner of the quarry. A path leads northwest through the midst of a field of boulders.^A particularly large granite boulder lies near you.^There is a small hole in the floor.",
  /*  65 */  "Below Quarry^You have entered a small space beneath the floor of the quarry. Another room lies to the southwest. There is an opening above your head.",
  /*  66 */  "Cramped Room^You have squeezed into a small, cramped room which measures five by four feet. The ceiling is very low. The only exit is to the northeast.",
  /*  67 */  "Blocked Passage^The passage ends here at a wall of solidified lava. The path leads back toward the west from here.",
  /*  68 */  "Empty Room^You've crawled through a low tunnel and emerged into a large empty room. Your light reflects off the walls, casting shadows throughout the chamber. A small tunnel leads down.",
  /*  69 */  "Vertical Passage^You're climbing through a steep vertical passage. The walls here are pale white and the passage is very narrow. Exits lead up and down from here.",
  /*  70 */  "Fossil Room^You're in a huge echoing cavern whose walls glisten from your light. Embedded in the walls of the chamber are the fossilized remains of many ancient animals. There is a hole a foot above your head and a passage leading out of the northern wall.",
  /*  71 */  "Tall Passage^You are standing in a tall north-south passage. The walls are smooth ivory. Huge stalactites hang from the ceiling.",
  /*  72 */  "Cavern of Bones^You are in a huge, ominous cavern. The walls to the east and west are concave in appearance and of a gray color. What looks like large ribs line those walls from the ceiling to the floor. Large prehistoric bones are scattered on the floor of the cavern amid stalagmites. There is a small pit to the northwest. To the south is a tall passage.",
  /*  73 */  "Cavern Corner^You are at the brink of a small pit in the northwest corner of the prehistoric cavern. Flat stones encircle the edge of the pit. Small stone steps lead down to the bottom. There is a passage leading west.",
  /*  74 */  "Cavern Pit^You have descended into a small pit. The floor of the pit is soft in some places. Stone steps lead up its side.",
  /*  75 */  "Confusing Room^You have entered a room with confusing passages leading off in several directions.",
  /*  76 */  "Picture Room^You have emerged into a room which is decorated with prehistoric pictures on the walls and ceiling. There is a small cubbyhole in the southeast corner of the room.",
  /*  77 */  "Cubbyhole^You have entered a small cubbyhole in the southeast corner of the picture room. The only exit is northwest.",
  /*  78 */  "Bottom of Fissure^You are now at the bottom of the fissure. The mist here is very thick. It is quite humid and visibility is extremely limited. Flowing nearby is a river of molten lava. The noise is so loud you can hardly hear yourself think. Steam and bubbles rise from the surface of the lava. The temperature is almost unbearable. Paths lead east and west.",
  /*  79 */  "East End of Fissure^You have come to the eastern end of the fissure where there is a wall of solidified lava. The river flows out of a huge crack in the floor of the fissure here. The volcanic action causes tremendous noise. The whole area is humid and misty. The path leads west.",
  /*  80 */  "West End of Fissure^You are at the western end of the fissure. The lava flows westward from here into a narrow crack in the fissure wall. It is much too narrow for you to continue further. Through the steam you can see the outline of a tunnel in the southern wall.",
  /*  81 */  "Humid Room^You have entered a room which is slightly humid. On the western wall is a message written in the rock. Passages lead north and south.",
  /*  82 */  "Sloping Passage^You are in a sloping passage. It continues upward or back down from here.",
  /*  83 */  "East-West Crawlway^You are traveling in an east-west crawlway. Roots emerge from the northern wall. A strange odor fills the passage.",
  /*  84 */  "Green Room^You have entered a room in which the walls are lined with overgrown plants. A strong smell of fertilizer permeates the air. A crawlway exits to the east.^To the west an enormous, menacing, ravenous (and very ugly) Venus flytrap bars your passage.^There is a giant Venus flytrap here with a big smile on its face. A passage leads west.",
  /*  85 */  "Winding Walkway^You are journeying through a winding walkway. In the distance you can hear a faint whining noise.",
  /*  86 */  "Winding Passage^You are walking in a winding passage. A whining noise originates from the south.",
  /*  87 */  "Computer Room, North^You are in a large room with a very low ceiling. Scattered about the room are Hershey's Kisses wrappers and a few M&M's. To the south, in the center of the room, sits a large mainframe computer. To the east there is a storage drive and to the west a large printer. The only exit lies to the north.^The steel door is closed and locked.^The steel door is unlocked and open.",
  /*  88 */  "Computer Room, East^The noise in this part of the room is extremely loud. Next to you is a large storage drive. There are two buttons on the front panel: red and blue. There is a clear plastic cover on the top of the storage drive.^^There is a hard disk in the drive.",
  /*  89 */  "Japanese Room^You've entered a long, narrow chamber. Ancient Japanese markings adorn the walls and ceiling. Beautifully decorated tapestries hang on the eastern wall. Passages exit the room to the south and northeast.",
  /*  90 */  "Psychedelic Room^You're traversing a long north-south passage. The walls here are psychedelic, with ugly colors splattered about the passage. You find the color combinations quite revolting. (Good thing you didn't just eat lunch!)",
  /*  91 */  "North End of Passage^You are at the northern end of a north-south passage. The walls are very ugly. The passage here forks to the northeast and northwest.",
  /*  92 */  "Trapper Room^You have discovered a room which appears to have been the dwelling of a fur trapper. Tattered skins litter the ground, along with the rusted remains of a few beartraps. Walkways lead northeast and southwest.",
  /*  93 */  "Ugly Passage^You have come upon an extension of the ugly passage which leads to the northwest. The passage narrows at one end.",
  /*  94 */  "South End of Passage^You are at the end of the passage. The artist(?) of the psychedelic passage met his fate here, but his remains have long since decayed to nothing.",
  /*  95 */  "Tunnel^You are walking in a tunnel which narrows towards the northeast.",
  /*  96 */  "Sandy Room^You have entered a room in which the floor is white sand. There are tunnels to the southwest and east, and an opening in the northern wall.",
  /*  97 */  "Near Ramp^You're at the end of the winding passage. To the south there is a small ramp leading up to a large steel door. Loud noises can be heard from behind the door.^The steel door is closed and locked.^The steel door is unlocked and open.",
  /*  98 */  "Computer Room, South^You are standing in front of the computer control panel. The control panel consists of a CRT display, control lights and a large button labeled \"load\".",
  /*  99 */  "Computer Room, West^You are standing in front of what appears to be a dusty printer. Many cables are connected throughout the room.",
  /* 100 */  "North-South Passage^You are traveling along a north-south passage.^To the south lies a pool of quicksand.^The air is still and motionless.",
  /* 101 */  "Tight Room^You're in a tight room near the surface of the ground.",
  /* 102 */  "Generator Room^You have discovered a room which contains a large electric generator. The generator is the main power source for a large computer. A steel cage encloses it for safety reasons. There is an exit to the east.",
  /* 103 */  "Bending Passage^You are in a passage which bends to the west. An exit leads north.",
  /* 104 */  "Granite Gallery^You have come upon a granite gallery some distance under the surface of the ground. The sound of your footsteps echoes through the chamber. A path leads east and west.",
  /* 105 */  "Rocky Corridor^You're walking through a rocky corridor. The corridor opens towards the northwest and east.",
  /* 106 */  "Tilted Room^You're in a room in which the floor tilts at a steep angle toward the west. Passages exit to the west and southeast.",
  /* 107 */  "Humming Passage, East^You are in an east-west passage. A faint hum can be heard towards the west. Another passage lies north from here.",
  /* 108 */  "End of Passage^The passage ends at a solid stone wall.",
  /* 109 */  "Humming Passage, West^You're traveling in a long east-west passage. A loud humming noise can be heard from the west. The stone walls here oscillate with an electricity that makes your hair stand on end.",
  /* 110 */  "Furnace Room^You've entered a cramped room with a low ceiling. A large furnace sits in the middle of the room. The contraption is about six feet tall and four feet wide. Many pipes connected to the furnace exit the room near the ceiling. On the furnace there is a small red dial. A passage leads southeast.",
  /* 111 */  "Maze^You are in a series of winding passages which appear similar.",
  /* 112 */  "Maze^You are in a series of curvy passages which appear similar.",
  /* 113 */  "Maze^You are walking in a series of circuitous passages.",
  /* 114 */  "Maze^You are walking in a series of similar-looking passages.",
  /* 115 */  "Maze^You are in a series of winding passages which appear similar.",
  /* 116 */  "Maze^You are in a series of curvy passages which appear similar.",
  /* 117 */  "Maze^You are walking in a series of circuitous passages.",
  /* 118 */  "Maze^You are walking in a series of similar-looking passages.",
  /* 119 */  "Maze^You are in a series of winding passages which appear similar.",
  /* 120 */  "Maze^You are in a series of curvy passages which appear similar.",
  /* 121 */  "Maze^You are walking in a series of circuitous passages.",
  /* 122 */  "Maze^You are walking in a series of similar-looking passages.",
  /* 123 */  "Maze^You are in a series of winding passages which look similar.",
  /* 124 */  "Maze^You are in a series of curvy passages which appear similar.",
  /* 125 */  "Maze^You are walking in a series of circuitous passages.",
  /* 126 */  "Maze^You are walking in a series of similar-looking passages.",
  /* 127 */  "Dusty Room^You have discovered a little-used room connected to the maze. Thick dust covers the floor and cobwebs hang from the ceiling.",
  /* 128 */  "Bank of Lava River^You are on the north bank of the lava river. To the south, the rickety bridge spans the river. A small, winding passage lies to the north. The ground here is solidified lava.",
  /* 129 */  "Forest, Near Road^You're in a small forest near the gravel road. A path continues northeast.",
  /* 130 */  "Near Abyss^You are now in a tremendous underground chamber through which a raging lava river flows. The river originates from a large crack in the eastern wall of the chamber and disappears into a large abyss in the floor to the west. To the north, a rickety wooden footbridge spans the lava river about ten feet above its surface. Steam rises from the river and fills the chamber. A path leads south.",
  /* 131 */  "Beneath Shed, East^You are at the eastern end of the room which lies beneath the shed. At the opposite end of the room lies a large air cushion. There is a small switch on the southern wall nearby.^The eastern wall is covered by a heavy iron panel.^To the east there is a passage through the iron panel.",
  /* 132 */  "Rickety Footbridge^You are standing on the footbridge, which seems even more rickety than it appeared at first. Many of its planks are rotted or missing. The boards creak under your weight and the bridge begins to sway dangerously.",
  /* 133 */  "Dead End^You are at a dead end. The passage leads back toward the east.",
  /* 134 */  "Climbing Tunnel^You have entered a long narrow tunnel which climbs steadily upward towards the northwest. The tunnel exits to the south.",
  /* 135 */  "Music Studio^You have entered a room which seems to have once been the studio for a music student of some sort. A broken-down piano stands against one wall. Several keys are splintered or missing. Crumpled pieces of paper, which may have been attempts at concertos, litter the floor.",
  /* 136 */  "Domed Room^You are in a small, poorly ventilated room in which the walls are decorated with musical scales. The ceiling is quite high and is shaped like a dome, giving the room excellent acoustics.^To the northwest, an iron panel covers the wall.^A passage leads northwest through the iron panel.",
  /* 137 */  "Forest^You're in a dim forest. A path leads northeast and southwest.",
  /* 138 */  "Edge of Chasm^You are standing on the edge of the chasm. Far below you can see many large rocks and boulders. A path leads west and southwest."
};



#define NUM_BLOCK_MESSAGES  17



char *BlockMsg[NUM_BLOCK_MESSAGES] =
{
  "You can't go that way.",
  "The foliage has become too dense to continue any further.",
  "There is no passage in that direction.",
  "The underbrush is too high to continue.",
  "There's no way over the fence.",
  "There's no way around the chasm.",
  "The wall here is solid sandstone.",
  "If you wish to get off the veranda, I suggest you use the steps.",
  "The door is boarded up.",
  "You can't leave through the window.",
  "Walking into the wall won't help.",

  "The gate is closed and locked.",
  "The oak door is closed.",
  "The trapdoor is closed.",
  "The steel door is closed and locked.",
  "You can't get by the quicksand!",
  "You can't get by the Venus flytrap!"
};



#define NUM_OBJECTS  38



char *ObjectDesc[NUM_OBJECTS] =
{
  "nothing^Nothing is here.",

  "you^You are here.",

  "an odd-shaped key^There is an odd-shaped key here.",
  "a can of oil^There is a small can of oil nearby.",
  "a small crowbar^There is a small metal crowbar here.",
  "a rusty shovel^Lying beside you is an old rusty shovel.",
  "a brass lamp^Beside you is a pitted brass lamp.",
  "a silver candelabra^There is a silver candelabra here.",
  "a diamond bracelet^A diamond bracelet lies nearby.",
  "a wooden chest^There is a small, dirty wooden chest here.",
  "gold doubloons^There are gold doubloons here.",
  "a wooden stool^There is a small wooden stool here.",
  "a precious ruby^There is a precious ruby here.",
  "a sparkling sapphire^A sparkling sapphire lies nearby.",
  "gold toothpicks^There is a set of gold toothpicks here.",
  "a worn pickaxe^There is a worn pickaxe here.",
  "a pearl necklace^A gleaming pearl necklace lies here.",
  "a crystal goblet^There is a crystal goblet lying here.",
  "a small parachute^Nearby lies a small parachute.",
  "an ivory tusk^There is an ivory tusk here.",
  "a rare painting^A rare painting lies nearby.",
  "a rare wine^There is a rare wine here.",
  "a burger^Lying nearby is a delicious burger.",
  "a rare stamp^A first edition stamp lies here.",
  "a jade ring^A beautiful jade ring is here.",
  "a bearskin rug^An expensive bearskin rug lies here.",
  "a magazine^There is a copy of CHEW magazine here.",
  "a hard disk^There is a hard disk lying here.",
  "a small compass^Nearby lies a small compass.",
  "a short printout^A short printout lies here.",
  "a platinum record^There is a platinum record nearby.",
  "a dusty power pack^A rather dusty power pack lies here.",
  "a piece of wood^A splintered piece of wood lies here.",
  "a priceless violin^There is a priceless violin here.",
  "rusted pliers^A rusted pair of pliers lies on the ground.",
  "a broken goblet^There is a broken goblet here.",
  "an empty bottle^There is an empty bottle here.",
  "a storage drive^NOT PRINTED"
};



void CompressPrintList(FILE *f, char *listname, char *list[], int listsize)
{
  int i, j, uncompressed_size, compressed_size;
  char *uncompressed_text, *compressed_text;
  char *hex = "0123456789abcdef";

  fprintf(f, "unsigned char *%s[%i] =\n{\n", listname, listsize);
  for (i=0; i<listsize; i++)
  {
    uncompressed_text = list[i];
    uncompressed_size = strlen(uncompressed_text)+1;

    compressed_text = malloc(uncompressed_size);
    compressed_size = CompressText(uncompressed_text, uncompressed_size, compressed_text);

    TotalUncompressed += uncompressed_size;
    TotalCompressed   += compressed_size;

    fputs("  \"", f);
    for (j=0; j<compressed_size; j++)
    {
      unsigned char ch = compressed_text[j];

      fputs("\\x", f);
      fputc(hex[ch >> 4], f);
      fputc(hex[ch & 15], f);
    }
    fputs((i < listsize-1) ? "\",\n" : "\"\n", f);

    free(compressed_text);
  }
  fputs("};\n\n", f);
}



void main(void)
{
  FILE *f;

  TotalUncompressed = 0;
  TotalCompressed   = 0;

  f = fopen("_data.c", "w");
  CompressPrintList(f, "RoomDesc", RoomDesc, NUM_ROOMS);
  CompressPrintList(f, "BlockMsg", BlockMsg, NUM_BLOCK_MESSAGES);
  CompressPrintList(f, "ObjectDesc", ObjectDesc, NUM_OBJECTS);
  fclose(f);

  printf("%i%%\n", 100 * TotalCompressed / TotalUncompressed);
}

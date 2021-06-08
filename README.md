A few months ago I was watching a YouTube channel called Apple Adventures.
The channel features classic Apple ][ text adventures being played from
start to finish using Internet walkthroughs.

One particular game caught my interest. It was created by Dan Kitchen in
the early 1980s, before he worked for Activision. The game is a Zork clone
called Crystal Caverns. It's not great; the map is mostly empty, and the
puzzles are not very complex. The parser is also very limited. I liked the
descriptions and setting, though; it reminds me a little of Hollywood Hijinx.

I opened the game in a hex editor and found that it runs on an engine likely
written by Kitchen in 6502 assembly language (there is an Apple and later
Commodore 64 version). I found a file containing data structures representing
rooms and objects. The file was the same on both platforms. I thought since
the data was already fairly organized, I could write a new version in C, and
create a better parser at the same time.

ANSI.SYS-style escape sequences can be compiled in to add a status line. These
are supported on the current version of Windows.

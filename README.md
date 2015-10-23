# energy_rush

This is a simple minesweeper clone using a hex grid instead. But the actual
gameplay is the same. The games includes 3 different modes just like the original.
It is actually more a test environment to see how to work with a hex grid and axial coordinates.
Nevertheless it is still fun to play.

# Requirements

This game requires DirectX SDK version 9.

# How to build

There is a Visual Studio 2015 Solution in the repository. Beside the actual code you also
need the following to other projects:
https://github.com/amecky/diesel2D
https://github.com/amecky/math
The best way is that each project is on the same hierarchy in your filesystem next to each other.
You only need to edit the settings for the C++ directories to include your DirectX SDK.

Currently you also need to create a subdirectory called "data" inside the energy_rush directory.

Unfortunately there is absolutely no documentation available on Diesel2D. This is a 2D game engine
written by me and the foundation of every game that I build. But to understand how this game itself
is working I think it is not too difficult.

# Contact

If you have any questions please contact me at amecky@gmail.com.
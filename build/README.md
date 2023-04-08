__The simulator only works with Windows currently!__

__The following tools are required:__
1. CMake 3.24 or newer.
2. MinGW 9.2.0 or newer.

__To compile the PC simulator, you need to follow these steps in a terminal:__
1. `cd build` In the terminal, your directory have to be the __build__ folder.
2. `cmake .. -G "MinGW Makefiles"` It will create the build system for GCC.
3. `cmake --build .` It will compile the PC simulator. The compuled binary will be located in the build folder as __Shellminator.exe__
4. `.\Shellminator.exe` Run the simulator.
# Project Chimera - CSCI 3010 Semester Project
### Jason Cummings, Alex Burnley, Riley Kirkham
A basic 3D platformer game with a toon shading style.

## Requirements
Your computer must have support for OpenGL 3.3. Most dependencies are included and built with the project with the exception of the following:
* Boost filesystem library
    * 'brew install boost' or https://www.boost.org/users/download/


## Usage
### Mac/Linux
Ensure that you have CMake version 3.6+ installed on the command line, then build the project by executing:
```
./scripts/cmake-make.sh -j8
```

Then run the game executable with:
```
./bin/bin/projectchimera
```

To rebuild without rerunning cmake, cd to the ./bin/ directory and run:
```
make -j8
```

### Windows

#### Option 1 - Visual Studio Project File
Using the CMake GUI, enter ```[full path to project]``` in the "Where is the source code:" field, and enter ```[full path to project]/bin``` in the "Where to build the binaries:" field. Then press the "Configure" and "Generate" buttons to generate a Visual Studio project. The game can then be built and run from Visual Studio.

#### Option 2 - NMAKE Makefiles
If you have Visual Studio Developer Tools with NMAKE installed, you can build the project from cmd using:
```
scripts\cmake-nmake.bat
```

Then run the game executable with:
```
bin\bin\projectchimera.exe
```

To rebuild without rerunning cmake, cd to the .\bin\ directory and run:
```
nmake
```

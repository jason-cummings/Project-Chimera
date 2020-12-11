## FbxConverter

The FbxConverter is meant to be an internal tool for converting FBX files to our own format. Since this tool is meant to be run once on fbx files, it does a lot of optimization (ensuring no duplicate vertices), and can take a while to run on files with large meshes.

#### Building:
The tool has been updated to build and run with CMake. Similar to the base directory of chimera, this folder contains a ```scripts``` subdirectory which contains sh and bat scripts for running CMake and compiling. To build, use the command ```./scripts/cmake-make.sh -j8``` on Mac, or ```scripts\cmake-nmake.bat``` from cmd on Windows.


#### Usage:
A wrapper script has been added to manage running the FbxConverter and moving created directories appropriately. This script accepts both the main fbx and the hitbox fbx as arguments and runs the actual FbxConverter program twice, once for each step. To run, use the following (note: this should be run from git-bash or bash equivalent on Windows because I'm too lazy to try to figure out an equivalent .bat):

```
./run.sh <main_fbx_filename> <hitbox_fbx_filename> [-d]
```

The ```-d``` flag can be used to enable debug print.

The hitbox option is done in a separate step as in most cases you will not want to use your rendered meshes as hitboxes. In this case, you will want a second fbx file that has the same node structure, but only includes meshes that you want as hitboxes. For example, a complex mesh can be replaced with a cube mesh, so that in game, the complex mesh has a simple hitbox, which is better for performance.

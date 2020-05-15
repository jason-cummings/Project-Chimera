The FbxConverter is meant to be an internal tool for converting FBX files to our own format. Since this tool is meant to be run once on fbx files, it does a lot of optimization (ensuring no duplicate vertices), and can take a while to run on files with large meshes.

Building:
The tool can be built using the makefile. However, due to struggles with including the FBX SDK, the tool may not build properly or run due to not finding the FBX SDK Library. If we require more heavy use of this tool in the future, we may try and resolve these issues to make the tool more cross platform and more distributable, however at this time our focus is on the game itself.

Usage:
Begin by processing the file with:
"./FbxParser <filename>.fbx"

Next, to add hitboxes to the output:
"./FbxParser <filename>.fbx -h"

The hitbox option is done in a separate step as in most cases you will not want to use your rendered meshes as hitboxes. In this case, you will want a second fbx file that has the same node structure, but only includes meshes that you want as hitboxes. For example, aa complex mesh can be replaced with a cube mesh, so that in game, the complex mesh has a simple hitbox, which is better for performance.

### Project Chimera - Homework 3 Checkpoint

#### Section 1 - Original Plan
Project Proposal: "At this point the user will be able to see the 3D world around them and control the player. The level at this point will not have a character model, and the level will only consist of a few boxes for testing purposes. The basic physics engine integration will be working at this point."

#### Section 2 - Actual Progress
At this point, we haven't quite reached our original goals for this checkpoint. We have however made a lot of progress on the overall architecture of our engine. Our engine currently builds from scratch with the use of a single script on both Windows and Mac, and presumable Linux (not tested but should be the same as Mac) without the need for the user to install additional dependencies.

We've started off by putting most of our focus on our scene graph and render system implementations. Our scene graph is fully functional and allows to set up scenes with a cascading hierarchy of transformation data. We have also set up a substantial portion of our rendering pipeline. We have a light in the scene and toon shading applied to the final result.

We are also currently loading in the rendered objects from an assets folder. These assets are converted into GameObjects, which are passed to RenderSystem for rendering. These assets are created by a separate program we've written and included in our GitHub repo as a tool to convert our model .fbx files to assets we can use easily. Building a program with the FBX SDK is complex and loading data in is a long process, so the fbxConverter is a separate tool that is not intended on being cross platform or being packaged with the game. While we have included the code for this tool and a makefile, it will likely not build on your computer. Since we will only be using it occasionally to prepare our assets for the game, we are not planning on making it very easy to build or use.

In short, our basic rendering process is in place and uses assets created externally. This lays a lot of the groundwork for being able to load complex levels once we have built them out.

Finally, we have a player object that uses a mesh of a head and can be controlled with WASD. There is no camera movement with the player currently and the player just moves in a plane at the moment. There is also a bug where sometime key releases are not registered. This player movement will be updated going forward to be much more game-like.

What we don't have at the moment is physics system integration. In our original proposal, we said that we would be spending the first weeks setting up the physics world and then have game object integrated into the world by this checkpoint. However, we quickly realized that we had a lot more architectural work to do before even beginning to consider how to implement physics. As of now, we have a physics system instance being used in Engine that just runs a basic hello world example provided by bullet. This example simulation is stepped in the background, has no visual representation in our program, and is just included as a proof of concept for bullet's use. We have an outline for a PhysicsComponent class that will be used in our GameObject class, but we haven't yet figured out how we're going to construct these PhysicsComponents for objects from the assets we are reading in.

#### Section 3 - Next Steps
Our original plan for the next deadline: "At this point, basic skeletal animation should be implemented. We are not expecting animations to be triggered by any actions or events, but will likely run in a loop. Blending animations will also not be implemented. At this point the player will be able to interact with the world and physics will be applied to the player. If all goes well, the next steps for the physics engine after this point will be implementing more advanced game mechanics, but we have not defined those yet."

Given the current state of our program, we believe we can get back on track finish up all our desired objectives by this point in time. As mentioned above, our rendering system has come well along and as such, we don't forsee any problems implementing skeletal animations as we have had past experiences with this. As for the physics objectives, one of our main short-term focuses will be figuring out how to incorporate physics based on data in the Assets folder. Once we figure this out, we will be able to dynamically create physics components for object in our scene, including for the player. Because the Player is just another GameObject in the scene, no additional work will have to be done in order to have the player interact with physical objects.

#### Section 4 - Screenshots
![Alt text](/checkpoints/screenshots/homework3.png?raw=true "Homework 3 Screenshot")

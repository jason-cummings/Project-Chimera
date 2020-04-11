### Project Chimera - Homework 4 Checkpoint

#### Section 1 - Original Plan
Project Proposal: At this point, basic skeletal animation should be implemented. We are not expecting animations to be triggered by any actions or events, but will likely run in a loop. Blending animations will also not be implemented. At this point the player will be able to interact with the world and physics will be applied to the player. If all goes well, the next steps for the physics engine after this point will be implementing more advanced game mechanics, but we have not defined those yet.

#### Section 2 - Actual Progress
At this point, we have completed nearly all of the goals we outlined in the original proposal. The most notable progress is that we now have our basic implementation working, which we were missing from last checkpoint. Objects in our scene now have hitboxes dynamically loaded from files. We also add in a few hardcoded cubes to play around with which are affected by gravity and other physical objects. Finally, the very basic player movement and camera systems are implemented. The player model is represent by a large block at the moment, but this player can be controlled by WASD/Space and can run around our scene and jump on things. There is still a lot of work to do to tune the movement to make it feel more like a normal video game, but now that we have the basic system in place, these adjustments should not be difficult.

Another important system we've implemented is the basic animation system. This allows us to move around platforms and objects in our scene, which is demonstrated in the AnimatedRaptor level. Also, although not mentioned in our proposal plan, we have a basic material implementation which allows us to texture all of the objects in our scene. We haven't actually created textured levels yet, so all the objects currently have the videogame standard "missing" texture (pink/black checkerboard). However, the architecture is already in place to use the correct materials when we create the textured levels.

The only goal in our proposal that we have not reached is skeletal animation, which will control the animation of the player and other similar objects.

##### Individual Progress
Jason:
* Level loading functionality
* Basic physics implementation

Alex:
* FBX parser work to export level info
* Basic animations
* Bits of rendering pipeline optimization

Riley:
* Camera control and initial pre-physics player movement in scene
* Material loading and displaying

#### Section 3 - Next Steps

Going forward, much of what we have left to do is polishing things and creating the actual levels and models to load into our game. As far as actual coding, we need to get skeletal animation done. However, Alex has experience with skeletal animations and shouldn't have much of a problem implementing it. We will also add some effects to our render system, the primary two being shadows and bloom in our renders. Once these elements are done, we will consider our rendering process to be complete for the purposes of this project. If we find we have extra time towards the end, we may implement additional effects such as volumetric light scattering and anti-aliasing.

As far as the level physics, as mentioned before, the player movement system still has a fair bit of tuning to be done to get the movement to be more natural and game-like. This will mostly be a matter of playing around with various settings such as friction and velocity damping while in air vs on the ground.

Finally, we'll be implementing a couple of menus, including a main menu which the program will open into and which will allow the player to select a level. We will also have a pause menu that will allow the player to go back to the main menu.

#### Section 5 - Video
![Alt text](/checkpoints/videos/HW4Checkpoint.mp4?raw=true "Homework 4 Checkpoint Video")

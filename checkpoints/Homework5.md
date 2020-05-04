### Project Chimera - Homework 4 Checkpoint

#### Section 1 - Original Plan
Project Proposal: The last two weeks will be spent polishing bugs and the appearance of the game. We may add more details into our levels and models, and improve the transitions between character animations if necessary.

#### Section 2 - Actual Progress
These last two weeks that have definitely not just been bug fixes, but at this point, we have gotten everything done that we intended to when we set out. We implemented state management to navigate between menus and the playable state smoothly and even threw in a loading screen for when we are loading into the actual game. We added a fully textured and animated level that has an end goal that triggers a win screen and is actually quite difficult. The completed run you'll see in the video took about 10 takes to get, and that was after a few hours of playing it already.

As far as other functions we implemented, we added shadows to the game, and allow the user to use F1 to cycle between shadow modes for performance (none, single pass, iterated). We added bloom effects to make it appear as though different parts of our level and overlays are lit up and glowing. And we added blended skeletal animations and our player model, which allow for somewhat smooth transitions between different animations. Finally we added a constant skybox which ties the level together.

Outside of that, a lot of time was spent on the various textures and level files, as well as fine tuning values for lighting and other visual effects, and then just performing last minute bug fixes and testing.

##### Individual Progress
Jason:
* Implemented shadow mapping
* Created menu textures
* Assisted with bloom

Alex:
* Implemented skeletal animations in the project
* Smoothly blended character animations based on player state changes
* Created character and level files
* Created skybox

Riley:
* Created menu states and button interactions
* Implemented state management and swapping functions
* Added bloom effects
* Created the actual animations

#### Section 3 - Next Steps

Obviously, we're done with this project for this class, but we're planning to keep working on this for fun going forward and will be reworking some of our architecture and functionality. The player movement system for instance is functional for this submission, but is very jankily thrown together. Now that I (Jason) have a much better understanding of Bullet and its internal structure, I want to rework how a lot of physics and movement is done to make it more intuitive and smooth. Also we want to have a bit of fun doing some more fancy graphical effects and game mechanics. 

#### Section 4 - Video
See ```/checkpoints/videos/FinalSubmission.mp4```

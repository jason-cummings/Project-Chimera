#ifndef POST_PROCESS_H
#define POST_PROCESS_H


#include <GL/glew.h>
#include "./../Framebuffer.hpp"

// PostProcess Class
//  - A post process is defined as a process that applies any kind of visual effect to an image

// PostProcess Subclass Constructors
//  - PostProcesses should avoid having input data textures in the constructor. The identifier of the data 
//    textures may change when the resolution of textures is changed in game. Instead of setting these 
//    values through the constructor, these values should be set using setters. the setters should be 
//    called in the setFrameBufferTextures function in RenderSystem. this will be called at initialization,
//    and ensures that these values will be reset anytime the texture resolution is changed.
//  - Any values provided in the constructor should be values that will not change due to resetting texture 
//    resolutions.

// Post process target framebuffer
//  - The target framebuffer (required by the post process constructor) should only have one texture target
//    processes that need more than one final texture target should not be considered post processes, as 
//    they are storing data that will be used in another process
//  - Post processes may contain their own intermediate framebuffers, for example, a bloom framebuffer 
//    may have framebuffers for blurring before applying the effect to the "target_buffer"

class PostProcess {
private:
	Framebuffer * target_buffer;

protected:
	void bindTargetBuffer();

public:
	inline PostProcess(Framebuffer * target) {target_buffer = target;}
	inline virtual void createFrameBuffers() {} 
	inline virtual void clearFrameBufferTextures() {}


	virtual void apply() = 0;
	
};

#endif
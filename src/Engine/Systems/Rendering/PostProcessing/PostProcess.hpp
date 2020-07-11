#ifndef POST_PROCESS_H
#define POST_PROCESS_H


#include <GL/glew.h>
#include "./../Framebuffer.hpp"

// PostProcess Class
//  - A post process is defined as a process that applies any kind of visual effect to an image

// PostProcess Subclass Constructors
//  - Any input data needed by the post process (frame buffer textures for input to shaders) 
//    should be passed in through the constructor of the post process

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


	virtual void apply() = 0;
	
};

#endif
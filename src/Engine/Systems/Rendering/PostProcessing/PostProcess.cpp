#include "PostProcess.hpp"

void PostProcess::bindTargetBuffer() {
	if(target_buffer != nullptr) {
		target_buffer->bind();
	}
	else glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
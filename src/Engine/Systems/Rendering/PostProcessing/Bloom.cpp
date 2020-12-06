#include "Bloom.hpp"

Bloom::Bloom(Framebuffer *target) : PostProcess(target) {
    bright_texture = 0;
    sm = ShaderManager::getShaderManager();
}

void Bloom::createFrameBuffers() {
    // Set up the pingpong buffers for blurring
    blur_buffer[0].addColorTexture("FragColor", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight());
    blur_buffer[1].addColorTexture("FragColor", RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight());
}

void Bloom::clearFrameBufferTextures() {
    // Set up the pingpong buffers for blurring
    blur_buffer[0].clearAll();
    blur_buffer[1].clearAll();
}

void Bloom::apply() {

    Shader *blur_shader = sm->getShader("blur");
    blur_shader->bind();
    blur_shader->setUniformInt("colorTexture", 0);
    glActiveTexture(GL_TEXTURE0);

    current_blur_buffer = 0;

    for (int i = 0; i < BLOOM_PASSES; i++) { // BLOOM_PASSES
        blur_shader->bind();
        blur_buffer[current_blur_buffer].bind();
        glViewport(0, 0, RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight());
        glClear(GL_COLOR_BUFFER_BIT);

        blur_shader->setUniformFloat("horizontal", (float)current_blur_buffer);
        GLuint tex_to_use = i == 0 ? bright_texture : blur_buffer[!current_blur_buffer].getTexture("FragColor")->getID();
        glBindTexture(GL_TEXTURE_2D, tex_to_use);

        RenderUtils::drawQuad();

        current_blur_buffer = !current_blur_buffer;
    }

    glViewport(0, 0, RenderUtils::getTextureWidth(), RenderUtils::getTextureHeight());

    RenderUtils::testGLError("Blur");

    bindTargetBuffer();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    Shader *quad_shader = sm->getShader("quad");
    quad_shader->bind();
    quad_shader->setUniformInt("colorTexture", 0);

    // Bind the texture to render
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blur_buffer[!current_blur_buffer].getTexture("FragColor")->getID());

    RenderUtils::drawQuad();

    glDisable(GL_BLEND);

    glUseProgram(0);
}
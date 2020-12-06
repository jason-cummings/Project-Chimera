#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <string>
#include <vector>

#include "../../Utilities/FilesystemWrapper.hpp"
#include "Shader.hpp"

const std::string skybox_texture_names[] = {
    "positiveX.png", "negativeX.png",
    "positiveY.png", "negativeY.png",
    "positiveZ.png", "negativeZ.png"};

class Skybox {
  private:
    GLuint vao;
    GLuint vbo;
    GLuint cubemap;

  public:
    Skybox(GLuint vao_in, GLuint vbo_in, GLuint cubemap_in);
    void draw(Shader *shader);
};

class SkyboxFactory {
    static bool initialized;
    static GLuint vao;
    static GLuint vbo;

    static GLuint createCubeMap(std::vector<fs::path> texture_names);
    static void initializeVAO();

  public:
    static Skybox *getSkybox(std::string directory);
};

const GLfloat skybox_vbo_data[] = {
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

#endif
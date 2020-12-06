#include "MeshFactory.hpp"

#include <iostream>

#define VBO_FILENAME "VBO"
#define IBO_FILENAME "IBO"

Mesh *MeshFactory::createBasicMesh(std::string input_directory) {
    Asset newVBOAsset(input_directory + "/" + VBO_FILENAME);
    if (!newVBOAsset.success()) {
        std::cerr << "Could not open VBO in " << input_directory << std::endl;
        return nullptr;
    }
    Asset newIBOAsset(input_directory + "/IBO");
    if (!newIBOAsset.success()) {
        std::cerr << "Could not open IBO in " << input_directory << std::endl;
        return nullptr;
    }

    // Create VBO and IBO
    GLuint vbo = createVBO(newVBOAsset);
    GLuint ibo = createIBO(newIBOAsset);

    Mesh *mesh = new Mesh(vbo, ibo, newIBOAsset.getBytes() / sizeof(int));
    return mesh;
}

Mesh *MeshFactory::createBasicMesh(fs::path input_directory) {
    fs::path vbo_path = input_directory;
    vbo_path.append("VBO");
    Asset newVBOAsset(vbo_path);
    if (!newVBOAsset.success()) {
        std::cerr << "Could not open VBO in " << input_directory << std::endl;
        return nullptr;
    }

    fs::path ibo_path = input_directory;
    ibo_path.append("IBO");
    Asset newIBOAsset(ibo_path);
    if (!newIBOAsset.success()) {
        std::cerr << "Could not open IBO in " << input_directory << std::endl;
        return nullptr;
    }

    // Create VBO and IBO
    GLuint vbo = createVBO(newVBOAsset);
    GLuint ibo = createIBO(newIBOAsset);

    Mesh *mesh = new Mesh(vbo, ibo, newIBOAsset.getBytes() / sizeof(int));
    return mesh;
}

SkinnedMesh *MeshFactory::createSkinnedMesh(std::string input_directory, JointList *joint_list) {
    Asset newVBOAsset(input_directory + "/VBO");
    if (!newVBOAsset.success()) {
        std::cerr << "Could not open VBO in " << input_directory << std::endl;
        return nullptr;
    }

    Asset newIBOAsset(input_directory + "/IBO");
    if (!newIBOAsset.success()) {
        std::cerr << "Could not open IBO in " << input_directory << std::endl;
        return nullptr;
    }

    // Create VBO
    GLuint vbo = createVBO(newVBOAsset);

    // Create IBO
    GLuint ibo = createIBO(newIBOAsset);

    SkinnedMesh *skinned_mesh = new SkinnedMesh(vbo, ibo, newIBOAsset.getBytes() / sizeof(int), joint_list);
    return skinned_mesh;
}

SkinnedMesh *MeshFactory::createSkinnedMesh(fs::path input_directory, JointList *joint_list) {
    fs::path vbo_path = input_directory;
    vbo_path.append("VBO");
    Asset newVBOAsset(vbo_path);
    if (!newVBOAsset.success()) {
        std::cerr << "Could not open VBO in " << input_directory << std::endl;
        return nullptr;
    }

    fs::path ibo_path = input_directory;
    ibo_path.append("IBO");
    Asset newIBOAsset(ibo_path);
    if (!newIBOAsset.success()) {
        std::cerr << "Could not open IBO in " << input_directory << std::endl;
        return nullptr;
    }

    // Create VBO and IBO
    GLuint vbo = createVBO(newVBOAsset);
    GLuint ibo = createIBO(newIBOAsset);

    SkinnedMesh *skinned_mesh = new SkinnedMesh(vbo, ibo, newIBOAsset.getBytes() / sizeof(int), joint_list);
    return skinned_mesh;
}

GLuint MeshFactory::createVBO(Asset &asset) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, asset.getBytes(), asset.getBuffer(), GL_STATIC_DRAW);
    return vbo;
}

GLuint MeshFactory::createIBO(Asset &asset) {
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, asset.getBytes(), asset.getBuffer(), GL_STATIC_DRAW);
    return ibo;
}
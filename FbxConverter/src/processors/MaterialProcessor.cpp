#include "MaterialProcessor.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "../Logger.hpp"
#include "../Util.hpp"

#define MATERIAL_FNAME "Material.txt"
#define DIFFUSE_FNAME "diffuse.txt"
#define EMISSIVE_FNAME "emissive.txt"
#define SHININESS_FNAME "shininess"

MaterialProcessor::MaterialProcessor(std::string base_dir) : optimizer(base_dir) {
    std::cout << "Initing Materials" << std::endl;
}

MaterialProcessor &MaterialProcessor::getInstance() {
    static MaterialProcessor instance("./output/Materials");
    return instance;
}

void MaterialProcessor::getMaterialsFromNode(fbxsdk::FbxNode *node, std::string node_directory) {
    std::string use_name = Util::sanitizeString(node->GetName());

    if (node->GetMaterialCount() == 0) {
        return;
    }
    if (node->GetMaterialCount() > 1) {
        Logger::error("More than one material for node " + use_name + " -- This is not implemented yet");
        return;
    }

    // Process the material and get its index
    int material_index = getMaterial(node->GetMaterial(0));
    if (material_index == -1) {
        Logger::log("No material for node " + use_name);
        return;
    }

    // Write the index to the material file for the node
    std::string material_file = node_directory + "/" + MATERIAL_FNAME;
    std::ofstream material_fstream(material_file);
    material_fstream << material_index;
    material_fstream.close();
}

int MaterialProcessor::getMaterial(fbxsdk::FbxSurfaceMaterial *material) {
    std::string use_name = Util::sanitizeString(material->GetName());
    std::cout << "Got Mat " << use_name << ": " << material << std::endl;

    // Check the optimizer first
    if (optimizer.checkExists(material)) {
        int mat_index = optimizer.getIndex(material);
        Logger::log("Material already exists - return index " + std::to_string(mat_index));
        return mat_index;
    }

    // Only implementing phong for now
    fbxsdk::FbxString material_type = material->ShadingModel.Get();
    if (strcmp(material_type.Buffer(), "Phong") != 0) {
        Logger::error("Unknown material type " + std::string(material_type));
        return -1;
    }

    std::cout << "Checking with optimizer: " << &optimizer << std::endl;
    int material_index = optimizer.addData(material);
    std::cout << "Got index " << material_index << std::endl;
    fbxsdk::FbxSurfacePhong *phong = (fbxsdk::FbxSurfacePhong *)material;

    Logger::log("Processing phong material " + use_name);
    Logger::log(" - specular:  " + std::to_string(phong->Specular.Get().mData[0]) + ", " + std::to_string(phong->Specular.Get().mData[1]) + ", " + std::to_string(phong->Specular.Get().mData[2]));
    Logger::log(" - shininess: " + std::to_string(phong->Shininess.Get()));

    // Get the texture names and write them to the appropriate files
    fbxsdk::FbxFileTexture *diffuse_source = (fbxsdk::FbxFileTexture *)phong->Diffuse.GetSrcObject();
    fbxsdk::FbxFileTexture *emissive_source = (fbxsdk::FbxFileTexture *)phong->Emissive.GetSrcObject();
    const char *diffuse_name = diffuse_source ? diffuse_source->GetFileName() : "";
    const char *emissive_name = emissive_source ? emissive_source->GetFileName() : "";
    bool has_diffuse = strlen(diffuse_name) > 0;
    bool has_emissive = strlen(emissive_name) > 0;

    Logger::log(" - diffuse:   " + std::to_string(has_diffuse) + std::string(" ") + diffuse_name);
    Logger::log(" - emissive:  " + std::to_string(has_emissive) + std::string(" ") + emissive_name);

    std::string material_directory = optimizer.getDataDirectory(material_index);
    writeTextureNameToFile(diffuse_name, material_directory + "/" + DIFFUSE_FNAME);
    writeTextureNameToFile(emissive_name, material_directory + "/" + EMISSIVE_FNAME);

    // Handle shininess
    float shininess[] = {(float)phong->Shininess.Get()};
    Logger::log(" - shininess: " + std::to_string(shininess[0]));
    std::ofstream shininess_fstream(material_directory + "/" + SHININESS_FNAME, std::ios::out | std::ios::binary);
    shininess_fstream.write((char *)shininess, sizeof(float));
    shininess_fstream.close();

    std::ifstream in(material_directory + "/" + SHININESS_FNAME, std::ios::in | std::ios::binary);
    char buffer[sizeof(float) / sizeof(char) + 1];
    in.read(buffer, sizeof(float));
    buffer[sizeof(float) / sizeof(char)] = '\0';
    in.close();

    return material_index;
}

void MaterialProcessor::writeTextureNameToFile(const char *tex_name, std::string fname) {
    // Get only the filename of the texture
    size_t slash_index = 0;
    const char *current = tex_name;
    while ((slash_index = strcspn(current, "/")) < strlen(current)) {
        current += slash_index + 1;
    }

    std::ofstream outfile(fname);
    outfile << current;
    outfile.close();
}

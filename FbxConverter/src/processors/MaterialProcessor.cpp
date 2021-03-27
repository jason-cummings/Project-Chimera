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

MaterialProcessor::MaterialProcessor(std::string baseDir) : optimizer(baseDir) {
    std::cout << "Initing Materials" << std::endl;
}

MaterialProcessor &MaterialProcessor::getInstance() {
    static MaterialProcessor instance("./output/Materials");
    return instance;
}

void MaterialProcessor::getMaterialsFromNode(fbxsdk::FbxNode *node, std::string nodeDirectory) {
    std::string useName = Util::sanitizeString(node->GetName());

    if (node->GetMaterialCount() == 0) {
        return;
    }
    if (node->GetMaterialCount() > 1) {
        Logger::error("More than one material for node " + useName + " -- This is not implemented yet");
        return;
    }

    // Process the material and get its index
    int materialIndex = getMaterial(node->GetMaterial(0));
    if (materialIndex == -1) {
        Logger::log("No material for node " + useName);
        return;
    }

    // Write the index to the material file for the node
    std::string materialFile = nodeDirectory + "/" + MATERIAL_FNAME;
    std::ofstream materialFstream(materialFile);
    materialFstream << materialIndex;
    materialFstream.close();
}

int MaterialProcessor::getMaterial(fbxsdk::FbxSurfaceMaterial *material) {
    std::string useName = Util::sanitizeString(material->GetName());
    std::cout << "Got Mat " << useName << ": " << material << std::endl;

    // Check the optimizer first
    if (optimizer.checkExists(material)) {
        int matIndex = optimizer.getIndex(material);
        Logger::log("Material already exists - return index " + std::to_string(matIndex));
        return matIndex;
    }

    // Only implementing phong for now
    fbxsdk::FbxString materialType = material->ShadingModel.Get();
    if (strcmp(materialType.Buffer(), "Phong") != 0) {
        Logger::error("Unknown material type " + std::string(materialType));
        return -1;
    }

    std::cout << "Checking with optimizer: " << &optimizer << std::endl;
    int materialIndex = optimizer.addData(material);
    std::cout << "Got index " << materialIndex << std::endl;
    fbxsdk::FbxSurfacePhong *phong = (fbxsdk::FbxSurfacePhong *)material;

    Logger::log("Processing phong material " + useName);
    Logger::log(" - specular:  " + std::to_string(phong->Specular.Get().mData[0]) + ", " + std::to_string(phong->Specular.Get().mData[1]) + ", " + std::to_string(phong->Specular.Get().mData[2]));
    Logger::log(" - shininess: " + std::to_string(phong->Shininess.Get()));

    // Get the texture names and write them to the appropriate files
    fbxsdk::FbxFileTexture *diffuseSource = (fbxsdk::FbxFileTexture *)phong->Diffuse.GetSrcObject();
    fbxsdk::FbxFileTexture *emissiveSource = (fbxsdk::FbxFileTexture *)phong->Emissive.GetSrcObject();
    const char *diffuseName = diffuseSource ? diffuseSource->GetFileName() : "";
    const char *emissiveName = emissiveSource ? emissiveSource->GetFileName() : "";
    bool hasDiffuse = strlen(diffuseName) > 0;
    bool hasEmissive = strlen(emissiveName) > 0;

    Logger::log(" - diffuse:   " + std::to_string(hasDiffuse) + std::string(" ") + diffuseName);
    Logger::log(" - emissive:  " + std::to_string(hasEmissive) + std::string(" ") + emissiveName);

    std::string materialDirectory = optimizer.getDataDirectory(materialIndex);
    writeTextureNameToFile(diffuseName, materialDirectory + "/" + DIFFUSE_FNAME);
    writeTextureNameToFile(emissiveName, materialDirectory + "/" + EMISSIVE_FNAME);

    // Handle shininess
    float shininess[] = {(float)phong->Shininess.Get()};
    Logger::log(" - shininess: " + std::to_string(shininess[0]));
    std::ofstream shininessFstream(materialDirectory + "/" + SHININESS_FNAME, std::ios::out | std::ios::binary);
    shininessFstream.write((char *)shininess, sizeof(float));
    shininessFstream.close();

    std::ifstream in(materialDirectory + "/" + SHININESS_FNAME, std::ios::in | std::ios::binary);
    char buffer[sizeof(float) / sizeof(char) + 1];
    in.read(buffer, sizeof(float));
    buffer[sizeof(float) / sizeof(char)] = '\0';
    in.close();

    return materialIndex;
}

void MaterialProcessor::writeTextureNameToFile(const char *texName, std::string fname) {
    // Get only the filename of the texture
    size_t slashIndex = 0;
    const char *current = texName;
    while ((slashIndex = strcspn(current, "/")) < strlen(current)) {
        current += slashIndex + 1;
    }

    std::ofstream outfile(fname);
    outfile << current;
    outfile.close();
}

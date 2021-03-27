#ifndef MATERIALPROCESSOR_H
#define MATERIALPROCESSOR_H

#include <fbxsdk.h>

#include "../DataOptimizer.hpp"

class MaterialProcessor {
private:
    DataOptimizer optimizer;

    /** Writes the material information out and returns the index for the node */
    int getMaterial(fbxsdk::FbxSurfaceMaterial *material);

    /** Convenience function to write out the filename of a texture */
    void writeTextureNameToFile(const char *texName, std::string fname);

    /** Singleton constructor */
    MaterialProcessor(std::string baseDir);
    MaterialProcessor(const MaterialProcessor &) = delete;
    MaterialProcessor &operator=(const MaterialProcessor &) = delete;

public:
    /** @return singleton instance */
    static MaterialProcessor &getInstance();

    /** Processes a material and adds it to the optimizer if it doesn't already exist */
    void getMaterialsFromNode(fbxsdk::FbxNode *node, std::string nodeDirectory);
};

#endif
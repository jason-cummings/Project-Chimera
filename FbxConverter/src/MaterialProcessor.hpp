#ifndef MATERIALPROCESSOR_H
#define MATERIALPROCESSOR_H

#include <fbxsdk.h>

#include "DataOptimizer.hpp"

class MaterialProcessor {
private:
    DataOptimizer optimizer;

    /** Writes the material information out and returns the index for the node */
    int getMaterial(fbxsdk::FbxSurfaceMaterial *material);

    /** Convenience function to write out the filename of a texture */
    void writeTextureNameToFile(const char *tex_name, std::string fname);

public:
    MaterialProcessor(std::string base_dir);

    /** Processes a material and adds it to the optimizer if it doesn't already exist */
    void getMaterialsFromNode(fbxsdk::FbxNode *node, std::string node_directory);
};

#endif
#include "FbxParser.hpp"

#include "FilesystemWrapper.hpp"
#include "Logger.hpp"
#include "Util.hpp"
#include "processors/AnimationProcessor.hpp"

#include <sstream>

FbxParser::FbxParser(std::string filename) : meshOptimizer("./output/Meshes"),
                                             skinnedMeshOptimizer("./output/SkinnedMeshes"),
                                             hitboxOptimizer("./output/Hitboxes") {
    fbxFilename = filename;
}

// initializes FbxManager and starts the data retrievel and export process
void FbxParser::init(bool forHitbox) {

    manager = fbxsdk::FbxManager::Create();
    if (!manager) {
        Logger::error("Error creating FBX Manager");
        exit(1);
    }

    scene = fbxsdk::FbxScene::Create(manager, "Scene");

    fbxsdk::FbxImporter *importer = fbxsdk::FbxImporter::Create(manager, "");
    const bool status = importer->Initialize(fbxFilename.c_str(), -1, manager->GetIOSettings());

    if (!status) {
        Logger::error("Error initializing importer object");
        exit(1);
    }

    if (!importer->IsFBX()) {
        Logger::error("Importer did not receive a valid FBX object");
        exit(1);
    }

    if (!importer->Import(scene)) {
        Logger::error("Scene import failed");
        exit(1);
    }
    // as we are using Maya, converting the axis system is not needed, but if needed in the future it should be done here

    // convert all geometry to triangles - OpenGL 3.3 doesn't allow quads
    fbxsdk::FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true);

    if (forHitbox) {
        // create a folder for hitboxes then process all nodes for hitboxes
        Util::createFolder("./output/Hitboxes");
        processNodesForHitbox(scene->GetRootNode(), "./output");
    } else {
        // create output folder
        Util::createFolder("./output");

        // create Mesh folder within output folder
        Util::createFolder("./output/Meshes");
        Util::createFolder("./output/SkinnedMeshes");

        //create Animation stacks folder within output folder
        Util::createFolder("./output/AnimationStacks");

        // create Materials folder within output folder
        Util::createFolder("./output/Materials");

        // get all animation layers so that they can be filled in later
        Logger::log("Animation stack info");
        Logger::stepIn();
        Logger::log("Number of animation stacks: " + std::to_string(importer->GetAnimStackCount()));
        for (int i = 0; i < importer->GetAnimStackCount(); i++) {
            fbxsdk::FbxTakeInfo *takeInfo = importer->GetTakeInfo(i);
            Logger::log("Animation Stack " + std::to_string(i) + ": " + takeInfo->mName.Buffer());

            // fbxsdk::FbxAnimStack *toProcess = fbxsdk::FbxCast<fbxsdk::FbxAnimStack>(
            //     scene->GetSrcObject(fbxsdk::FbxCriteria::ObjectType(fbxsdk::FbxAnimStack::ClassId), i));
            fbxsdk::FbxAnimStack *toProcess = scene->GetSrcObject<fbxsdk::FbxAnimStack>(i);

            AnimationProcessor &animProcessor = AnimationProcessor::getInstance();
            animProcessor.processAnimationStack(toProcess);
        }
        Logger::stepUp();

        // analyse scenegraph and creates a list of joints
        Logger::log("Processing skeleton hierarchy");
        SkeletonProcessor skeletonProcessor = SkeletonProcessor::getInstance();
        skeletonProcessor.processSkeletonHierarchy(scene->GetRootNode());

        // export joints
        Logger::log("Processing joints");
        Util::createFolder("./output/JointList");
        skeletonProcessor.exportJointList("./output/JointList");

        //process all nodes for export - this processes meshes
        Logger::log("Processing scene nodes");
        processNodes(scene->GetRootNode(), "./output");
    }

    importer->Destroy();
}

void FbxParser::processNodes(fbxsdk::FbxNode *node, std::string parentDirectory) {
    Logger::stepIn();

    // Sanitize the node name for the sake of safety
    // std::string toProcessName = Util::sanitizeString(node->GetName());
    // node->SetName(toProcessName.c_str());

    //create folder for this node
    std::string nodeDirectory = parentDirectory + "/" + Util::sanitizeString(node->GetName());
    Util::createFolder(nodeDirectory);

    // Write transformation information
    writeNodeTranslationInformtion(node, nodeDirectory);

    // check if any animations affect this node
    AnimationProcessor &animProcessor = AnimationProcessor::getInstance();
    animProcessor.processNodeForAnimation(node);

    // check for a mesh and process and export it
    fbxsdk::FbxNodeAttribute *attribute = node->GetNodeAttribute();

    if (attribute) {
        Logger::log("- Attribute: " + std::to_string((int)attribute->GetAttributeType()));
        if (attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh) {
            Logger::log("- Node has mesh");
            fbxsdk::FbxMesh *mesh = node->GetMesh();
            Logger::log("- Number of Polygons: " + std::to_string(mesh->GetPolygonCount()));
            // check to see if this is a skinned mesh
            Logger::log("- Mesh has " + std::to_string(mesh->GetDeformerCount()) + std::string(" deformers"));
            if (mesh->GetDeformerCount() > 0) {
                Logger::log(" - Processing deformers for this mesh");
                SkeletonProcessor skeletonProcessor = SkeletonProcessor::getInstance();
                std::vector<ControlPointBoneWeights> temp = skeletonProcessor.processDeformers(node);
                processSkinnedMesh(mesh, nodeDirectory, temp);
            } else
                processMesh(mesh, nodeDirectory);
        }

        // if this object is a bone, export a "bone" file. this informs projectchimera that the node is a bone
        if (attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton) {
            std::ofstream boneFile(nodeDirectory + "/bone");
            boneFile << "1";
            boneFile.close();
        }
    }

    MaterialProcessor &materialProcessor = MaterialProcessor::getInstance();
    materialProcessor.getMaterialsFromNode(node, nodeDirectory);

    // create folder for children

    const int childCount = node->GetChildCount();
    if (childCount > 0) {
        std::string childrenDirectory = nodeDirectory + "/children";
        Util::createFolder(childrenDirectory);

        for (int i = 0; i < childCount; i++) {
            processNodes(node->GetChild(i), childrenDirectory);
        }
    }

    Logger::stepUp();
}

void FbxParser::writeNodeTranslationInformtion(fbxsdk::FbxNode *node, std::string nodeDirectory) {
    // get transformation data and convert to glm
    fbxsdk::FbxDouble3 translation = node->LclTranslation.Get();
    fbxsdk::FbxDouble3 rotation = node->LclRotation.Get();
    fbxsdk::FbxDouble3 scaling = node->LclScaling.Get();

    fbxsdk::FbxVector4 postTargetRotation = node->GetTargetUpVector();

    glm::vec3 translationVector = glm::vec3(translation[0], translation[1], translation[2]);
    glm::vec3 rotationVector = glm::vec3(rotation[0] + postTargetRotation[0],
                                         rotation[1] + postTargetRotation[1],
                                         rotation[2] + postTargetRotation[2]);
    glm::vec3 scalingVector = glm::vec3(scaling[0], scaling[1], scaling[2]);

    // prints node information, useful for Logger::log and verifying that fbx file has expected values
    Logger::log("Node: " + Util::sanitizeString(node->GetName()));
    Logger::log(" - translation: " + std::to_string(translation[0]) + ", " + std::to_string(translation[1]) + ", " + std::to_string(translation[2]));
    Logger::log(" - rotation:    " + std::to_string(rotation[0]) + ", " + std::to_string(rotation[1]) + ", " + std::to_string(rotation[2]));
    Logger::log(" - rotation2:   " + std::to_string(rotationVector[0]) + ", " + std::to_string(rotationVector[1]) + ", " + std::to_string(rotationVector[2]));
    Logger::log(" - scaling:     " + std::to_string(scaling[0]) + ", " + std::to_string(scaling[1]) + ", " + std::to_string(scaling[2]));

    //export transform data

    std::ofstream translationFile(nodeDirectory + "/translation", std::ios::out | std::ios::binary);
    translationFile.write((const char *)&translationVector[0], sizeof(glm::vec3));
    translationFile.close();

    std::ofstream rotationFile(nodeDirectory + "/rotation", std::ios::out | std::ios::binary);
    rotationFile.write((const char *)&rotationVector[0], sizeof(glm::vec3));
    rotationFile.close();

    std::ofstream scalingFile(nodeDirectory + "/scaling", std::ios::out | std::ios::binary);
    scalingFile.write((const char *)&scalingVector[0], sizeof(glm::vec3));
    scalingFile.close();
}

//=============================================Read Meshes===========================================

// process a mesh. uses the data optimizer to check if it has already been processed.
// if not, then all control points are read and a MeshExporter is used
void FbxParser::processMesh(fbxsdk::FbxMesh *mesh, std::string parentDirectory) {
    int meshIndex = 0;
    if (meshOptimizer.checkExists(mesh)) {
        Logger::log(" - Mesh exists: " + std::to_string(meshOptimizer.getIndex(mesh)));
        meshIndex = meshOptimizer.getIndex(mesh);

    } else {

        meshIndex = meshOptimizer.addData(mesh);
        MeshExporter<Vertex> me;

        int numPolygons = mesh->GetPolygonCount();
        const fbxsdk::FbxVector4 *controlPoints = mesh->GetControlPoints();
        Logger::log(" - control point count: " + std::to_string(mesh->GetControlPointsCount()));
        for (int i = 0; i < numPolygons; i++) {
            for (int j = 0; j < 3; j++) {
                int vertexIndex = mesh->GetPolygonVertex(i, j);

                glm::vec4 position = glm::vec4(static_cast<float>(controlPoints[vertexIndex][0]), static_cast<float>(controlPoints[vertexIndex][1]), static_cast<float>(controlPoints[vertexIndex][2]), 1.0);
                glm::vec3 normal = getNormal(mesh, vertexIndex, i * 3 + j);
                glm::vec2 uv = getUV(mesh, vertexIndex, i * 3 + j);

                Vertex v = Vertex();
                v.position = position;
                v.normal = normal;
                v.uv = uv;

                me.addVertex(v);
            }
        }

        me.exportMesh(meshOptimizer.getDataDirectory(meshIndex));
    }

    // add file to node folder for meshIndex
    std::ofstream file;
    file.open(parentDirectory + "/Mesh.txt");
    file << meshIndex;
    file.close();
}

// same as processing a mesh, but adds data for skeletal animation
void FbxParser::processSkinnedMesh(fbxsdk::FbxMesh *mesh, std::string parentDirectory, std::vector<ControlPointBoneWeights> &boneWeights) {
    int meshIndex = 0;
    if (skinnedMeshOptimizer.checkExists(mesh)) {
        Logger::log(" - Mesh exists: " + std::to_string(skinnedMeshOptimizer.getIndex(mesh)));
        meshIndex = skinnedMeshOptimizer.getIndex(mesh);

    } else {

        meshIndex = skinnedMeshOptimizer.addData(mesh);
        MeshExporter<SkinnedVertex> me;

        int numPolygons = mesh->GetPolygonCount();
        const fbxsdk::FbxVector4 *controlPoints = mesh->GetControlPoints();
        Logger::log(" - control point count: " + std::to_string(mesh->GetControlPointsCount()));
        for (int i = 0; i < numPolygons; i++) {
            for (int j = 0; j < 3; j++) {
                int vertexIndex = mesh->GetPolygonVertex(i, j);
                glm::vec4 position = glm::vec4(static_cast<float>(controlPoints[vertexIndex][0]), static_cast<float>(controlPoints[vertexIndex][1]), static_cast<float>(controlPoints[vertexIndex][2]), 1.0);
                glm::vec3 normal = getNormal(mesh, vertexIndex, i * 3 + j);
                glm::vec2 uv = getUV(mesh, vertexIndex, i * 3 + j);
                SkinnedVertex v = SkinnedVertex();
                v.position = position;
                v.normal = normal;
                v.uv = uv;

                // fill weight array of vertex then populate with data
                for (int i = 0; i < 22; i++) {
                    v.weightArray[i] = 0.0f;
                }
                for (int i = 0; i < 4; i++) {
                    v.weightArray[boneWeights[vertexIndex].indexes[i]] = boneWeights[vertexIndex].weights[i];
                }

                me.addVertex(v);
            }
        }

        me.exportMesh(skinnedMeshOptimizer.getDataDirectory(meshIndex));
    }

    // add file to node folder for meshIndex
    std::ofstream file;
    file.open(parentDirectory + "/SkinnedMesh.txt");
    file << meshIndex;
    file.close();
}

// helper funtion to get normal of a vertex
glm::vec3 FbxParser::getNormal(fbxsdk::FbxMesh *mesh, int controlPointIndex, int vertexIndex) {
    FbxGeometryElementNormal *vertexNormal = mesh->GetElementNormal(0);

    float x = -1, y = -1, z = -1;

    switch (vertexNormal->GetMappingMode()) {
    case FbxGeometryElement::eByControlPoint:
        switch (vertexNormal->GetReferenceMode()) {
        case FbxGeometryElement::eDirect: {
            x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
            y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
            z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
            break;
        }

        case FbxGeometryElement::eIndexToDirect: {
            int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
            x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
            y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
            z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
            break;
        }

        default:
            throw std::runtime_error("Error with Normal Referencing mode");
        }
        break;
    case FbxGeometryElement::eByPolygonVertex:
        switch (vertexNormal->GetReferenceMode()) {
        case FbxGeometryElement::eDirect: {
            x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexIndex).mData[0]);
            y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexIndex).mData[1]);
            z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexIndex).mData[2]);
            break;
        }
        case FbxGeometryElement::eIndexToDirect: {
            int index = vertexNormal->GetIndexArray().GetAt(vertexIndex);
            x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
            y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
            z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
            break;
        }
        default:
            throw std::runtime_error(std::string("Error with Normal Referencing mode"));
        }
        break;
    default:
        throw std::runtime_error("Error with Normal Mapping Mode");
    }

    return glm::vec3(x, y, z);
}

// helper function to get UV data of a vertex
glm::vec2 FbxParser::getUV(fbxsdk::FbxMesh *mesh, int controlPointIndex, int vertexIndex) {
    FbxGeometryElementUV *vertexUv = mesh->GetElementUV(0);

    if (mesh->GetElementUVCount() < 1) {
        std::runtime_error("Mesh has not been UV unwrapped");
    }

    float u = -1, v = -1;

    switch (vertexUv->GetMappingMode()) {
    case FbxGeometryElement::eByControlPoint:
        switch (vertexUv->GetReferenceMode()) {
        case FbxGeometryElement::eDirect: {
            u = static_cast<float>(vertexUv->GetDirectArray().GetAt(controlPointIndex).mData[0]);
            v = static_cast<float>(vertexUv->GetDirectArray().GetAt(controlPointIndex).mData[1]);
            break;
        }

        case FbxGeometryElement::eIndexToDirect: {
            int index = vertexUv->GetIndexArray().GetAt(controlPointIndex);
            u = static_cast<float>(vertexUv->GetDirectArray().GetAt(index).mData[0]);
            v = static_cast<float>(vertexUv->GetDirectArray().GetAt(index).mData[1]);
            break;
        }

        default:
            throw std::runtime_error("Error with UV Referencing mode");
        }
        break;
    case FbxGeometryElement::eByPolygonVertex:
        switch (vertexUv->GetReferenceMode()) {
        case FbxGeometryElement::eDirect: {
            u = static_cast<float>(vertexUv->GetDirectArray().GetAt(vertexIndex).mData[0]);
            v = static_cast<float>(vertexUv->GetDirectArray().GetAt(vertexIndex).mData[1]);
            break;
        }
        case FbxGeometryElement::eIndexToDirect: {
            int index = vertexUv->GetIndexArray().GetAt(vertexIndex);
            u = static_cast<float>(vertexUv->GetDirectArray().GetAt(index).mData[0]);
            v = static_cast<float>(vertexUv->GetDirectArray().GetAt(index).mData[1]);
            break;
        }
        default:
            throw std::runtime_error(std::string("Error with UV Referencing mode"));
        }
        break;
    default:
        throw std::runtime_error("Error with UV Mapping Mode");
    }

    return glm::vec2(u, v);
}

// =================================================For hitboxes===================================

// processes nodes for hitboxes. this means only processing for meshes, and only adds hitboxes to existing node folders.
// this function will not create folders for the nodes, they must already be existing.
void FbxParser::processNodesForHitbox(fbxsdk::FbxNode *node, std::string parentDirectory) {

    std::string useName = Util::sanitizeString(node->GetName());
    std::string nodeDirectory = parentDirectory + "/" + useName;
    const int childCount = node->GetChildCount();

    Logger::log("Node: " + useName);

    // Create directory and write translation info if necessary
    if (!fs::exists(nodeDirectory)) {
        Util::createFolder(nodeDirectory);
        writeNodeTranslationInformtion(node, nodeDirectory);
    }

    fbxsdk::FbxNodeAttribute *attribute = node->GetNodeAttribute();
    if (attribute) {
        if (attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh) {
            Logger::log(" - Node has mesh");
            fbxsdk::FbxMesh *mesh = node->GetMesh();
            processMeshForHitbox(mesh, nodeDirectory);
        }
    }

    std::string childrenDirectory = nodeDirectory + "/children";

    for (int i = 0; i < childCount; i++) {
        processNodesForHitbox(node->GetChild(i), childrenDirectory);
    }
}

// processes a mesh for hitboxes, which means only exporting vertex position data, and no rendering data.
void FbxParser::processMeshForHitbox(fbxsdk::FbxMesh *mesh, std::string nodeDirectory) {

    int meshIndex = 0;
    if (hitboxOptimizer.checkExists(mesh)) {
        Logger::log("Mesh exists: " + std::to_string(hitboxOptimizer.getIndex(mesh)));
        meshIndex = hitboxOptimizer.getIndex(mesh);

    } else {

        meshIndex = hitboxOptimizer.addData(mesh);
        MeshExporter<MinimalVertex> me;

        int numPolygons = mesh->GetPolygonCount();
        const fbxsdk::FbxVector4 *controlPoints = mesh->GetControlPoints();
        Logger::log("control point count: " + std::to_string(mesh->GetControlPointsCount()));
        for (int i = 0; i < numPolygons; i++) {
            for (int j = 0; j < 3; j++) {
                int vertexIndex = mesh->GetPolygonVertex(i, j);
                glm::vec4 position = glm::vec4(static_cast<float>(controlPoints[vertexIndex][0]), static_cast<float>(controlPoints[vertexIndex][1]), static_cast<float>(controlPoints[vertexIndex][2]), 1.0);

                MinimalVertex mv;
                mv.position = glm::vec3(position);

                me.addVertex(mv);
            }
        }

        me.exportMesh(hitboxOptimizer.getDataDirectory(meshIndex));
    }

    // add file to node folder for meshIndex
    std::ofstream file;
    file.open(nodeDirectory + "/Hitbox.txt");
    file << meshIndex;
    file.close();
}

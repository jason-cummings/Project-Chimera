#include "FbxParser.hpp"

#include "Logger.hpp"
#include "Util.hpp"
#include "processors/AnimationProcessor.hpp"

#include <sstream>

FbxParser::FbxParser(std::string filename) : mesh_optimizer("./output/Meshes"),
                                             skinned_mesh_optimizer("./output/SkinnedMeshes"),
                                             hitbox_optimizer("./output/Hitboxes") {
    fbx_filename = filename;
}

// initializes FbxManager and starts the data retrievel and export process
void FbxParser::init(bool for_hitbox) {

    manager = fbxsdk::FbxManager::Create();
    if (!manager) {
        Logger::error("Error creating FBX Manager");
        exit(1);
    }

    scene = fbxsdk::FbxScene::Create(manager, "Scene");

    fbxsdk::FbxImporter *importer = fbxsdk::FbxImporter::Create(manager, "");
    const bool status = importer->Initialize(fbx_filename.c_str(), -1, manager->GetIOSettings());

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
    fbxsdk::FbxGeometryConverter geometry_converter(manager);
    geometry_converter.Triangulate(scene, true);

    if (for_hitbox) {
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
            fbxsdk::FbxTakeInfo *take_info = importer->GetTakeInfo(i);
            Logger::log("Animation Stack " + std::to_string(i) + ": " + take_info->mName.Buffer());

            // fbxsdk::FbxAnimStack *toProcess = fbxsdk::FbxCast<fbxsdk::FbxAnimStack>(
            //     scene->GetSrcObject(fbxsdk::FbxCriteria::ObjectType(fbxsdk::FbxAnimStack::ClassId), i));
            fbxsdk::FbxAnimStack *toProcess = scene->GetSrcObject<fbxsdk::FbxAnimStack>(i);

            AnimationProcessor &anim_processor = AnimationProcessor::getInstance();
            anim_processor.processAnimationStack(toProcess);
        }
        Logger::stepUp();

        // analyse scenegraph and creates a list of joints
        Logger::log("Processing skeleton hierarchy");
        SkeletonProcessor skeleton_processor = SkeletonProcessor::getInstance();
        skeleton_processor.processSkeletonHierarchy(scene->GetRootNode());

        // export joints
        Logger::log("Processing joints");
        Util::createFolder("./output/JointList");
        skeleton_processor.exportJointList("./output/JointList");

        //process all nodes for export - this processes meshes
        Logger::log("Processing scene nodes");
        processNodes(scene->GetRootNode(), "./output");
    }

    importer->Destroy();
}

void FbxParser::processNodes(fbxsdk::FbxNode *node, std::string parent_directory) {
    Logger::stepIn();

    // Sanitize the node name for the sake of safety
    // std::string to_process_name = Util::sanitizeString(node->GetName());
    // node->SetName(to_process_name.c_str());

    //create folder for this node
    std::string node_directory = parent_directory + "/" + Util::sanitizeString(node->GetName());
    Util::createFolder(node_directory);

    // Write transformation information
    writeNodeTranslationInformtion(node, node_directory);

    // check if any animations affect this node
    AnimationProcessor &anim_processor = AnimationProcessor::getInstance();
    anim_processor.processNodeForAnimation(node);

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
                SkeletonProcessor skeleton_processor = SkeletonProcessor::getInstance();
                std::vector<ControlPointBoneWeights> temp = skeleton_processor.processDeformers(node);
                processSkinnedMesh(mesh, node_directory, temp);
            } else
                processMesh(mesh, node_directory);
        }

        // if this object is a bone, export a "bone" file. this informs projectchimera that the node is a bone
        if (attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton) {
            std::ofstream boneFile(node_directory + "/bone");
            boneFile << "1";
            boneFile.close();
        }
    }

    MaterialProcessor &material_processor = MaterialProcessor::getInstance();
    material_processor.getMaterialsFromNode(node, node_directory);

    // create folder for children

    const int child_count = node->GetChildCount();
    if (child_count > 0) {
        std::string children_directory = node_directory + "/children";
        Util::createFolder(children_directory);

        for (int i = 0; i < child_count; i++) {
            processNodes(node->GetChild(i), children_directory);
        }
    }

    Logger::stepUp();
}

void FbxParser::writeNodeTranslationInformtion(fbxsdk::FbxNode *node, std::string node_directory) {
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

    std::ofstream translationFile(node_directory + "/translation", std::ios::out | std::ios::binary);
    translationFile.write((const char *)&translationVector[0], sizeof(glm::vec3));
    translationFile.close();

    std::ofstream rotationFile(node_directory + "/rotation", std::ios::out | std::ios::binary);
    rotationFile.write((const char *)&rotationVector[0], sizeof(glm::vec3));
    rotationFile.close();

    std::ofstream scalingFile(node_directory + "/scaling", std::ios::out | std::ios::binary);
    scalingFile.write((const char *)&scalingVector[0], sizeof(glm::vec3));
    scalingFile.close();
}

//=============================================Read Meshes===========================================

// process a mesh. uses the data optimizer to check if it has already been processed.
// if not, then all control points are read and a MeshExporter is used
void FbxParser::processMesh(fbxsdk::FbxMesh *mesh, std::string parent_directory) {
    int mesh_index = 0;
    if (mesh_optimizer.checkExists(mesh)) {
        Logger::log(" - Mesh exists: " + std::to_string(mesh_optimizer.getIndex(mesh)));
        mesh_index = mesh_optimizer.getIndex(mesh);

    } else {

        mesh_index = mesh_optimizer.addData(mesh);
        MeshExporter<Vertex> me;

        int num_polygons = mesh->GetPolygonCount();
        const fbxsdk::FbxVector4 *control_points = mesh->GetControlPoints();
        Logger::log(" - control point count: " + std::to_string(mesh->GetControlPointsCount()));
        for (int i = 0; i < num_polygons; i++) {
            for (int j = 0; j < 3; j++) {
                int vertex_index = mesh->GetPolygonVertex(i, j);

                glm::vec4 position = glm::vec4(static_cast<float>(control_points[vertex_index][0]), static_cast<float>(control_points[vertex_index][1]), static_cast<float>(control_points[vertex_index][2]), 1.0);
                glm::vec3 normal = getNormal(mesh, vertex_index, i * 3 + j);
                glm::vec2 uv = getUV(mesh, vertex_index, i * 3 + j);

                Vertex v = Vertex();
                v.position = position;
                v.normal = normal;
                v.uv = uv;

                me.addVertex(v);
            }
        }

        me.exportMesh(mesh_optimizer.getDataDirectory(mesh_index));
    }

    // add file to node folder for mesh_index
    std::ofstream file;
    file.open(parent_directory + "/Mesh.txt");
    file << mesh_index;
    file.close();
}

// same as processing a mesh, but adds data for skeletal animation
void FbxParser::processSkinnedMesh(fbxsdk::FbxMesh *mesh, std::string parent_directory, std::vector<ControlPointBoneWeights> &bone_weights) {
    int mesh_index = 0;
    if (skinned_mesh_optimizer.checkExists(mesh)) {
        Logger::log(" - Mesh exists: " + std::to_string(skinned_mesh_optimizer.getIndex(mesh)));
        mesh_index = skinned_mesh_optimizer.getIndex(mesh);

    } else {

        mesh_index = skinned_mesh_optimizer.addData(mesh);
        MeshExporter<SkinnedVertex> me;

        int num_polygons = mesh->GetPolygonCount();
        const fbxsdk::FbxVector4 *control_points = mesh->GetControlPoints();
        Logger::log(" - control point count: " + std::to_string(mesh->GetControlPointsCount()));
        for (int i = 0; i < num_polygons; i++) {
            for (int j = 0; j < 3; j++) {
                int vertex_index = mesh->GetPolygonVertex(i, j);
                glm::vec4 position = glm::vec4(static_cast<float>(control_points[vertex_index][0]), static_cast<float>(control_points[vertex_index][1]), static_cast<float>(control_points[vertex_index][2]), 1.0);
                glm::vec3 normal = getNormal(mesh, vertex_index, i * 3 + j);
                glm::vec2 uv = getUV(mesh, vertex_index, i * 3 + j);
                SkinnedVertex v = SkinnedVertex();
                v.position = position;
                v.normal = normal;
                v.uv = uv;

                // fill weight array of vertex then populate with data
                for (int i = 0; i < 22; i++) {
                    v.weight_array[i] = 0.0f;
                }
                for (int i = 0; i < 4; i++) {
                    v.weight_array[bone_weights[vertex_index].indexes[i]] = bone_weights[vertex_index].weights[i];
                }

                me.addVertex(v);
            }
        }

        me.exportMesh(skinned_mesh_optimizer.getDataDirectory(mesh_index));
    }

    // add file to node folder for mesh_index
    std::ofstream file;
    file.open(parent_directory + "/SkinnedMesh.txt");
    file << mesh_index;
    file.close();
}

// helper funtion to get normal of a vertex
glm::vec3 FbxParser::getNormal(fbxsdk::FbxMesh *mesh, int control_point_index, int vertex_index) {
    FbxGeometryElementNormal *vertex_normal = mesh->GetElementNormal(0);

    float x = -1, y = -1, z = -1;

    switch (vertex_normal->GetMappingMode()) {
    case FbxGeometryElement::eByControlPoint:
        switch (vertex_normal->GetReferenceMode()) {
        case FbxGeometryElement::eDirect: {
            x = static_cast<float>(vertex_normal->GetDirectArray().GetAt(control_point_index).mData[0]);
            y = static_cast<float>(vertex_normal->GetDirectArray().GetAt(control_point_index).mData[1]);
            z = static_cast<float>(vertex_normal->GetDirectArray().GetAt(control_point_index).mData[2]);
            break;
        }

        case FbxGeometryElement::eIndexToDirect: {
            int index = vertex_normal->GetIndexArray().GetAt(control_point_index);
            x = static_cast<float>(vertex_normal->GetDirectArray().GetAt(index).mData[0]);
            y = static_cast<float>(vertex_normal->GetDirectArray().GetAt(index).mData[1]);
            z = static_cast<float>(vertex_normal->GetDirectArray().GetAt(index).mData[2]);
            break;
        }

        default:
            throw std::runtime_error("Error with Normal Referencing mode");
        }
        break;
    case FbxGeometryElement::eByPolygonVertex:
        switch (vertex_normal->GetReferenceMode()) {
        case FbxGeometryElement::eDirect: {
            x = static_cast<float>(vertex_normal->GetDirectArray().GetAt(vertex_index).mData[0]);
            y = static_cast<float>(vertex_normal->GetDirectArray().GetAt(vertex_index).mData[1]);
            z = static_cast<float>(vertex_normal->GetDirectArray().GetAt(vertex_index).mData[2]);
            break;
        }
        case FbxGeometryElement::eIndexToDirect: {
            int index = vertex_normal->GetIndexArray().GetAt(vertex_index);
            x = static_cast<float>(vertex_normal->GetDirectArray().GetAt(index).mData[0]);
            y = static_cast<float>(vertex_normal->GetDirectArray().GetAt(index).mData[1]);
            z = static_cast<float>(vertex_normal->GetDirectArray().GetAt(index).mData[2]);
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
glm::vec2 FbxParser::getUV(fbxsdk::FbxMesh *mesh, int control_point_index, int vertex_index) {
    FbxGeometryElementUV *vertex_uv = mesh->GetElementUV(0);

    if (mesh->GetElementUVCount() < 1) {
        std::runtime_error("Mesh has not been UV unwrapped");
    }

    float u = -1, v = -1;

    switch (vertex_uv->GetMappingMode()) {
    case FbxGeometryElement::eByControlPoint:
        switch (vertex_uv->GetReferenceMode()) {
        case FbxGeometryElement::eDirect: {
            u = static_cast<float>(vertex_uv->GetDirectArray().GetAt(control_point_index).mData[0]);
            v = static_cast<float>(vertex_uv->GetDirectArray().GetAt(control_point_index).mData[1]);
            break;
        }

        case FbxGeometryElement::eIndexToDirect: {
            int index = vertex_uv->GetIndexArray().GetAt(control_point_index);
            u = static_cast<float>(vertex_uv->GetDirectArray().GetAt(index).mData[0]);
            v = static_cast<float>(vertex_uv->GetDirectArray().GetAt(index).mData[1]);
            break;
        }

        default:
            throw std::runtime_error("Error with UV Referencing mode");
        }
        break;
    case FbxGeometryElement::eByPolygonVertex:
        switch (vertex_uv->GetReferenceMode()) {
        case FbxGeometryElement::eDirect: {
            u = static_cast<float>(vertex_uv->GetDirectArray().GetAt(vertex_index).mData[0]);
            v = static_cast<float>(vertex_uv->GetDirectArray().GetAt(vertex_index).mData[1]);
            break;
        }
        case FbxGeometryElement::eIndexToDirect: {
            int index = vertex_uv->GetIndexArray().GetAt(vertex_index);
            u = static_cast<float>(vertex_uv->GetDirectArray().GetAt(index).mData[0]);
            v = static_cast<float>(vertex_uv->GetDirectArray().GetAt(index).mData[1]);
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
void FbxParser::processNodesForHitbox(fbxsdk::FbxNode *node, std::string parent_directory) {

    std::string use_name = Util::sanitizeString(node->GetName());
    std::string node_directory = parent_directory + "/" + use_name;
    const int child_count = node->GetChildCount();

    Logger::log("Node: " + use_name);

    // Create directory and write translation info if necessary
    if (!fs::exists(node_directory)) {
        Util::createFolder(node_directory);
        writeNodeTranslationInformtion(node, node_directory);
    }

    fbxsdk::FbxNodeAttribute *attribute = node->GetNodeAttribute();
    if (attribute) {
        if (attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh) {
            Logger::log(" - Node has mesh");
            fbxsdk::FbxMesh *mesh = node->GetMesh();
            processMeshForHitbox(mesh, node_directory);
        }
    }

    std::string children_directory = node_directory + "/children";

    for (int i = 0; i < child_count; i++) {
        processNodesForHitbox(node->GetChild(i), children_directory);
    }
}

// processes a mesh for hitboxes, which means only exporting vertex position data, and no rendering data.
void FbxParser::processMeshForHitbox(fbxsdk::FbxMesh *mesh, std::string node_directory) {

    int mesh_index = 0;
    if (hitbox_optimizer.checkExists(mesh)) {
        Logger::log("Mesh exists: " + std::to_string(hitbox_optimizer.getIndex(mesh)));
        mesh_index = hitbox_optimizer.getIndex(mesh);

    } else {

        mesh_index = hitbox_optimizer.addData(mesh);
        MeshExporter<MinimalVertex> me;

        int num_polygons = mesh->GetPolygonCount();
        const fbxsdk::FbxVector4 *control_points = mesh->GetControlPoints();
        Logger::log("control point count: " + std::to_string(mesh->GetControlPointsCount()));
        for (int i = 0; i < num_polygons; i++) {
            for (int j = 0; j < 3; j++) {
                int vertex_index = mesh->GetPolygonVertex(i, j);
                glm::vec4 position = glm::vec4(static_cast<float>(control_points[vertex_index][0]), static_cast<float>(control_points[vertex_index][1]), static_cast<float>(control_points[vertex_index][2]), 1.0);

                MinimalVertex mv;
                mv.position = glm::vec3(position);

                me.addVertex(mv);
            }
        }

        me.exportMesh(hitbox_optimizer.getDataDirectory(mesh_index));
    }

    // add file to node folder for mesh_index
    std::ofstream file;
    file.open(node_directory + "/Hitbox.txt");
    file << mesh_index;
    file.close();
}

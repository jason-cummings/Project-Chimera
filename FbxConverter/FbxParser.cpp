#include "FbxParser.h"

// build
// g++ -I/Applications/Autodesk/FBX\ SDK/2020.0.1/include  -L/Applications/Autodesk/FBX\ SDK/2020.0.1/lib/clang/release/ -lfbxsdk FbxParser.cpp

FbxParser::FbxParser(std::string filename) {
	fbx_filename = filename;
}

void FbxParser::init(bool for_hitbox) {
	manager = FbxManager::Create();
	if(!manager) {
		std::cout << "Error creating FBX Manager" << std::endl;
		exit(1);
	}

	scene = FbxScene::Create(manager, "Scene");

	FbxImporter * importer = FbxImporter::Create(manager, "");
	const bool status = importer->Initialize(fbx_filename.c_str(), -1, manager->GetIOSettings());\

	if(!status) {
		std::cout << "Error initializing importer object" << std::endl;
		exit(1);
	}

	if(importer->IsFBX()) {

		bool import_status = importer->Import(scene);

		if(import_status) {

			// might need to convert axis system here - skipping for now during initial setup

			//convert all geometry to triangles - OpenGL 3.3 doesn't allow quads
			FbxGeometryConverter geometry_converter(manager);
			geometry_converter.Triangulate(scene, true);
		}		
	}

	mesh_optimizer = DataOptimizer("output/Meshes");
	skinned_mesh_optimizer = DataOptimizer("output/SkinnedMeshes");
	material_optimizer = DataOptimizer("output/Materials");
	hitbox_optimizer = DataOptimizer("output/Hitboxes");

	if(for_hitbox) {
		createFolder("./output/Hitboxes");
		processNodesForHitbox(scene->GetRootNode(), "", "./output");
	}
	else {
		// create output folder
		createFolder("./output");

		// create Mesh folder within output folder
		createFolder("./output/Meshes");
		createFolder("./output/SkinnedMeshes");

		createFolder("./output/AnimationStacks");

		// create Materials folder within output folder

		// get all animation layers so that they can be filled in later
		std::cout << "Animation stack info" << std::endl;
		std::cout << "Number of animation stacks: " << importer->GetAnimStackCount();

		for(int i = 0; i < importer->GetAnimStackCount(); i++) {
			FbxTakeInfo *take_info = importer->GetTakeInfo(i);
			std::cout << "Animation Stack " << i << ": " << take_info->mName.Buffer() << std::endl;
			processAnimationStack(FbxCast<FbxAnimStack>(scene->GetSrcObject(FbxCriteria::ObjectType(FbxAnimStack::ClassId), i)));
		}

		skeleton_processor.processSkeletonHierarchy(scene->GetRootNode());

		createFolder("./output/JointList");
		skeleton_processor.exportJointList("./output/JointList");

		processNodes(scene->GetRootNode(), "", "./output");
	}

	importer->Destroy();

	
}

void FbxParser::processNodes(FbxNode * node, std::string depth, std::string parent_directory) {

	//create folder for this node
	std::string node_directory = parent_directory + "/" + node->GetName();
	createFolder(node_directory);


	const int child_count = node->GetChildCount();

	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	FbxVector4 postTargetRotation = node->GetTargetUpVector();

	glm::vec3 translationVector = glm::vec3(translation[0],translation[1],translation[2]);
	glm::vec3 rotationVector = glm::vec3(rotation[0] + postTargetRotation[0],
										 rotation[1] + postTargetRotation[1],
										 rotation[2] + postTargetRotation[2]);
	glm::vec3 scalingVector = glm::vec3(scaling[0],scaling[1],scaling[2]);


	std::cout << depth << "Node: " << node->GetName() << std::endl;
	std::cout << depth << " - translation: " << translation[0] << ", " << translation[1] << ", " << translation[2] << std::endl;
	std::cout << depth << " - rotation: " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << std::endl;
	std::cout << depth << " - rotation2: " << rotationVector[0] << ", " << rotationVector[1] << ", " << rotationVector[2] << std::endl;
	std::cout << depth << " - scaling: " << scaling[0] << ", " << scaling[1] << ", " << scaling[2] << std::endl;

	const FbxVector4 lT = node->GetGeometricTranslation(FbxNode::eSourcePivot); 
    const FbxVector4 lR = node->GetGeometricRotation(FbxNode::eSourcePivot); 
    const FbxVector4 lS = node->GetGeometricScaling(FbxNode::eSourcePivot); 

    std::cout << depth << " - lT: " << lT[0] << ", " << lT[1] << ", " << lT[2] << ", " << lT[3] << std::endl;
    std::cout << depth << " - lR: " << lR[0] << ", " << lR[1] << ", " << lR[2] << ", " << lR[3] << std::endl;
    std::cout << depth << " - lS: " << lS[0] << ", " << lS[1] << ", " << lS[2] << ", " << lS[3] << std::endl;

	//store transform data

	std::ofstream translationFile (node_directory + "/translation", std::ios::out | std::ios::binary);
	translationFile.write((const char *)&translationVector[0],sizeof(glm::vec3));
	translationFile.close();

	std::ofstream rotationFile (node_directory + "/rotation", std::ios::out | std::ios::binary);
	rotationFile.write((const char *)&rotationVector[0],sizeof(glm::vec3));
	rotationFile.close();

	std::ofstream scalingFile (node_directory + "/scaling", std::ios::out | std::ios::binary);
	scalingFile.write((const char *)&scalingVector[0],sizeof(glm::vec3));
	scalingFile.close();



	processNodeForAnimation(node);

	FbxNodeAttribute * attribute = node->GetNodeAttribute();

	if(attribute) {
		if(attribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			std::cout << depth << " - Node has mesh" << std::endl;
			FbxMesh * mesh = node->GetMesh();
			std::cout << depth << " - Number of Polygons: " << mesh->GetPolygonCount() << std::endl;
			// check to see if this is a skinned mesh
			std::cout << depth << " - Mesh has " << mesh->GetDeformerCount() << " deformers" << std::endl;
			if(mesh->GetDeformerCount() > 0) {
				std::cout << depth << " - Processing deformers for this mesh" << std::endl;
				std::vector<ControlPointBoneWeights> temp = skeleton_processor.processDeformers(node);
				processSkinnedMesh(mesh,node_directory,temp);
			}
			else processMesh(mesh,node_directory);
		}

		if(attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton) {
			std::ofstream boneFile(node_directory + "/bone");
			boneFile << "1";
			boneFile.close();
		}
	}

	// create folder for children
	std::string children_directory = node_directory + "/children";
	createFolder(children_directory);

	for (int i = 0; i < child_count; i++) {
		processNodes(node->GetChild(i), depth + "    ",children_directory);
	}
}






//=============================================Read Meshes===========================================






void FbxParser::processMesh(FbxMesh * mesh, std::string parent_directory) {
	int mesh_index = 0;
	if(mesh_optimizer.checkExists(mesh)) {
		std::cout << "Mesh exists: " << mesh_optimizer.getIndex(mesh) << std::endl;
		mesh_index = mesh_optimizer.getIndex(mesh);

	}
	else {

		mesh_index = mesh_optimizer.addData(mesh);
		MeshExporter<Vertex> me;

		int num_polygons = mesh->GetPolygonCount();
		const FbxVector4 * control_points = mesh->GetControlPoints();
		std::cout << "control point count: " << mesh->GetControlPointsCount() << std::endl;
		for(int i = 0; i < num_polygons; i++) {
			for(int j = 0; j < 3; j++) {
				int vertex_index = mesh->GetPolygonVertex(i,j);
				glm::vec4 position = glm::vec4(static_cast<float>(control_points[vertex_index][0]), static_cast<float>(control_points[vertex_index][1]), static_cast<float>(control_points[vertex_index][2]),1.0);
				glm::vec3 normal = getNormal(mesh, vertex_index, i*3 + j);
				glm::vec2 uv = getUV(mesh, vertex_index, i*3 + j);
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


void FbxParser::processSkinnedMesh(FbxMesh * mesh, std::string parent_directory,std::vector<ControlPointBoneWeights>& bone_weights) {
	int mesh_index = 0;
	if(skinned_mesh_optimizer.checkExists(mesh)) {
		std::cout << "Mesh exists: " << skinned_mesh_optimizer.getIndex(mesh) << std::endl;
		mesh_index = skinned_mesh_optimizer.getIndex(mesh);

	}
	else {

		mesh_index = skinned_mesh_optimizer.addData(mesh);
		MeshExporter<SkinnedVertex> me;

		int num_polygons = mesh->GetPolygonCount();
		const FbxVector4 * control_points = mesh->GetControlPoints();
		std::cout << "control point count: " << mesh->GetControlPointsCount() << std::endl;
		for(int i = 0; i < num_polygons; i++) {
			for(int j = 0; j < 3; j++) {
				int vertex_index = mesh->GetPolygonVertex(i,j);
				glm::vec4 position = glm::vec4(static_cast<float>(control_points[vertex_index][0]), static_cast<float>(control_points[vertex_index][1]), static_cast<float>(control_points[vertex_index][2]),1.0);
				glm::vec3 normal = getNormal(mesh, vertex_index, i*3 + j);
				glm::vec2 uv = getUV(mesh, vertex_index, i*3 + j);
				SkinnedVertex v = SkinnedVertex();
				v.position = position;
				v.normal = normal;
				v.uv = uv;
				// v.joint_index_0 = bone_weights[vertex_index].indexes[0];
				// v.joint_weight_0 = bone_weights[vertex_index].weights[0];

				// v.joint_index_1 = bone_weights[vertex_index].indexes[1];
				// v.joint_weight_1 = bone_weights[vertex_index].weights[1];

				// v.joint_index_2 = bone_weights[vertex_index].indexes[2];
				// v.joint_weight_2 = bone_weights[vertex_index].weights[2];

				// v.joint_index_3 = bone_weights[vertex_index].indexes[3];
				// v.joint_weight_3 = bone_weights[vertex_index].weights[3];

				for(int i = 0; i < 22; i++) {
					v.weight_array[i] = 0.0f;
				}
				for(int i = 0; i < 4; i++) {
					v.weight_array[bone_weights[vertex_index].indexes[i]] = bone_weights[vertex_index].weights[i];
				}
				// int max = 0;
				// float max_val = 0;
				// for(int i = 0; i < 4; i++) {
				// 	if(bone_weights[vertex_index].weights[i] > max_val) {
				// 		max = i;
				// 		max_val = bone_weights[vertex_index].weights[i];
				// 	}
				// }

				// v.weight_array[bone_weights[vertex_index].indexes[max]] = 1.0f;
				
				std::cout << "Vertex: (" << position[0] << ", " << position[1] << ", " << position[2] << ")" <<std::endl;
				std::cout << " - index: " << bone_weights[vertex_index].indexes[0] << ", " <<bone_weights[vertex_index].weights[0] <<std::endl;
				std::cout << "   index: " << bone_weights[vertex_index].indexes[1] << ", " <<bone_weights[vertex_index].weights[1] <<std::endl;
				std::cout << "   index: " << bone_weights[vertex_index].indexes[2] << ", " <<bone_weights[vertex_index].weights[2] <<std::endl;
				std::cout << "   index: " << bone_weights[vertex_index].indexes[3] << ", " <<bone_weights[vertex_index].weights[3] <<std::endl;

				// std::cout << " - index: " << v.joint_index_0 << ", " << v.joint_weight_0 <<std::endl;

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



glm::vec3 FbxParser::getNormal(FbxMesh * mesh, int control_point_index, int vertex_index) {
	FbxGeometryElementNormal * vertex_normal = mesh->GetElementNormal(0);

	float x = -1,y = -1,z = -1;

	switch(vertex_normal->GetMappingMode()) {
		case FbxGeometryElement::eByControlPoint: 
			switch(vertex_normal->GetReferenceMode()) {
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
				
				//case FbxGeometryElement::eIndex:

				default:
					throw std::runtime_error("Error with Normal Referencing mode");
			}
			break;
		case FbxGeometryElement::eByPolygonVertex:
			switch(vertex_normal->GetReferenceMode()) {
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

	//std::cout << "Normal: " << x << ", " << y << ", " << z << std::endl;
	return glm::vec3(x,y,z);
}

glm::vec2 FbxParser::getUV(FbxMesh * mesh,  int control_point_index, int vertex_index) {
	FbxGeometryElementUV * vertex_uv = mesh->GetElementUV(0);


	if(mesh->GetElementUVCount() < 1) {
		std::runtime_error("Mesh has not been UV unwrapped");
	}

	float u = -1, v = -1;

	switch(vertex_uv->GetMappingMode()) {
		case FbxGeometryElement::eByControlPoint: 
			switch(vertex_uv->GetReferenceMode()) {
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
				
				//case FbxGeometryElement::eIndex:

				default:
					throw std::runtime_error("Error with UV Referencing mode");
			}
			break;
		case FbxGeometryElement::eByPolygonVertex:
			switch(vertex_uv->GetReferenceMode()) {
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

	//std::cout << "UV: " << u << ", " << v << std::endl;
	return glm::vec2(u,v);
}






// =============================================Processing Animations===================================






void FbxParser::processAnimationStack(FbxAnimStack * animation_stack) {
	createFolder(std::string("./output/AnimationStacks/") + animation_stack->GetName());
	int num_layers = animation_stack->GetMemberCount(FbxCriteria::ObjectType(FbxAnimLayer::ClassId));

	std::cout << "layers: " << num_layers << std::endl;

	//create folders for all layers in this animation stack and add them and their directories to 
	// lists to use later when processing nodes

	for(int layer_i = 0; layer_i < num_layers; layer_i++) {
		FbxAnimLayer* layer = FbxCast<FbxAnimLayer>(animation_stack->GetMember(FbxCriteria::ObjectType(FbxAnimLayer::ClassId), layer_i));

		std::string layer_dir = std::string("./output/AnimationStacks/") + animation_stack->GetName() + std::string("/") + layer->GetName();
		createFolder(layer_dir);

		animations.push_back(layer);
		animation_directories.push_back(layer_dir);


	}
}

void FbxParser::processNodeForAnimation(FbxNode * node) {
	for(int i = 0; i < animations.size(); i++) {
		// go through all animations, and check for animation curves that affect this node
		FbxAnimCurve* pos_x_curve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_X);
		FbxAnimCurve* pos_y_curve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Y);
		FbxAnimCurve* pos_z_curve = node->LclTranslation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Z);

		saveKeyframes(node, pos_x_curve, pos_y_curve, pos_z_curve, i, "translation");

		FbxAnimCurve* rot_x_curve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_X);
		FbxAnimCurve* rot_y_curve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Y);
		FbxAnimCurve* rot_z_curve = node->LclRotation.GetCurve(animations[i], FBXSDK_CURVENODE_COMPONENT_Z);

		saveKeyframes(node, rot_x_curve, rot_y_curve, rot_z_curve, i, "rotation");

		
	}
}

void FbxParser::saveKeyframes(FbxNode * node, FbxAnimCurve* x_curve, FbxAnimCurve* y_curve, FbxAnimCurve* z_curve, int animation_index, std::string filename) {
	if(x_curve && y_curve && z_curve) {
		// std::cout << "X Keys: " << x_curve->KeyGetCount() << std::endl;
		// std::cout << "Y Keys: " << y_curve->KeyGetCount() << std::endl;
		// std::cout << "Z Keys: " << z_curve->KeyGetCount() << std::endl;
	
		std::vector<Keyframe> key_list;
		//get values from all keys
		for(int k = 0; k < x_curve->KeyGetCount(); k++) {
			FbxAnimCurveKey xkey = x_curve->KeyGet(k);
			FbxAnimCurveKey ykey = y_curve->KeyGet(k);
			FbxAnimCurveKey zkey = z_curve->KeyGet(k);

			if(!(xkey.GetTime().GetSecondDouble() == ykey.GetTime().GetSecondDouble()
				&& xkey.GetTime().GetSecondDouble() == zkey.GetTime().GetSecondDouble()))
				std::cout << "====================================================\n\n\n\n\n\n" << 
					"Times not equal for all keyframes. \n" <<
					"This will cause artifacts in animations due to innacurate values" << std::endl;

			Keyframe key = Keyframe();
			key.time = xkey.GetTime().GetSecondDouble();
			key.value[0] = xkey.GetValue();
			key.value[1] = ykey.GetValue();
			key.value[2] = zkey.GetValue();

			key_list.push_back(key);
		}

		//save keys to file

		std::string animation_curve_dir = animation_directories[animation_index] + "/" + node->GetName();
		createFolder(animation_curve_dir);
		std::ofstream key_file(animation_curve_dir + "/" + filename, std::ios::out | std::ios::binary);
		key_file.write((const char *)&key_list[0],key_list.size() * sizeof(Keyframe));
		key_file.close();
	}
}






// =================================================For hitboxes===================================





void FbxParser::processNodesForHitbox(FbxNode * node, std::string depth, std::string parent_directory) {

	std::string node_directory = parent_directory + "/" + node->GetName();


	const int child_count = node->GetChildCount();


	std::cout << depth << "Node: " << node->GetName() << std::endl;

	FbxNodeAttribute * attribute = node->GetNodeAttribute();
	if(attribute) {
		if(attribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			std::cout << depth << " - Node has mesh" << std::endl;
			FbxMesh * mesh = node->GetMesh();
			processMeshForHitbox(mesh,node_directory);
		}
	}

	std::string children_directory = node_directory + "/children";

	for (int i = 0; i < child_count; i++) {
		processNodesForHitbox(node->GetChild(i), depth + "    ",children_directory);
	}
}



void FbxParser::processMeshForHitbox(FbxMesh * mesh, std::string parent_directory) {

	int mesh_index = 0;
	if(hitbox_optimizer.checkExists(mesh)) {
		std::cout << "Mesh exists: " << hitbox_optimizer.getIndex(mesh) << std::endl;
		mesh_index = hitbox_optimizer.getIndex(mesh);

	}
	else {

		mesh_index = hitbox_optimizer.addData(mesh);
		MeshExporter<MinimalVertex> me;

		int num_polygons = mesh->GetPolygonCount();
		const FbxVector4 * control_points = mesh->GetControlPoints();
		std::cout << "control point count: " << mesh->GetControlPointsCount() << std::endl;
		for(int i = 0; i < num_polygons; i++) {
			for(int j = 0; j < 3; j++) {
				int vertex_index = mesh->GetPolygonVertex(i,j);
				glm::vec4 position = glm::vec4(static_cast<float>(control_points[vertex_index][0]), static_cast<float>(control_points[vertex_index][1]), static_cast<float>(control_points[vertex_index][2]),1.0);

				MinimalVertex mv;
				mv.position = glm::vec3(position);

				me.addVertex(mv);

				
			}
		}

		me.exportMesh(hitbox_optimizer.getDataDirectory(mesh_index));

	}

	// add file to node folder for mesh_index
	std::ofstream file;
	file.open(parent_directory + "/Hitbox.txt");
	file << mesh_index;
	file.close();
}





int main(int argc, char ** argv) {
	std::string fbx_filename = "test.fbx";
	bool for_hitbox = false;
	if(argc > 1) {
		fbx_filename = std::string(argv[1]);
	}
	if(argc > 2 && std::string(argv[2]).compare("-h") == 0)
	{
		for_hitbox = true;
	}
	FbxParser p = FbxParser(fbx_filename);
	p.init(for_hitbox);
	return 0;
}
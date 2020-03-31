#include "FBXParser.h"

// build
// g++ -I/Applications/Autodesk/FBX\ SDK/2020.0.1/include  -L/Applications/Autodesk/FBX\ SDK/2020.0.1/lib/clang/release/ -lfbxsdk FbxParser.cpp

FbxParser::FbxParser(std::string filename) {
	fbx_filename = filename;
}

void FbxParser::init() {
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
		std::cout << "Animation stack info - not used for now" << std::endl;
		std::cout << "Number of animation stacks: " << importer->GetAnimStackCount();

		for(int i = 0; i < importer->GetAnimStackCount(); i++) {
			FbxTakeInfo *take_info = importer->GetTakeInfo(i);
			std::cout << "Animation Stack " << i << ": " << take_info->mName.Buffer() << std::endl;
		}

		bool import_status = importer->Import(scene);

		if(import_status) {

			// might need to convert axis system here - skipping for now during initial setup

			//convert all geometry to triangles - OpenGL 3.3 doesn't allow quads
			FbxGeometryConverter geometry_converter(manager);
			geometry_converter.Triangulate(scene, true);


		}
	}

	// create output folder
	createFolder("./output");

	// create Mesh folder within output folder
	createFolder("./output/Meshes");

	// create Materials folder within output folder

	mesh_optimizer = DataOptimizer("output/Meshes");
	material_optimizer = DataOptimizer("output/Materials");

	processNodes(scene->GetRootNode(), "", "./output");

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

	glm::vec4 translationVector = glm::vec4(translation[0],translation[1],translation[2],1.0);
	glm::vec3 rotationVector = glm::vec3(rotation[0],rotation[1],rotation[2]);
	glm::vec3 scalingVector = glm::vec3(scaling[0],scaling[1],scaling[2]);


	std::cout << depth << "Node: " << node->GetName() << std::endl;
	std::cout << depth << " - translation: " << translation[0] << ", " << translation[1] << ", " << translation[2] << std::endl;
	std::cout << depth << " - rotation: " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << std::endl;
	std::cout << depth << " - scaling: " << scaling[0] << ", " << scaling[1] << ", " << scaling[2] << std::endl;

	//store transform data

	std::ofstream translationFile (node_directory + "/translation", std::ios::out | std::ios::binary);
	translationFile.write((const char *)&translationVector[0],sizeof(glm::vec4));
	translationFile.close();

	std::ofstream rotationFile (node_directory + "/rotation", std::ios::out | std::ios::binary);
	rotationFile.write((const char *)&rotationVector[0],sizeof(glm::vec3));
	rotationFile.close();

	std::ofstream scalingFile (node_directory + "/scaling", std::ios::out | std::ios::binary);
	scalingFile.write((const char *)&scalingVector[0],sizeof(glm::vec3));
	scalingFile.close();


	FbxNodeAttribute * attribute = node->GetNodeAttribute();
	if(attribute) {
		if(attribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			std::cout << depth << " - Node has mesh" << std::endl;
			FbxMesh * mesh = node->GetMesh();
			std::cout << depth << " - Number of Polygons: " << mesh->GetPolygonCount() << std::endl;
			processMesh(mesh,node_directory);
		}
	}

	// create folder for children
	std::string children_directory = node_directory + "/children";
	createFolder(children_directory);

	for (int i = 0; i < child_count; i++) {
		processNodes(node->GetChild(i), depth + "    ",children_directory);
	}
}
FbxMesh * last = NULL;
void FbxParser::processMesh(FbxMesh * mesh, std::string parent_directory) {
	
	std::cout << "Pointer: " << mesh << std::endl;
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
				//std::cout << /*static_cast<float>(controlPoints[i][0])*/mesh->GetPolygonVertex(i,j) << ": " << static_cast<float>(control_points[vertex_index][0]) << ", " << static_cast<float>(control_points[vertex_index][1]) << ", " << static_cast<float>(control_points[vertex_index][2]) << std::endl;
				glm::vec4 position = glm::vec4(static_cast<float>(control_points[vertex_index][0]), static_cast<float>(control_points[vertex_index][1]), static_cast<float>(control_points[vertex_index][2]),1.0);
				glm::vec3 normal = getNormal(mesh, vertex_index, i*3 + j);
				glm::vec2 uv = getUV(mesh, vertex_index, i*3 + j);
				Vertex v = Vertex();
				v.position = position;
				v.normal = normal;
				v.uv = uv;

				// MinimalVertex mv;
				// mv.position = glm::vec3(position);

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
	

	//std::cout << me << std::endl;
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


int main() {
	FbxParser p = FbxParser("test.fbx");
	p.init();
	return 0;
}
#include "LoadAnimation.hpp"

std::vector<std::string> splitDatas(const std::string &datas, const char spliter) {

	std::istringstream ss{datas};
    std::string currentData;
    std::vector<std::string> splited;

    while (std::getline(ss, currentData, spliter)) { splited.push_back(currentData); }

    return splited;
}

bool loadVertex(std::shared_ptr<Gg::Component::AnimatedMesh> mesh, tinyxml2::XMLElement *vertexDataNode) {

	const char *datasRAW{vertexDataNode->GetText()};
	if(datasRAW == nullptr) {

		std::cout << "Error with vertex datas." << std::endl;
		return false;
	}

	std::string datas{datasRAW};

	std::vector<std::string> splitedDatas{splitDatas(datas, ' ')};

	mesh->m_vertexPosition.resize(splitedDatas.size()/3);

	for(size_t i{0}; i < splitedDatas.size(); i += 3) { 

		mesh->m_vertexPosition[i/3] = glm::vec3{std::stof(splitedDatas[i]), std::stof(splitedDatas[i + 1]), std::stof(splitedDatas[i + 2])};
	}


	return true;
}

bool loadTriangleNormalTextureCoords(std::shared_ptr<Gg::Component::AnimatedMesh> mesh, 
									 tinyxml2::XMLElement *normalDataNode,
									 tinyxml2::XMLElement *textureDataNode,
									 tinyxml2::XMLElement *triangleDataNode) {


	//Normals

	const char *datasNormalRAW{normalDataNode->GetText()};
	if(datasNormalRAW == nullptr) {

		std::cout << "Error with normal datas." << std::endl;
		return false;
	}

	std::string datasNormal{datasNormalRAW};

	std::vector<std::string> splitedNormalDatas{splitDatas(datasNormal, ' ')};
	std::vector<glm::vec3> normals;
	normals.resize(splitedNormalDatas.size()/3);

	for(size_t i{0}; i < splitedNormalDatas.size(); i += 3) { 

		normals[i/3] = glm::vec3{std::stof(splitedNormalDatas[i]), std::stof(splitedNormalDatas[i + 1]), std::stof(splitedNormalDatas[i + 2])}; 
	}

	//Textures coords

	const char *datasTextureRAW{textureDataNode->GetText()};
	if(datasTextureRAW == nullptr) {

		std::cout << "Error with normal datas." << std::endl;
		return false;
	}

	std::string datasTexture{datasTextureRAW};

	std::vector<std::string> splitedTextureDatas{splitDatas(datasTexture, ' ')};
	std::vector<glm::vec3> textureCoords;
	textureCoords.resize(splitedTextureDatas.size()/2);

	for(size_t i{0}; i < splitedTextureDatas.size(); i += 2) { 

		textureCoords[i/2] = glm::vec3{std::stof(splitedTextureDatas[i]), std::stof(splitedTextureDatas[i + 1]), 0.f}; 
	}

	//Triangle & co

	const char *datasTriangleRAW{triangleDataNode->GetText()};

	if(datasTriangleRAW == nullptr) {
		std::cout << "Error with triangle datas." << std::endl;
		return false;
	}

	std::string datasTriangle{datasTriangleRAW};

	std::vector<std::string> splitedTriangleDatas{splitDatas(datasTriangle, ' ')};

	for(size_t i{0}; i < splitedTriangleDatas.size(); i += 3) { 

		mesh->m_vertexIndice.emplace_back(std::stof(splitedTriangleDatas[i]));
		mesh->m_vertexNormal.emplace_back(normals[std::stoi(splitedTriangleDatas[i + 1])]);
		//mesh->m_vertexColor.emplace_back(textureCoords[std::stoi(splitedTriangleDatas[i + 2])]);
		mesh->m_vertexColor.emplace_back(glm::vec3{0.21, 0.21, 0.21});
	}

	return true;
}

bool loadWeights(std::shared_ptr<Gg::Component::AnimatedMesh> mesh, 
									 tinyxml2::XMLElement *weightDataNode,
									 tinyxml2::XMLElement *bonePerVerticeNode,
									 tinyxml2::XMLElement *bonesAndWeightDatasNode) {


	//Weight

	const char *datasWeightRAW{weightDataNode->GetText()};
	if(datasWeightRAW == nullptr) {

		std::cout << "Error with weight datas." << std::endl;
		return false;
	}

	std::string datasWeight{datasWeightRAW};

	std::vector<std::string> splitedWeightDatas{splitDatas(datasWeight, ' ')};
	std::vector<float> weight;
	weight.resize(splitedWeightDatas.size());

	for(size_t i{0}; i < splitedWeightDatas.size(); i++) { weight[i] = std::stof(splitedWeightDatas[i]); } 

	//Nb Bone per vertice

	const char *datasNbBonesRAW{bonePerVerticeNode->GetText()};
	if(datasNbBonesRAW == nullptr) {

		std::cout << "Error with weight datas." << std::endl;
		return false;
	}

	std::string datasNbBones{datasNbBonesRAW};

	std::vector<std::string> splitedNbBonesDatas{splitDatas(datasNbBones, ' ')};
	std::vector<float> nbBones;
	nbBones.resize(splitedNbBonesDatas.size());

	for(size_t i{0}; i < splitedNbBonesDatas.size(); i++) { nbBones[i] = std::stoi(splitedNbBonesDatas[i]); } 

	//Bones and weight

	const char *datasBonesAndWeightRAW{bonesAndWeightDatasNode->GetText()};
	if(datasBonesAndWeightRAW == nullptr) {

		std::cout << "Error with weight datas." << std::endl;
		return false;
	}

	std::string datasBonesAndWeight{datasBonesAndWeightRAW};

	std::vector<std::string> splitedBonesAndWeightDatas{splitDatas(datasBonesAndWeight, ' ')};
	std::vector<int> bonesAndWeight;
	bonesAndWeight.resize(splitedBonesAndWeightDatas.size());

	for(size_t i{0}; i < splitedBonesAndWeightDatas.size(); i++) { bonesAndWeight[i] = std::stoi(splitedBonesAndWeightDatas[i]); }


	//Combining for final result
	int currentOffset{0};
	unsigned int currentNbBones{0}, vertexNbBones{0};

	mesh->m_vertexBones.resize(mesh->m_vertexPosition.size());
	mesh->m_vertexWeight.resize(mesh->m_vertexPosition.size());
	for(unsigned int currentVertex{0}; currentVertex < nbBones.size(); currentVertex++) {

		vertexNbBones = nbBones[currentVertex];
		currentNbBones = 0;

		for(unsigned int currentBone{0}; currentBone < vertexNbBones || currentNbBones >= 3; currentBone++) {

			mesh->m_vertexBones[currentVertex][currentNbBones] = bonesAndWeight[currentOffset];
			mesh->m_vertexWeight[currentVertex][currentNbBones] = weight[bonesAndWeight[currentOffset + 1]];

			currentOffset += 2;
			currentNbBones++;
		}

		//if not 3 bones, complete the vec3

		for(unsigned int i{currentNbBones}; i < 3; i++) {

			mesh->m_vertexBones[currentVertex][i] = -1;
			mesh->m_vertexWeight[currentVertex][i] = 0.f;
		}
	}

	return true;
}

bool loadMesh(std::shared_ptr<Gg::Component::AnimatedMesh> mesh, tinyxml2::XMLDocument &document) {

	tinyxml2::XMLElement *root{document.RootElement()};

	if(root == nullptr) {

		std::cout << "Can't find root element." << std::endl;
		return false;
	}

	const char *rootName{root->Name()};

	if(strcmp(rootName, "COLLADA") != 0) {

		std::cout << "Can't find COLLADA element." << std::endl;
		return false;
	}

	tinyxml2::XMLElement *libGeometryNode{root->FirstChildElement("library_geometries")};
	if(libGeometryNode == nullptr) {

		std::cout << "Can't find element \"library_geometries\"." << std::endl;
		return false;
	}

	tinyxml2::XMLElement *geometryNode{libGeometryNode->FirstChildElement("geometry")};
	if(geometryNode == nullptr) {

		std::cout << "Can't find element \"geometry\"." << std::endl;
		return false;
	}

	tinyxml2::XMLElement *meshNode{geometryNode->FirstChildElement("mesh")};
	if(meshNode == nullptr) {

		std::cout << "Can't find element \"mesh\"." << std::endl;
	
		return false;
	}

	tinyxml2::XMLElement *currentNode{meshNode->FirstChildElement("source")},
						 *vertexNode{nullptr},
						 *normalNode{nullptr},
						 *textureNode{nullptr},
						 *triangleNode{nullptr};;
	
	while(currentNode != nullptr) {

		if(currentNode->Attribute("id", "rambo_239-mesh-positions")) { vertexNode = currentNode; }
		if(currentNode->Attribute("id", "rambo_239-mesh-normals")) { normalNode = currentNode; }
		if(currentNode->Attribute("id", "rambo_239-mesh-map-0")) { textureNode = currentNode; }

		currentNode = currentNode->NextSiblingElement("source");
	}

	triangleNode = meshNode->FirstChildElement("triangles");

	if(vertexNode == nullptr || normalNode == nullptr || textureNode == nullptr || triangleNode == nullptr) {

		std::cout << "Problem with data structures" << std::endl;
		return false;
	}

	tinyxml2::XMLElement *vertexDataNode{vertexNode->FirstChildElement("float_array")},
						 *normalDataNode{normalNode->FirstChildElement("float_array")},
						 *textureDataNode{textureNode->FirstChildElement("float_array")},
						 *triangleDataNode{triangleNode->FirstChildElement("p")};

	if(!loadVertex(mesh, vertexDataNode)) {

		std::cout << "Problem with vertex datas." << std::endl;
		return false;
	}

	if(!loadTriangleNormalTextureCoords(mesh, normalDataNode, textureDataNode, triangleDataNode)) {

		std::cout << "Problem with triangles datas." << std::endl;
		return false;
	}

	// Bones and weight

	tinyxml2::XMLElement *libControllersNode{root->FirstChildElement("library_controllers")};
	if(libControllersNode == nullptr) {

		std::cout << "Can't find element \"library_controllers\"." << std::endl;
		return false;
	}

	tinyxml2::XMLElement *controllerNode{libControllersNode->FirstChildElement("controller")};
	if(controllerNode == nullptr) {

		std::cout << "Can't find element \"controller\"." << std::endl;
		return false;
	}

	tinyxml2::XMLElement *skinNode{controllerNode->FirstChildElement("skin")};
	if(skinNode == nullptr) {

		std::cout << "Can't find element \"skin\"." << std::endl;
	
		return false;
	}

	currentNode = skinNode->FirstChildElement("source");

	tinyxml2::XMLElement *weightNode{nullptr},
						 *boneAndWeightNode{nullptr};
	
	while(currentNode != nullptr) {

		if(currentNode->Attribute("id", "Armature_rambo-skin-weights")) { weightNode = currentNode; }

		currentNode = currentNode->NextSiblingElement("source");
	}

	boneAndWeightNode = skinNode->FirstChildElement("vertex_weights");

	if(weightNode == nullptr || boneAndWeightNode == nullptr) {

		std::cout << "Problem with data structures" << std::endl;
		return false;
	}

	tinyxml2::XMLElement *weightDataNode{weightNode->FirstChildElement("float_array")},
						 *bonePerVerticeNode{boneAndWeightNode->FirstChildElement("vcount")},
						 *bonesAndWeightDatasNode{boneAndWeightNode->FirstChildElement("v")};

	if(!loadWeights(mesh, weightDataNode, bonePerVerticeNode, bonesAndWeightDatasNode)) {

		std::cout << "Problem with weight datas." << std::endl;
		return false;
	}

	return true;
}

bool loadAnimation(Gg::GulgEngine &engine, const Gg::Entity entity, const std::string path) { 

	tinyxml2::XMLDocument file;
	tinyxml2::XMLError result{file.LoadFile(path.c_str())};

	if(result != tinyxml2::XML_SUCCESS) { 

		std::cout <<  "Cant open file \"" << path << "\"." << std::endl;
		return false;
	}


	std::shared_ptr<Gg::Component::AnimatedMesh> mesh{std::make_shared<Gg::Component::AnimatedMesh>(engine.getProgram("AnimationProgram"), engine.getTexture("RamboTexture"))};
    
	if(!loadMesh(mesh, file)) {
		std::cout << "Error with file \"" << path << "\"." << std::endl;
		return false;
	}

	else {

		engine.addComponentToEntity(entity, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(mesh));
		mesh->reshape();
	}

	return true; 
}
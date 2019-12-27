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
									 std::vector<tinyxml2::XMLElement*> triangleDataNodes) {


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

	for(tinyxml2::XMLElement *currentDatas: triangleDataNodes) {

		const char *datasTriangleRAW{currentDatas->GetText()};
		if(datasTriangleRAW == nullptr) {

			std::cout << "Error with triangle datas." << std::endl;
			return false;
		}

		std::string datasTriangle{datasTriangleRAW};

		std::vector<std::string> splitedTriangleDatas{splitDatas(datasTriangle, ' ')};


		for(size_t i{0}; i < splitedTriangleDatas.size(); i += 3) { 

			mesh->m_vertexIndice.emplace_back(std::stof(splitedTriangleDatas[i]));
			mesh->m_vertexNormal.emplace_back(normals[std::stoi(splitedTriangleDatas[i + 1])]);
			mesh->m_vertexColor.emplace_back(textureCoords[std::stoi(splitedTriangleDatas[i + 2])]);
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
						 *triangleNode{nullptr};
	
	while(currentNode != nullptr) {

		if(currentNode->Attribute("id", "rambo_head-mesh-positions")) { vertexNode = currentNode; }
		if(currentNode->Attribute("id", "rambo_head-mesh-normals")) { normalNode = currentNode; }
		if(currentNode->Attribute("id", "rambo_head-mesh-map-0")) { textureNode = currentNode; }

		currentNode = currentNode->NextSiblingElement("source");
	}

	if(vertexNode == nullptr || normalNode == nullptr || textureNode == nullptr) {

		std::cout << "Problem with data structures" << std::endl;
		return false;
	}

	tinyxml2::XMLElement *vertexDataNode{vertexNode->FirstChildElement("float_array")},
						 *normalDataNode{normalNode->FirstChildElement("float_array")},
						 *textureDataNode{textureNode->FirstChildElement("float_array")};;

	std::vector<tinyxml2::XMLElement*> trianglesDataNodes;

	currentNode = meshNode->FirstChildElement("triangles");

	while(currentNode != nullptr) {

		trianglesDataNodes.emplace_back(currentNode->FirstChildElement("p"));
		currentNode = currentNode->NextSiblingElement("triangles");
	}


	if(!loadVertex(mesh, vertexDataNode)) {

		std::cout << "Problem with datas." << std::endl;
		return false;
	}

	if(!loadTriangleNormalTextureCoords(mesh, normalDataNode, textureDataNode, trianglesDataNodes)) {

		std::cout << "Problem with datas." << std::endl;
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
    engine.addComponentToEntity(entity, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(mesh));

	if(!loadMesh(mesh, file)) {
		std::cout << "Error with file \"" << path << "\"." << std::endl;
		return false;
	}

	mesh->reshape();

	return true; 
}
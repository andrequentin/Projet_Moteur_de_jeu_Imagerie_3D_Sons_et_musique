#include "LoadAnimation.hpp"

bool loadAnimation(Gg::GulgEngine &engine, const Gg::Entity entity, const std::string path) { 


	tinyxml2::XMLDocument file;
	tinyxml2::XMLError result{file.LoadFile(path.c_str())};

	if(result != tinyxml2::XML_SUCCESS) { 

		std::cout <<  "Cant open file \"" << path << "\"." << std::endl;
		return false;
	}

	

	return true; 
}
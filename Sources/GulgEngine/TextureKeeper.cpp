#include "GulgEngine/TextureKeeper.hpp"	

namespace Gg {

TextureKeeper::TextureKeeper() {}

bool TextureKeeper::loadTexture(const std::string path, const std::string name) {

	if(!isLoadedTexture(name)) {

		std::vector<unsigned char> image;
	    unsigned int width, height;
	    unsigned int error{lodepng::decode(image, width, height, path.c_str())};

        if(error != 0) { 

        	const char* errorString = lodepng_error_text(error);
        	std::cout << "Error while loading png file \"" << path << "\": " << errorString << std::endl;
        	
        	return false;
        }

		GLuint textureID;
        glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
		glGenerateMipmap(GL_TEXTURE_2D);

		m_textures.insert(std::make_pair(name, textureID));
	}


	return true;
}

bool TextureKeeper::isLoadedTexture(const std::string name) const {

	if(m_textures.find(name) != m_textures.end()) { return true; }
	return false;
}

GLuint TextureKeeper::getTexture(const std::string name) const {

	if(isLoadedTexture(name)) { return m_textures.at(name); }

	std::cout << "Gulg error: asked for texture \"" + name + "\", which isn't loaded." << std::endl;
	return 0;
}

}
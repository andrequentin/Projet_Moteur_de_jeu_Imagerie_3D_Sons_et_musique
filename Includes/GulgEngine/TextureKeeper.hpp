#ifndef TEXTURE_KEEPER_HPP
#define TEXTURE_KEEPER_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#include <GLFW/glfw3.h>

#include "lodepng.h"

namespace Gg {

class TextureKeeper {

	public:

		TextureKeeper();

		bool loadTexture(const std::string path, const std::string name);
		bool isLoadedTexture(const std::string name) const;
		GLuint getTexture(const std::string name) const;

	private:
                              
		std::map<std::string, GLuint> m_textures;
};

}

#endif
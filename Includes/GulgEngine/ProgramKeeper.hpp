#ifndef PROGRAM_KEEPER_HPP
#define PROGRAM_KEEPER_HPP

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

namespace Gg {

class ProgramKeeper {

	public:

		ProgramKeeper();

		bool loadProgram(const std::string vertexPath, const std::string fragmentPath, const std::string name);

		bool isLoadedProgram(const std::string name) const;
		GLuint getProgram(const std::string name) const;

	private:
                              
		std::map<std::string, GLuint> m_programs;
};

}

#endif
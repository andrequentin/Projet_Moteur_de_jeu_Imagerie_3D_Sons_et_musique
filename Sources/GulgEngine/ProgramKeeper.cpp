#include "GulgEngine/ProgramKeeper.hpp"	

namespace Gg {

ProgramKeeper::ProgramKeeper() {}

bool ProgramKeeper::loadProgram(const std::string vertexPath, const std::string fragmentPath, const std::string name) {

	if(!isLoadedProgram(name)) {

		GLuint newProgram{glCreateProgram()};
	    GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)}, fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};

	    std::string vertexCode;
	    std::ifstream vertexStream{vertexPath, std::ios::in};
	    if(vertexStream.is_open()){

		    std::stringstream sstr;
	        sstr << vertexStream.rdbuf();
	        vertexCode = sstr.str();
	        vertexStream.close();

		} 

	    else {

			std::cout << "Gulg error: Can't open shader file \"" << vertexPath << ", program can't compile." << std::endl; 
			return false;
		}

	    std::string fragmentCode;
	    std::ifstream fragmentStream{fragmentPath, std::ios::in};
	    if(fragmentStream.is_open()){

	        std::stringstream sstr;
	        sstr << fragmentStream.rdbuf();
	        fragmentCode = sstr.str();
	        fragmentStream.close();

	    } 

	    else {

	        std::cout << "Gulg error: Can't open shader file \"" << fragmentPath << ", program can't compile." << std::endl; 
	        return false;
	    }


		// Compile Vertex Shader

	    char const * vertexCodePointer = vertexCode.c_str();
	    glShaderSource(vertexShader, 1, &vertexCodePointer, nullptr);
	    glCompileShader(vertexShader);

	    GLint compiledSuccessfully{0};

	    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledSuccessfully);
	    if(compiledSuccessfully == GL_FALSE)  {

	        GLint maxLength{0};
	        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
	        std::vector<GLchar> errorLog;
	        errorLog.resize(maxLength);
	        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

	        std::cout << "Gulg error: Failed to compile vertex shader: " << compiledSuccessfully;
	        for(GLchar currentChar: errorLog) { std::cout << currentChar; }
	        std::cout << std::endl;

	        return false;
	    }



		// Compile Fragment Shader

	    char const * fragmentCodePointer = fragmentCode.c_str();
	    glShaderSource(fragmentShader, 1, &fragmentCodePointer, nullptr);
	    glCompileShader(fragmentShader);

		compiledSuccessfully = 0;

	    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledSuccessfully);
	    if(compiledSuccessfully == GL_FALSE)  {

	        GLint maxLength{0};
	        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
	        std::vector<GLchar> errorLog;
	        errorLog.resize(maxLength);
	        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

	        std::cout << "Gulg error: Failed to compile fragment shader: " << compiledSuccessfully;
	        for(GLchar currentChar: errorLog) { std::cout << currentChar; }
	        std::cout << std::endl;

	        return false;
	    }


	    // Link the program
	    glAttachShader(newProgram, vertexShader);
	    glAttachShader(newProgram, fragmentShader);

	    glLinkProgram(newProgram);

	    compiledSuccessfully = 0;

	    glGetProgramiv(newProgram, GL_LINK_STATUS, &compiledSuccessfully);
	    if (compiledSuccessfully == GL_FALSE) {

	        GLint maxLength{0};
	        glGetProgramiv(newProgram, GL_INFO_LOG_LENGTH, &maxLength);
	        std::vector<GLchar> errorLog;
	        errorLog.resize(maxLength);
	        glGetProgramInfoLog(newProgram, maxLength, &maxLength, &errorLog[0]);
	        
	        std::cout << "Gulg error: Failed to compile program.";
	        for(GLchar currentChar: errorLog) { std::cout << currentChar; }
	        std::cout << std::endl;
	        return false;
	    }

	    glDetachShader(newProgram, vertexShader);
	    glDetachShader(newProgram, fragmentShader);

	    m_programs.insert(std::make_pair(name, newProgram));
	}

	return true;
}

bool ProgramKeeper::isLoadedProgram(const std::string name) const {

	if(m_programs.find(name) != m_programs.end()) { return true; }
	return false;
}

GLuint ProgramKeeper::getProgram(const std::string name) const {

	if(isLoadedProgram(name)) { return m_programs.at(name); }

	std::cout << "Gulg error: asked for program \"" + name + "\", which isn't loaded." << std::endl;
	return 0;
}

}
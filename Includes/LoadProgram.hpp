#ifndef LOAD_PROGRAM_HPP
#define LOAD_PROGRAM_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <GL/glew.h>
#include <GL/gl.h>

#include <GLFW/glfw3.h>

bool createProgram(GLuint &program, const std::string &vertexPath, const std::string &fragmentPath);

#endif
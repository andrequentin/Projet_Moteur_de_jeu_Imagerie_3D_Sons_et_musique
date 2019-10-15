#ifndef VOXEL_SET_HPP
#define VOXEL_SET_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "VoxelWorld.hpp"

#include <iostream>

class VoxelSet {

	public:

		VoxelSet(VoxelWorld &voxelWorld, 
				const GLuint shaderProgram, 
				const unsigned int voxelDimension = 1, 
				const std::vector<unsigned int> &voxelSet = {});

		~VoxelSet();

		void addVoxel(const unsigned int voxelID);

		void reshape(const float voxelDimension = 1.f);
		
		void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

	private:

		void remakeGLShape();

		VoxelWorld &m_voxelWorld;

		std::vector<unsigned int> m_voxelSet;

		GLuint m_vbo, m_vao, m_program;
		GLint m_MVPProgramLocation;
        
        std::vector<glm::vec3> m_vertexPosition;
        std::vector<glm::vec4> m_vertexColor;
        std::vector<GLfloat> m_finalVertexArray;        
        
        glm::mat4 m_modelMatrix;
};

#endif
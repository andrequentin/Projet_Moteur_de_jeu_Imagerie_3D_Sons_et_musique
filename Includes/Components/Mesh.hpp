#ifndef MESH_COMPONENTS_HPP
#define MESH_COMPONENTS_HPP

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "Components/Component.hpp"

namespace Gg {

namespace Component {

struct Mesh: public AbstractComponent {

	Mesh(GLuint program):

		m_program{program},
		m_modelMatrixID{glGetUniformLocation(m_program, "ModelMatrix")},
		m_viewMatrixID{glGetUniformLocation(m_program, "ViewMatrix")},
		m_projectionMatrixID{glGetUniformLocation(m_program, "ProjectionMatrix")} {

			glGenVertexArrays(1, &m_vertexArrayID);
			glGenBuffers(1, &m_vertexPositionID);
			glGenBuffers(1, &m_vertexNormalID);
			glGenBuffers(1, &m_vertexIndiceID);
			glGenBuffers(1, &m_vertexColorID);
		}

	Mesh(const Mesh &mesh):
		m_program{mesh.m_program},
		m_modelMatrixID{mesh.m_modelMatrixID},
		m_viewMatrixID{mesh.m_viewMatrixID},
		m_projectionMatrixID{mesh.m_projectionMatrixID},
		m_vertexPosition{mesh.m_vertexPosition},
		m_vertexNormal{mesh.m_vertexNormal},
		m_vertexColor{mesh.m_vertexColor},
		m_vertexIndice{mesh.m_vertexIndice} {


		glGenVertexArrays(1, &m_vertexArrayID);
		glGenBuffers(1, &m_vertexPositionID);
		glGenBuffers(1, &m_vertexNormalID);
		glGenBuffers(1, &m_vertexIndiceID);
		glGenBuffers(1, &m_vertexColorID);

		reshape();
	}

	~Mesh() {

		glDeleteBuffers(1, &m_vertexColorID);
		glDeleteBuffers(1, &m_vertexIndiceID);
		glDeleteBuffers(1, &m_vertexNormalID);
		glDeleteBuffers(1, &m_vertexPositionID);
		glDeleteVertexArrays(1, &m_vertexArrayID);
	}


	virtual std::shared_ptr<AbstractComponent> clone() const { 

		return std::static_pointer_cast<AbstractComponent>(std::make_shared<Mesh>(*this)); 
	}


	void reshape() {

		glBindVertexArray(m_vertexArrayID);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionID);
	    glBufferData(GL_ARRAY_BUFFER, m_vertexPosition.size()*sizeof(glm::vec3), &m_vertexPosition[0], GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vertexNormalID);
	    glBufferData(GL_ARRAY_BUFFER, m_vertexNormal.size()*sizeof(glm::vec3), &m_vertexNormal[0], GL_STATIC_DRAW);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorID);
	    glBufferData(GL_ARRAY_BUFFER, m_vertexColor.size()*sizeof(glm::vec3), &m_vertexColor[0], GL_STATIC_DRAW);
	    glEnableVertexAttribArray(2);
	    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndiceID);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndice.size() * sizeof(unsigned int), &m_vertexIndice[0], GL_STATIC_DRAW);
	}

	void draw(const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {

		glBindVertexArray(m_vertexArrayID);
		glUseProgram(m_program);

	    glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	    glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	    glUniformMatrix4fv(m_projectionMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndiceID);
	    glDrawElements(GL_TRIANGLES, m_vertexIndice.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	}

	GLuint m_program, m_vertexArrayID, m_vertexIndiceID, m_vertexPositionID, m_vertexNormalID, m_vertexColorID;
	const GLint m_modelMatrixID, m_viewMatrixID, m_projectionMatrixID;
        
    std::vector<glm::vec3> m_vertexPosition, m_vertexNormal, m_vertexColor;
    std::vector<unsigned int> m_vertexIndice;         
};

}}

#endif
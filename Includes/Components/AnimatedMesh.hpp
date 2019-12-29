#ifndef ANIMATED_MESH_COMPONENTS_HPP
#define ANIMATED_MESH_COMPONENTS_HPP

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "Components/Mesh.hpp"

namespace Gg {

namespace Component {

struct Bone {

	unsigned int id;
	std::vector<Bone> children;

	glm::mat4 localTransformations, globalTransformations;

	void updateTransformations(const glm::mat4 transformations) {

		globalTransformations = transformations*localTransformations;
		for(size_t i{0}; i < children.size(); i++) { children[i].updateTransformations(globalTransformations); }
	}

	unsigned int bonesNumber() const {

		unsigned int result;
		if(children.empty()) { result = 1; }
		else {

			for(size_t i{0}; i < children.size(); i++) { result += children[i].bonesNumber(); }
			result++;
		}

		return result;
	}

	void giveTransformations(std::vector<glm::mat4> &transfo) const {

		transfo[id] = globalTransformations;
		for(size_t i{0}; i < children.size(); i++) { children[i].giveTransformations(transfo); }
	}
};

struct AnimatedMesh: public Mesh {

	AnimatedMesh(GLuint program, GLuint texture):
		Mesh{program},
		m_bonesTransformationsID{glGetUniformLocation(m_program, "BonesTransformations")},
		m_textureID{texture} {

			glGenVertexArrays(1, &m_vertexArrayID);
			glGenBuffers(1, &m_vertexPositionID);
			glGenBuffers(1, &m_vertexNormalID);
			glGenBuffers(1, &m_vertexIndiceID);
			glGenBuffers(1, &m_vertexColorID);
			glGenBuffers(1, &m_vertexBonesID);
			glGenBuffers(1, &m_vertexWeightID);
		}

	AnimatedMesh(const AnimatedMesh &mesh):
		Mesh{mesh.m_program},
		m_textureID{mesh.m_textureID},
		m_vertexBones{mesh.m_vertexBones},
		m_vertexWeight{mesh.m_vertexWeight} {

		glGenBuffers(1, &m_vertexBonesID);
		glGenBuffers(1, &m_vertexWeightID);

		m_vertexPosition = mesh.m_vertexPosition;
		m_vertexNormal = mesh.m_vertexNormal;
		m_vertexColor = mesh.m_vertexColor;
		m_vertexIndice = mesh.m_vertexIndice;

		reshape();
	}

	~AnimatedMesh() {

		glDeleteBuffers(1, &m_vertexWeightID);
		glDeleteBuffers(1, &m_vertexBonesID);
		glDeleteBuffers(1, &m_vertexColorID);
		glDeleteBuffers(1, &m_vertexIndiceID);
		glDeleteBuffers(1, &m_vertexNormalID);
		glDeleteBuffers(1, &m_vertexPositionID);
		glDeleteVertexArrays(1, &m_vertexArrayID);
	}


	virtual std::shared_ptr<AbstractComponent> clone() const { 

		return std::static_pointer_cast<AbstractComponent>(std::make_shared<AnimatedMesh>(*this)); 
	}


	virtual void reshape() {

		glBindVertexArray(m_vertexArrayID);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionID);
	    glBufferData(GL_ARRAY_BUFFER, m_vertexPosition.size()*sizeof(glm::vec3), &m_vertexPosition[0], GL_DYNAMIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vertexNormalID);
	    glBufferData(GL_ARRAY_BUFFER, m_vertexNormal.size()*sizeof(glm::vec3), &m_vertexNormal[0], GL_DYNAMIC_DRAW);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorID);
	    glBufferData(GL_ARRAY_BUFFER, m_vertexColor.size()*sizeof(glm::vec3), &m_vertexColor[0], GL_DYNAMIC_DRAW);
	    glEnableVertexAttribArray(2);
	    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBonesID);
	    glBufferData(GL_ARRAY_BUFFER, m_vertexBones.size()*sizeof(glm::ivec3), &m_vertexBones[0], GL_DYNAMIC_DRAW);
	    glEnableVertexAttribArray(3);
	    glVertexAttribPointer(3, 3, GL_INT, GL_FALSE, 0, nullptr);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vertexWeightID);
	    glBufferData(GL_ARRAY_BUFFER, m_vertexWeight.size()*sizeof(glm::vec3), &m_vertexWeight[0], GL_DYNAMIC_DRAW);
	    glEnableVertexAttribArray(4);
	    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndiceID);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndice.size() * sizeof(unsigned int), &m_vertexIndice[0], GL_DYNAMIC_DRAW);
	}

	virtual void draw(const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {

		std::vector<glm::mat4> bonesTransfo;
		unsigned int nbBones{m_bones.bonesNumber()};
		bonesTransfo.resize(nbBones);
		m_bones.giveTransformations(bonesTransfo);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glBindVertexArray(m_vertexArrayID);
		glUseProgram(m_program);

	    glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	    glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	    glUniformMatrix4fv(m_projectionMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);
	    glUniformMatrix4fv(m_bonesTransformationsID, nbBones, GL_FALSE, &bonesTransfo[0][0][0]);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexIndiceID);
	    glDrawElements(GL_TRIANGLES, m_vertexIndice.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	}

	GLint m_bonesTransformationsID;

	GLuint m_textureID, m_vertexBonesID, m_vertexWeightID;
        
    std::vector<glm::ivec3> m_vertexBones;
    std::vector<glm::vec3> m_vertexWeight;
    Bone m_bones;  
};

}}

#endif
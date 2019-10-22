#include "VoxelSet.hpp"

VoxelSet::VoxelSet(VoxelWorld &voxelWorld, 
				   const GLuint shaderProgram, 
				   const unsigned int voxelDimension, 
				   const std::vector<unsigned int> &voxelSet):

	m_voxelWorld{voxelWorld},
	m_voxelSet{voxelSet},
	m_program{shaderProgram},
	m_MVPProgramLocation{glGetUniformLocation(m_program, "MVPMatrix")},
	m_modelMatrix{glm::mat4{1.f}} { 

	//m_modelMatrix.identity();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	reshape(voxelDimension); 
}

VoxelSet::~VoxelSet() {

    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void VoxelSet::addVoxel(const unsigned int voxelID) { m_voxelSet.emplace_back(voxelID); }

void VoxelSet::reshape(const float voxelDimension) {

	const float verticeSize{voxelDimension/2.f};
	const unsigned int nbVectorPerVoxel{6*2*3};

	/*
		6 faces
		2 triangle per voxel face
		3 vertice per triangle
		3 coordinate per vertice
	*/


	/* 
		6 faces
		1 color per face
		4 coordinate per color
	*/

	//m_vertexPosition.resize(nbVectorPerVoxel*m_voxelSet.size());
	//m_vertexColor.resize(6*m_voxelSet.size());

	 //3 for vertex, 4 for color

	/*glm::vec3 currentCenterPosition;
	glm::vec4 currentColor;

	for(size_t i {0}; i < m_voxelSet.size(); i++) {

		currentCenterPosition = m_voxelWorld.getVoxelPosition(m_voxelSet[i])*voxelDimension;
		currentColor = m_voxelWorld.getColor(m_voxelSet[i]);

		//Face 1

		m_vertexPosition[i*nbVectorPerVoxel] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 1] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 2] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 3] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 4] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 5] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, verticeSize};

		m_vertexColor[i*6] = currentColor;

		//Face 2

		m_vertexPosition[i*nbVectorPerVoxel + 6] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 7] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 8] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 9] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 10] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 11] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};

		m_vertexColor[i*6 + 1] = currentColor;


		//Face 3

		m_vertexPosition[i*nbVectorPerVoxel + 12] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 13] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 14] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, -verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 15] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 16] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 17] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, -verticeSize};

		m_vertexColor[i*6 + 2] = currentColor;


		//Face 4

		m_vertexPosition[i*nbVectorPerVoxel + 18] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 19] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 20] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 21] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 22] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 23] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, verticeSize};

		m_vertexColor[i*6 + 3] = currentColor;

		//Face 5

		m_vertexPosition[i*nbVectorPerVoxel + 24] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 25] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 26] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 27] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 28] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 29] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};

		m_vertexColor[i*6 + 4] = currentColor;


		//Face 6

		m_vertexPosition[i*nbVectorPerVoxel + 30] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 31] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 32] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 33] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 34] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 35] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};

		m_vertexColor[i*6 + 5] = currentColor;
	}*/

	m_vertexPosition.clear();
	m_vertexColor.clear();
	m_finalVertexArray.clear();

	std::vector<std::pair<unsigned int, glm::vec3>> allFaces{voxelsAndOrientations()};
	allFaces = selectVisibleFaces(allFaces);

	std::vector<glm::vec3> currentQuad;

	for(std::pair<unsigned int, glm::vec3> currentFace: allFaces) {

		currentQuad = getFaceFromOrientation(m_voxelWorld.getVoxelPosition(currentFace.first), currentFace.second, voxelDimension);
		m_vertexPosition.emplace_back(currentQuad[0]);
		m_vertexPosition.emplace_back(currentQuad[1]);
		m_vertexPosition.emplace_back(currentQuad[2]);

		m_vertexPosition.emplace_back(currentQuad[0]);
		m_vertexPosition.emplace_back(currentQuad[3]);
		m_vertexPosition.emplace_back(currentQuad[2]);

		m_vertexColor.emplace_back(m_voxelWorld.getColor(currentFace.first));
	}


	for(size_t i {0}; i < m_vertexPosition.size(); i++) {

		m_finalVertexArray.emplace_back(m_vertexPosition[i][0]);
		m_finalVertexArray.emplace_back(m_vertexPosition[i][1]);
		m_finalVertexArray.emplace_back(m_vertexPosition[i][2]);

		m_finalVertexArray.emplace_back(m_vertexColor[i/6][0]);
		m_finalVertexArray.emplace_back(m_vertexColor[i/6][1]);
		m_finalVertexArray.emplace_back(m_vertexColor[i/6][2]);
		m_finalVertexArray.emplace_back(m_vertexColor[i/6][3]);
	}

	remakeGLShape();
}

void VoxelSet::remakeGLShape() {

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, m_finalVertexArray.size()*sizeof(GLfloat), m_finalVertexArray.data(), GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*7, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindAttribLocation(m_program, 0, "vertex");

   	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*7, reinterpret_cast<void*>(sizeof(GLfloat)*3));
    glBindAttribLocation(m_program, 1, "color");
}

std::vector<std::pair<unsigned int, glm::vec3>> VoxelSet::voxelsAndOrientations() const {

	std::vector<std::pair<unsigned int, glm::vec3>> result;
	result.reserve(m_voxelSet.size()*6);

	for(unsigned int currendID: m_voxelSet) {

		result.emplace_back(std::make_pair(currendID, glm::vec3{1.f, 0.f, 0.f}));
		result.emplace_back(std::make_pair(currendID, glm::vec3{-1.f, 0.f, 0.f}));
		result.emplace_back(std::make_pair(currendID, glm::vec3{0.f, 1.f, 0.f}));
		result.emplace_back(std::make_pair(currendID, glm::vec3{0.f, -1.f, 0.f}));
		result.emplace_back(std::make_pair(currendID, glm::vec3{0.f, 0.f, 1.f}));
		result.emplace_back(std::make_pair(currendID, glm::vec3{0.f, 0.f, -1.f}));
	}

	return result;
}

std::vector<std::pair<unsigned int, glm::vec3>> VoxelSet::selectVisibleFaces(const std::vector<std::pair<unsigned int, glm::vec3>> &facesToSelect) const {

	std::vector<std::pair<unsigned int, glm::vec3>> result;
	result.reserve(facesToSelect.size());

	glm::vec3 testingFace, worldSize{m_voxelWorld.getWoldDimensions()};

	for(const std::pair<unsigned int, glm::vec3> &currendFace: facesToSelect) {

		testingFace = m_voxelWorld.getVoxelPosition(currendFace.first) + currendFace.second;

		if(m_voxelWorld.getColor(currendFace.first) != glm::vec4{0.f, 0.f, 0.f, 0.f}) { // Delete void voxel

			//Out of bounds, have to draw it
			if(testingFace.x < 0.f || testingFace.x >= worldSize.x
			|| testingFace.y < 0.f || testingFace.y >= worldSize.y
			|| testingFace.z < 0.f || testingFace.z >= worldSize.z) {

				result.emplace_back(currendFace);
			}

			//No neighbourg
			else if(m_voxelWorld.getColor(testingFace.x, testingFace.y, testingFace.z) == glm::vec4{0.f, 0.f, 0.f, 0.f}) {

				result.emplace_back(currendFace);
			}
		}
	}

	return result;
}

std::vector<glm::vec3> VoxelSet::getFaceFromOrientation(const glm::vec3 &position, const glm::vec3 &orientation, const float voxelDimension) const {

	std::vector<glm::vec3> quad;
	quad.resize(4);

	if(orientation[0] == 0.f && orientation[1] == 0.f) {

		quad[0] = (position + orientation/2.f + glm::vec3{-0.5f, 0.5f, 0.f}); 
		quad[1] = (position + orientation/2.f + glm::vec3{-0.5f, -0.5f, 0.f}); 
		quad[2] = (position + orientation/2.f + glm::vec3{0.5f, -0.5f, 0.f}); 
		quad[3] = (position + orientation/2.f + glm::vec3{0.5f, 0.5f, 0.f}); 
	}

	else if(orientation[0] == 0.f && orientation[2] == 0.f) {

		quad[0] = (position + orientation/2.f + glm::vec3{-0.5f, 0.f, 0.5f}); 
		quad[1] = (position + orientation/2.f + glm::vec3{-0.5f, 0.f, -0.5f}); 
		quad[2] = (position + orientation/2.f + glm::vec3{0.5f, 0.f, -0.5f}); 
		quad[3] = (position + orientation/2.f + glm::vec3{0.5f, 0.f, 0.5f}); 
	}

	else if(orientation[1] == 0.f && orientation[2] == 0.f) {

		quad[0] = (position + orientation/2.f + glm::vec3{0.f, -0.5f, 0.5f}); 
		quad[1] = (position + orientation/2.f + glm::vec3{0.f, -0.5f, -0.5f}); 
		quad[2] = (position + orientation/2.f + glm::vec3{0.f, 0.5f, -0.5f}); 
		quad[3] = (position + orientation/2.f + glm::vec3{0.f, 0.5f, 0.5f}); 
	}

	return quad;
}

void VoxelSet::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

	glUseProgram(m_program);
    glBindVertexArray(m_vao);

    /*glm::mat4 mvp{m_modelMatrix};
    mvp *= viewMatrix;
    mvp *= projectionMatrix;*/
    glm::mat4 mvp{projectionMatrix};
    mvp *= viewMatrix;
    mvp *= m_modelMatrix;
    glUniformMatrix4fv(m_MVPProgramLocation, 1, GL_FALSE, &mvp[0][0]);
    
    glDrawArrays(GL_TRIANGLES, 0, m_vertexPosition.size()); 
}
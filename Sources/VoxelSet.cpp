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
	const unsigned int nbVectorPerVoxel{6*2*3}, nbVectorPerFace{6};

	/*
		6 faces
		2 triangle per voxel face
		3 vertice per triangle
		3 coordinate per vertice
	*/

	m_vertexPosition.resize(3);

	/* 
		6 faces
		1 color per face
		4 coordinate per color
	*/
	m_vertexColor.resize(m_voxelSet.size()*6);

	//m_finalVertexArray.resize(m_vertexPosition.size()*3 + m_vertexColor.size()*4);

	m_finalVertexArray.resize(m_vertexPosition.size()*3);

	glm::vec3 currentCenterPosition;

	for(size_t i {0}; i < 1; i++) {

		currentCenterPosition = m_voxelWorld.getVoxelPosition(m_voxelSet[i])*voxelDimension;

		//Face 1

		m_vertexPosition[i*nbVectorPerVoxel] = glm::vec3{-1, -1, 0};
		m_vertexPosition[i*nbVectorPerVoxel + 1] = glm::vec3{0, 1, 0};
		m_vertexPosition[i*nbVectorPerVoxel + 2] = glm::vec3{1, -1, 0};

		/*m_vertexPosition[i*nbVectorPerVoxel + 3] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 4] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 5] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, verticeSize};

		m_vertexColor[i*6] = m_voxelWorld.getColor(m_voxelSet[i]);

		/*
		//Face 2

		m_vertexPosition[i*nbVectorPerVoxel + 6] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 7] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 8] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 9] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 10] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 11] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};

		m_vertexColor[i*6 + 1] = m_voxelWorld.getColor(m_voxelSet[i]);


		//Face 3

		m_vertexPosition[i*nbVectorPerVoxel + 12] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 13] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 14] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, -verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 15] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 16] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 17] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, -verticeSize};

		m_vertexColor[i*6 + 2] = m_voxelWorld.getColor(m_voxelSet[i]);


		//Face 4

		m_vertexPosition[i*nbVectorPerVoxel + 18] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 19] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 20] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 21] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 22] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 23] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};

		m_vertexColor[i*6 + 3] = m_voxelWorld.getColor(m_voxelSet[i]);

	*/

		//Face 5

		/*m_vertexPosition[i*nbVectorPerVoxel + 24] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 25] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 26] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 27] = currentCenterPosition + glm::vec3{-verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 28] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 29] = currentCenterPosition + glm::vec3{verticeSize, verticeSize, verticeSize};

		m_vertexColor[i*6 + 4] = m_voxelWorld.getColor(m_voxelSet[i]);


		//Face 6

		m_vertexPosition[i*nbVectorPerVoxel + 30] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 31] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, -verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 32] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};

		m_vertexPosition[i*nbVectorPerVoxel + 33] = currentCenterPosition + glm::vec3{-verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 34] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, verticeSize};
		m_vertexPosition[i*nbVectorPerVoxel + 35] = currentCenterPosition + glm::vec3{verticeSize, -verticeSize, -verticeSize};

		m_vertexColor[i*6 + 5] = m_voxelWorld.getColor(m_voxelSet[i]);*/
	}


	for(size_t i {0}; i < m_vertexPosition.size(); i++) {

		m_finalVertexArray[i*3] = m_vertexPosition[i][0];
		m_finalVertexArray[i*3 + 1] = m_vertexPosition[i][1];
		m_finalVertexArray[i*3 + 2] = m_vertexPosition[i][2];

		// 1 color per face -> 1 color = 6 vertex 

		//m_finalVertexArray.emplace_back(m_vertexColor[i/6][0]);
		//m_finalVertexArray.emplace_back(m_vertexColor[i/6][1]);
		//m_finalVertexArray.emplace_back(m_vertexColor[i/6][2]);
		//m_finalVertexArray.emplace_back(m_vertexColor[i/6][3]);
	}


	for(size_t i {0}; i < m_finalVertexArray.size(); i++) {

		std::cout << m_finalVertexArray[i] << std::endl;
	}

	remakeGLShape();
}

void VoxelSet::remakeGLShape() {

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(m_finalVertexArray), m_finalVertexArray.data(), GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*3, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindAttribLocation(m_program, 0, "vertex");

   	//glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(GLfloat)*7, reinterpret_cast<void*>(3));
    //glBindAttribLocation(m_program, 1, "color");
}

void VoxelSet::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

	glUseProgram(m_program);
    glBindVertexArray(m_vao);

    glm::mat4 mvp{m_modelMatrix};
    m_modelMatrix *= viewMatrix;
    m_modelMatrix *= projectionMatrix;
    glUniformMatrix4fv(m_MVPProgramLocation, 1, GL_FALSE, &mvp[0][0]);
    
    glDrawArrays(GL_TRIANGLES, 0, 9); 
}
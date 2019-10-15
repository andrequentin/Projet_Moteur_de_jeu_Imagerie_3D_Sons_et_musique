#include "VoxelWorld.hpp"

VoxelWorld::VoxelWorld(const unsigned int x, const unsigned int y, const unsigned int z):
	m_sizeX{x}, m_sizeY{y}, m_sizeZ{z} {

	m_voxels.resize(m_sizeX*m_sizeY*m_sizeZ);

	for(glm::vec4 &currentVoxel: m_voxels) {

		currentVoxel = glm::vec4 {0.f, 0.f, 0.f, 0.f};
	}
}

glm::vec4 VoxelWorld::getColor(const unsigned int x, const unsigned int y, const unsigned int z) const {

	if(x >= m_sizeX || y >= m_sizeY || z >= m_sizeZ) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	return m_voxels[x*m_sizeY*m_sizeZ + y*m_sizeZ + z];
}

void VoxelWorld::setColor(const unsigned int x, const unsigned int y, const unsigned int z, const glm::vec4 &color) {

	if(x >= m_sizeX || y >= m_sizeY || z >= m_sizeZ) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	} 

	m_voxels[x*m_sizeY*m_sizeZ + y*m_sizeZ + z] = color;
}

glm::vec4 VoxelWorld::getColor(const unsigned int voxelID) const {

	if(voxelID > m_voxels.size()) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	return m_voxels[voxelID];
}

void VoxelWorld::setColor(const unsigned int voxelID, const glm::vec4 &color) {

	if(voxelID > m_voxels.size()) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	m_voxels[voxelID] = color;
}

unsigned int VoxelWorld::getVoxelID(const unsigned int x, const unsigned int y, const unsigned int z) const {

	if(x >= m_sizeX || y >= m_sizeY || z >= m_sizeZ) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	return x*m_sizeY*m_sizeZ + y*m_sizeZ + z;
}

glm::vec3 VoxelWorld::getVoxelPosition(const unsigned int voxelID) const {

	if(voxelID > m_voxels.size()) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	return glm::vec3{voxelID/(m_sizeY*m_sizeZ) % m_sizeX, voxelID/m_sizeZ % m_sizeY, voxelID % m_sizeZ};
}

glm::vec3 VoxelWorld::getWoldDimensions() const { return glm::vec3{m_sizeX, m_sizeY, m_sizeZ}; }
#include "Components/VoxelMap.hpp"

VoxelMap::VoxelMap(const unsigned int x, const unsigned int y, const unsigned int z):
	m_sizeX{x}, m_sizeY{y}, m_sizeZ{z} {

	m_voxels.resize(m_sizeX*m_sizeY*m_sizeZ);

	for(glm::vec4 &currentVoxel: m_voxels) {

		currentVoxel = glm::vec4 {0.0f, 0.0f, 0.0f, 0.0f};
	}
}

VoxelMap::VoxelMap(const VoxelMap &map):
	m_sizeX{map.m_sizeX},
	m_sizeY{map.m_sizeY},
	m_sizeZ{map.m_sizeZ} {}


glm::vec4 VoxelMap::getColor(const unsigned int x, const unsigned int y, const unsigned int z) const {

	if(x >= m_sizeX || y >= m_sizeY || z >= m_sizeZ) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	return m_voxels[x*m_sizeY*m_sizeZ + y*m_sizeZ + z];
}

void VoxelMap::setColor(const unsigned int x, const unsigned int y, const unsigned int z, const glm::vec4 &color) {

	if(x >= m_sizeX || y >= m_sizeY || z >= m_sizeZ) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	m_voxels[x*m_sizeY*m_sizeZ + y*m_sizeZ + z] = color;
}

glm::vec4 VoxelMap::getColor(const unsigned int voxelID) const {

	if(voxelID > m_voxels.size()) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	return m_voxels[voxelID];
}

void VoxelMap::setColor(const unsigned int voxelID, const glm::vec4 &color) {

	if(voxelID > m_voxels.size()) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	m_voxels[voxelID] = color;
}

unsigned int VoxelMap::getVoxelID(const unsigned int x, const unsigned int y, const unsigned int z) const {

	if(x >= m_sizeX || y >= m_sizeY || z >= m_sizeZ) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	return x*m_sizeY*m_sizeZ + y*m_sizeZ + z;
}

glm::vec3 VoxelMap::getVoxelPosition(const unsigned int voxelID) const {

	if(voxelID > m_voxels.size()) {

		throw std::runtime_error("Error: try to acces to an voxel who is outside the world.");
	}

	return glm::vec3{voxelID/(m_sizeY*m_sizeZ) % m_sizeX, voxelID/m_sizeZ % m_sizeY, voxelID % m_sizeZ};
}

std::array<unsigned int, 3> VoxelMap::getWorldDimensions() const { return std::array<unsigned int, 3>{m_sizeX, m_sizeY, m_sizeZ}; }

void VoxelMap::explode(unsigned int x,unsigned int y, unsigned int z,int explosivePower){
	for( int i{-explosivePower};i<explosivePower;i++){
		for( int j{-explosivePower};j<explosivePower;j++){
			for( int k{-explosivePower};k<=explosivePower;k++){
				if((static_cast<int>(x)+i) < static_cast<int>(m_sizeX) && (static_cast<int>(x)+i)>=0 && (static_cast<int>(y)+j) < static_cast<int>(m_sizeY) && (static_cast<int>(y)+j)>=0 && (static_cast<int>(z)+k) < static_cast<int>(m_sizeZ) && (static_cast<int>(z)+k)>=0){
					setColor(x+i,y+j,z+k,glm::vec4{0.f,0.f,0.f,0.f });
				}
			}
		}
	}
}

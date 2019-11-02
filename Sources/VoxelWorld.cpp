#include "VoxelWorld.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>
VoxelWorld::VoxelWorld(const unsigned int x, const unsigned int y, const unsigned int z):
	m_sizeX{x}, m_sizeY{y}, m_sizeZ{z} {

	m_voxels.resize(m_sizeX*m_sizeY*m_sizeZ);

	for(glm::vec4 &currentVoxel: m_voxels) {

		currentVoxel = glm::vec4 {0.0f, 0.0f, 0.0f, 0.0f};
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

void VoxelWorld::perlinGen( unsigned int frequenceInterpolation){
	// BRUIT ALÉATOIRE
	std::vector<std::vector<unsigned int>> bruit;
	bruit.resize(m_sizeX);
	srand (time(NULL));

	for(unsigned  i = 0;i < m_sizeX ;i++){
		for(unsigned int j = 0;j < m_sizeY; j++){
			bruit[i].push_back((unsigned int)rand()%(m_sizeZ/2)+1);
		}
	}

	if(m_sizeX % frequenceInterpolation != 0 || m_sizeY % frequenceInterpolation != 0){
		throw std::runtime_error("Error : m_sizeX and m_sizeY must be multiples of frequenceInterpolation");
		return;
	}

	//INTERPOLATION LINÉAIRE 2D
	for(unsigned int f = 0;f < frequenceInterpolation;f++){
		for(unsigned int g = 0;g < frequenceInterpolation;g++){
			unsigned int l=(m_sizeX*f)/frequenceInterpolation;
			unsigned int	m=(m_sizeX*(f+1))/frequenceInterpolation-1;
			if(f>0)l--;
			unsigned int	o=(m_sizeY*g)/frequenceInterpolation;
			unsigned int	p=(m_sizeY*(g+1))/frequenceInterpolation-1;
			if(g>0)o--;
			int a=bruit[l][o],
					b=bruit[l][p],
					c=bruit[m][o],
					d=bruit[m][p];

			for(int i = l; i <= m; i++){
				for(int j = o; j <= p; j++){
					double 	u=((double)i-(double)l)/((double)m-(double)l),
									v=((double)j-(double)o)/((double)p-(double)o);
					double	ap=a*(1.0-u) + c*(u),
									bp=b*(1.0-u) + d*(u);
					bruit[i][j] = (unsigned int )(ap*(1.0-v) + bp*(v));
				}
			}
		}
	}
	std::cout<<"--------------"<<std::endl;

	for(unsigned int i = 0;i < m_sizeX ;i++){
		for(unsigned int j = 0;j < m_sizeY; j++){
			for(unsigned int k = 0;k<bruit[i][j];k++){
				setColor(getVoxelID(i,j,k), glm::vec4{0.5f, 1.f, 0.5f, 1.f});
			}
		}
	}
	return;
}

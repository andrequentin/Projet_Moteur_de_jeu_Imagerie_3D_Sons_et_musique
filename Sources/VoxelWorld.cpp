#include "VoxelWorld.hpp"
#include <ctime>
#include <random>
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

void VoxelWorld::generateWorld( unsigned int interpolationFrequency){

	// BRUIT PERLIN ALÉATOIRE
	std::vector<std::vector<unsigned int>> bruit;
	bruit.resize(m_sizeX);
	auto const seed = std::time(nullptr);
  std::default_random_engine engin { seed };
	for(unsigned  x{0};x < m_sizeX ;x++){
		for(unsigned int y{0};y < m_sizeY; y++){
			bruit[x].push_back(engin()%(m_sizeZ/interpolationFrequency)+1);
			// std::cout<<bruit[x][y]<<"\t";
		}
		// std::cout<<std::endl;
	}

	if(m_sizeX % interpolationFrequency != 0 || m_sizeY % interpolationFrequency != 0){
		throw std::runtime_error("Error : m_sizeX and m_sizeY must be multiples of interpolationFrequency");
	}

	//INTERPOLATION LINÉAIRE 2D
	for(unsigned int f{0};f < interpolationFrequency;f++){
		for(unsigned int g{0};g < interpolationFrequency;g++){
			unsigned int minX{(m_sizeX*f)/interpolationFrequency};
			unsigned int	maxX{(m_sizeX*(f+1))/interpolationFrequency-1};
			if(f>0)minX--;
			unsigned int	minY{(m_sizeY*g)/interpolationFrequency};
			unsigned int	maxY{(m_sizeY*(g+1))/interpolationFrequency-1};
			if(g>0)minY--;
			double 	a (bruit[minX][minY]), // point d'interpolation 0-0
							b (bruit[minX][maxY]), // point d'interpolation 0-1
							c (bruit[maxX][minY]), // point d'interpolation 1-0
							d (bruit[maxX][maxY]); // point d'interpolation 1-1

			for(unsigned int i = minX; i <= maxX; i++){
				for(unsigned int j = minY; j <= maxY; j++){
					double 	u=static_cast<double>(i-minX)/static_cast<double>(maxX-minX), // distance entre a et la coordonées x du point courant
									v=static_cast<double>(j-minY)/static_cast<double>(maxY-minY); // distance entre b et la coordonées y du point courant
					double	ap (a*(1.0-u) + c*(u)), // ap = interpolation a-c distance u
									bp (b*(1.0-u) + d*(u)); // bp = interpolation b-d distance u
					bruit[i][j] = static_cast<unsigned int>(ap*(1.0-v) + bp*(v)); //interpolation ap-bp distance v
				}
			}
		}
	}
	// std::cout<<"--------------"<<std::endl;

	for(unsigned int x{0};x < m_sizeX ;x++){
		for(unsigned int y{0};y < m_sizeY; y++){
			// std::cout<<bruit[x][y]<<"\t";
			for(unsigned int z{0};z<bruit[x][y];z++){
				setColor(getVoxelID(x,y,z), glm::vec4{0.5f, 1.f, 0.5f, 1.f});
			}
		}
		// std::cout<<std::endl;
	}
	return;
}

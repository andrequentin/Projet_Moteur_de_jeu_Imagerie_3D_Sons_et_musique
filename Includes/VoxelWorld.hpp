#ifndef VOXEL_WORLD_HPP
#define VOXEL_WORLD_HPP

#include <vector>
#include <stdexcept>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

//#define EmptyVoxel glm::vec4{0.f, 0.f, 0.f, 0.f}

class VoxelWorld {

	public:

		VoxelWorld(const unsigned int x, const unsigned int y, const unsigned int z);

		glm::vec4 getColor(const unsigned int x, const unsigned int y, const unsigned int z) const;
		void setColor(const unsigned int x, const unsigned int y, const unsigned int z, const glm::vec4 &color);

		glm::vec4 getColor(const unsigned int voxelID) const;
		void setColor(const unsigned int voxelID, const glm::vec4 &color);

		unsigned int getVoxelID(const unsigned int x, const unsigned int y, const unsigned int z) const;

		glm::vec3 getVoxelPosition(const unsigned int voxelID) const;

		glm::vec3 getWoldDimensions() const;

	private:
		
		std::vector<glm::vec4> m_voxels;

		const unsigned int m_sizeX, m_sizeY, m_sizeZ;
};

#endif
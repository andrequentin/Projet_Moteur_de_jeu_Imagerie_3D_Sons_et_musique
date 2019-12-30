#ifndef VOXEL_MAP_HPP
#define VOXEL_MAP_HPP

#include <vector>
#include <stdexcept>
#include <ctime>
#include <random>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Components/Component.hpp"

class VoxelMap: public Gg::Component::AbstractComponent{

	public:

		VoxelMap(const unsigned int x, const unsigned int y, const unsigned int z);

		VoxelMap(const VoxelMap &map);

		virtual std::shared_ptr<AbstractComponent> clone() const{

			return std::static_pointer_cast<Gg::Component::AbstractComponent>(std::make_shared<VoxelMap>(*this));
		}

		glm::vec4 getColor(const unsigned int x, const unsigned int y, const unsigned int z) const;
		void setColor(const unsigned int x, const unsigned int y, const unsigned int z, const glm::vec4 &color);

		glm::vec4 getColor(const unsigned int voxelID) const;
		void setColor(const unsigned int voxelID, const glm::vec4 &color);

		unsigned int getVoxelID(const unsigned int x, const unsigned int y, const unsigned int z) const;

		glm::vec3 getVoxelPosition(const unsigned int voxelID) const;

		std::array<unsigned int, 3> getWorldDimensions() const;

		void explode(unsigned int x,unsigned int y, unsigned int z,int explosivePower);

	private:

		std::vector<glm::vec4> m_voxels;

		const unsigned int m_sizeX, m_sizeY, m_sizeZ;
};

#endif

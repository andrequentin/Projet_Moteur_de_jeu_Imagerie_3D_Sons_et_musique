#ifndef ENTITY_CREATOR_HPP
#define ENTITY_CREATOR_HPP

#include <stdexcept>
#include <deque>
#include <string>

#include "GulgEngine/GulgDeclarations.hpp"

namespace Gg {

class EntityCreator {

	public:

		EntityCreator();
		EntityCreator(const unsigned int entityReserveSize);

		void addToReserve(const unsigned int entityReserveSize);
		void freeEntity(const Entity freeEntity);

		Entity createEntity();
		size_t nbRemainingEntities() const;

	private:
                              
		std::deque<Entity> m_nextEntities;
		unsigned int m_nbMaximalEntity;

};

}

#endif
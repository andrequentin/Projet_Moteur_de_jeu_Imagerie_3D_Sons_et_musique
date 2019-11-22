#include "GulgEngine/EntityCreator.hpp"

namespace Gg {

	
EntityCreator::EntityCreator(): m_nbMaximalEntity{1} {}
EntityCreator::EntityCreator(const unsigned int entityReserveSize): m_nbMaximalEntity{1} { addToReserve(entityReserveSize); }

void EntityCreator::addToReserve(const unsigned int entityReserveSize) {

	for(unsigned int i{m_nbMaximalEntity}; i < m_nbMaximalEntity + entityReserveSize; i++) { m_nextEntities.emplace_back(i); }

	m_nbMaximalEntity += entityReserveSize;
}

void EntityCreator::freeEntity(const Entity freeEntity) { m_nextEntities.emplace_back(freeEntity); }

Entity EntityCreator::createEntity() {

	if(m_nextEntities.size() == 0) { addToReserve(1); }

	Entity newEntity{m_nextEntities.front()};
	m_nextEntities.pop_front();
	return newEntity;
}

size_t EntityCreator::nbRemainingEntities() const{ return m_nextEntities.size(); }

}
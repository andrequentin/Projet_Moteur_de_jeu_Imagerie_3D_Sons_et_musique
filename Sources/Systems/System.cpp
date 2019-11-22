#include "Systems/AbstractSystem.hpp"

namespace Gg {

namespace Systems {

AbstractSystem::AbstractSystem(GulgEngine &gulgEngine): m_gulgEngine{gulgEngine} {}

AbstractSystem::~AbstractSystem() {}

void AbstractSystem::addEntity(const Entity newEntity) {

	for(std::unique_ptr<Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) {

		if((m_gulgEngine.getEntitySignature(newEntity) & currentAlgo->getSignature()) == currentAlgo->getSignature()) {

			currentAlgo->addEntity(newEntity);
		}
	}
}

void AbstractSystem::deleteEntity(const Entity newEntity) {

	for(std::unique_ptr<Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) { currentAlgo->deleteEntity(newEntity); }
}

void AbstractSystem::applyAlgorithms() {

	for(std::unique_ptr<Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) { currentAlgo->apply(); }
}

}}
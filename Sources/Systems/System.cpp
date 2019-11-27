#include "Systems/System.hpp"

namespace Gg {

namespace Systems {

System::System(GulgEngine &gulgEngine): m_gulgEngine{gulgEngine} {}

System::~System() {}

void System::addEntity(const Entity newEntity) {

	for(std::unique_ptr<Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) {

		if((m_gulgEngine.getEntitySignature(newEntity) & currentAlgo->getSignature()) == currentAlgo->getSignature()) {

			currentAlgo->addEntity(newEntity);
		}
	}
}

void System::deleteEntity(const Entity newEntity) {

	for(std::unique_ptr<Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) { currentAlgo->deleteEntity(newEntity); }
}

void System::applyAlgorithms() {

	for(std::unique_ptr<Algorithm::AbstractAlgorithm> &currentAlgo: m_algorithms) { currentAlgo->apply(); }
}

}}
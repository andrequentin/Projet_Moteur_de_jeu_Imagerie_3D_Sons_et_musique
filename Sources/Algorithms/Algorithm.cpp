#include "Algorithms/Algorithm.hpp"

namespace Gg {

namespace Algorithm {

AbstractAlgorithm::AbstractAlgorithm(GulgEngine &gulgEngine): m_gulgEngine{gulgEngine} {}

AbstractAlgorithm::~AbstractAlgorithm() {}

void AbstractAlgorithm::addEntity(const Entity newEntity) { 

	std::vector<Entity>::iterator it{std::find(m_entitiesToApply.begin(), m_entitiesToApply.end(), newEntity)};
	if(it == m_entitiesToApply.end()) { m_entitiesToApply.emplace_back(newEntity); }
}

void AbstractAlgorithm::deleteEntity(const Entity entity) {

	std::vector<Entity>::iterator it{std::find(m_entitiesToApply.begin(), m_entitiesToApply.end(), entity)};
	if(it != m_entitiesToApply.end()) { m_entitiesToApply.erase(it); }
}

Signature AbstractAlgorithm::getSignature() const { return m_signature; }

}}
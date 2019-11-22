#include "Algorithms/SpecializedAlgorithm.hpp"

namespace Gg {

namespace Algorithm {

SpecializedAlgorithm::SpecializedAlgorithm(const std::string componentToApply, GulgEngine &gulgEngine): 
	AbstractAlgorithm{gulgEngine},
	m_componentToApply{componentToApply} {

	m_signature = m_gulgEngine.getComponentSignature(m_componentToApply);
}

SpecializedAlgorithm::~SpecializedAlgorithm() {}

}}
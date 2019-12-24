#include "Systems/Physics.hpp"

Physics::Physics(Gg::GulgEngine &gulgEngine): System{gulgEngine} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateForces>(gulgEngine));
}

Physics::~Physics() {}

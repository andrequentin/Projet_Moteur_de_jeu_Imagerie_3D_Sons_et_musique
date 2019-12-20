#include "Systems/Collisions.hpp"

Collisions::Collisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w): System{gulgEngine},world{w} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateCollisions>(gulgEngine,w));
}

Collisions::~Collisions() {}

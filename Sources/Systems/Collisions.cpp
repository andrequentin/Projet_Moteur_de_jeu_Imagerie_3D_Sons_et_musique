#include "Systems/Collisions.hpp"

Collisions::Collisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w): System{gulgEngine},world{w} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateCollisions>(gulgEngine,w));
	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::CollisionsResolution>(gulgEngine,w,
		dynamic_cast<Gg::Algorithm::UpdateCollisions*>(m_algorithms[0].get())));

}

Collisions::~Collisions() {}

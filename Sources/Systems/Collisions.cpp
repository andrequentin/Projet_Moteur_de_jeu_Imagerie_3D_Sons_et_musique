#include "Systems/Collisions.hpp"
#include "Algorithms/UpdateCollisions.hpp"
#include "Algorithms/CollisionsResolution.hpp"
Collisions::Collisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w,FMOD::Studio::EventInstance* s): System{gulgEngine},world{w},explosioneventInstance{s} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateCollisions>(gulgEngine,w,this));
	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::CollisionsResolution>(gulgEngine,w,this));


}

Collisions::~Collisions() {}

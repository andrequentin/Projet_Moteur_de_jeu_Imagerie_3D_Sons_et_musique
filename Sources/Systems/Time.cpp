#include "Systems/Time.hpp"
#include "Algorithms/UpdateTimer.hpp"

Time::Time(Gg::GulgEngine &gulgEngine,Gg::Entity &w,FMOD::Studio::EventDescription* s): System{gulgEngine},world{w},explosioneventDescription{s} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateTimer>(gulgEngine,w,this));

}

Time::~Time() {}

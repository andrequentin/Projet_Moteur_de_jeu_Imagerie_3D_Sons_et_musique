#include "Systems/Time.hpp"
#include "Algorithms/UpdateTimer.hpp"

Time::Time(Gg::GulgEngine &gulgEngine,Gg::Entity &w,FMOD::Studio::EventInstance* s): System{gulgEngine},world{w},explosioneventInstance{s} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateTimer>(gulgEngine,w,this));

}

Time::~Time() {}

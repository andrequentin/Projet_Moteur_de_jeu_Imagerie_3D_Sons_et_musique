#include "Systems/Time.hpp"
#include "Algorithms/UpdateTimer.hpp"

Time::Time(Gg::GulgEngine &gulgEngine,Gg::Entity &w): System{gulgEngine},world{w} {

	m_algorithms.emplace_back(std::make_unique<Gg::Algorithm::UpdateTimer>(gulgEngine,w,this));

}

Time::~Time() {}

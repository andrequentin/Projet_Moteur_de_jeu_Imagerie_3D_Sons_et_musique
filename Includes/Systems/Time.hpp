#ifndef TIME_SYSTEM_HPP
#define TIME_SYSTEM_HPP

#include "Systems/System.hpp"


class Time: public Gg::Systems::System {

	public:

		Time(Gg::GulgEngine &gulgEngine,Gg::Entity &w);

		virtual ~Time();

		Gg::Entity &world;
		std::vector<Gg::Entity> toDelete;
		std::vector<Gg::Entity> toAdd;

};


#endif

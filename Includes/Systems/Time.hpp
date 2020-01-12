#ifndef TIME_SYSTEM_HPP
#define TIME_SYSTEM_HPP

#include "Systems/System.hpp"

#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_errors.h>
class Time: public Gg::Systems::System {

	public:

		Time(Gg::GulgEngine &gulgEngine,Gg::Entity &w,FMOD::Studio::EventInstance * s);

		virtual ~Time();
		Gg::Entity &world;
		FMOD::Studio::EventInstance *explosioneventInstance;

		std::vector<Gg::Entity> toDelete;
		std::vector<Gg::Entity> toAdd;

};


#endif

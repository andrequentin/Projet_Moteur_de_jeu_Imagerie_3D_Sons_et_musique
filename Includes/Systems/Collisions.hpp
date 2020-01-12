#ifndef COLLISIONS_SYSTEM_HPP
#define COLLISIONS_SYSTEM_HPP

#include "Systems/System.hpp"

#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_errors.h>


class Collisions: public Gg::Systems::System {

	public:

		Collisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w,FMOD::Studio::EventDescription * s);

		virtual ~Collisions();


		Gg::Entity &world;
		FMOD::Studio::EventDescription *explosioneventDescription;

		std::vector<std::pair<Gg::Entity,std::vector<int>>> entity_world_collisions;
		std::vector<std::pair<Gg::Entity,Gg::Entity>> entity_entity_collisions;
		std::vector<Gg::Entity> toDelete;
		std::vector<Gg::Entity> toAdd;

};


#endif

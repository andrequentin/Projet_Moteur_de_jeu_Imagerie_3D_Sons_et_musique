#ifndef COLLISIONS_SYSTEM_HPP
#define COLLISIONS_SYSTEM_HPP

#include "Systems/System.hpp"

#include "Algorithms/UpdateCollisions.hpp"

class Collisions: public Gg::Systems::System {

	public:

		Collisions(Gg::GulgEngine &gulgEngine,Gg::Entity &w);

		virtual ~Collisions();

		Gg::Entity &world;

};


#endif

#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Systems/System.hpp"

#include "Algorithms/UpdateForces.hpp"

class Physics: public Gg::Systems::System {

	public:

		Physics(Gg::GulgEngine &gulgEngine);

		virtual ~Physics();


};


#endif

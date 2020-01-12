#ifndef COLLISIONS_RESOLUTION_ALGORITHM_HPP
#define COLLISIONS_RESOLUTION_ALGORITHM_HPP

#include "Algorithms/Algorithm.hpp"
#include "Systems/Collisions.hpp"
#include "NewMap.hpp"
#include "Components/SceneObject.hpp"
#include "Components/Collider.hpp"
#include "Components/Forces.hpp"
#include "Components/VoxelMap.hpp"
#include "Components/Explosive.hpp"
#include "Components/StepSound.hpp"


namespace Gg {

namespace Algorithm {

class CollisionsResolution: public AbstractAlgorithm {

	public:

		CollisionsResolution(GulgEngine &gulgEngine, Gg::Entity &w, Collisions* c );
		virtual ~CollisionsResolution();
		void apply();

	private:
		Gg::Entity &world;
     Collisions* collisions;

};

}}

#endif

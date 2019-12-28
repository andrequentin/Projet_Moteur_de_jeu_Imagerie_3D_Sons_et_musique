#ifndef COLLISIONS_RESOLUTION_ALGORITHM_HPP
#define COLLISIONS_RESOLUTION_ALGORITHM_HPP

#include "Algorithms/Algorithm.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"
#include "Components/Collider.hpp"
#include "Components/Forces.hpp"
#include "Components/VoxelMap.hpp"
#include "Components/Mesh.hpp"
#include "Algorithms/UpdateCollisions.hpp"


namespace Gg {

namespace Algorithm {

class CollisionsResolution: public AbstractAlgorithm {

	public:

		CollisionsResolution(GulgEngine &gulgEngine, Gg::Entity &w,Gg::Algorithm::UpdateCollisions* c );
		virtual ~CollisionsResolution();

		void apply();

	private:
		Gg::Entity &world;
    Gg::Algorithm::UpdateCollisions* collisions;

};

}}

#endif

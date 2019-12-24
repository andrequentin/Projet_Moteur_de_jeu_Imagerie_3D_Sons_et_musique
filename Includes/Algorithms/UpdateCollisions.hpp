#ifndef UPDATE_COLLISIONS_ALGORITHM_HPP
#define UPDATE_COLLISIONS_ALGORITHM_HPP

#include "Algorithms/Algorithm.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"
#include "Components/Collider.hpp"
#include "Components/Forces.hpp"
#include "Components/VoxelMap.hpp"
#include "Components/Mesh.hpp"


namespace Gg {

namespace Algorithm {

class UpdateCollisions: public AbstractAlgorithm {

	public:

		UpdateCollisions(GulgEngine &gulgEngine, Gg::Entity &w );
		virtual ~UpdateCollisions();

		void apply();

	private:
    Gg::Entity &world;

};

}}

#endif

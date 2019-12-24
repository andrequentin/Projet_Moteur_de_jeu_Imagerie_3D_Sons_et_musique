#ifndef UPDATE_FORCES_ALGORITHM_HPP
#define UPDATE_FORCES_ALGORITHM_HPP

#include "Algorithms/Algorithm.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"
#include "Components/Forces.hpp"


namespace Gg {

namespace Algorithm {

class UpdateForces: public AbstractAlgorithm {

	public:

		UpdateForces(GulgEngine &gulgEngine);
		virtual ~UpdateForces();

		void apply();


};

}}

#endif

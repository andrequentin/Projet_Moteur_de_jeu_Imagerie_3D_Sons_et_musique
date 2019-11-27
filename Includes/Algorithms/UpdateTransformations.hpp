#ifndef UPDATE_TRANSFORMATION_ALGORITHM_HPP
#define UPDATE_TRANSFORMATION_ALGORITHM_HPP

#include <string>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

#include "Algorithms/Algorithm.hpp"

#include "Components/Transformation.hpp"
#include "Components/SceneObject.hpp"

namespace Gg {

namespace Algorithm {

class UpdateTransformations: public AbstractAlgorithm {

	public:

		UpdateTransformations(GulgEngine &gulgEngine);
		virtual ~UpdateTransformations(); 

		void apply(); 

	private:

		void applyTransformations(const Gg::Entity entity, const glm::mat4 transformation = glm::mat4{1.0});
};

}}

#endif
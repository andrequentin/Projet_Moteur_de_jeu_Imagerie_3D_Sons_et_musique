#ifndef DRAW_MESH_ALGORITHM_HPP
#define DRAW_MESH_ALGORITHM_HPP

#include <string>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

#include "Algorithms/SpecializedAlgorithm.hpp"

#include "Components/Light.hpp"
#include "Components/SceneObject.hpp"

namespace Gg {

namespace Algorithm {

class UpdateLight: public SpecializedAlgorithm {

	public:

		UpdateLight(const std::string componentToApply, const GLuint program,  GulgEngine &gulgEngine);
		virtual ~UpdateLight();

		void apply(); 

	private:

		const GLuint m_program;
		unsigned int m_currentLightNumber;
};

}}

#endif
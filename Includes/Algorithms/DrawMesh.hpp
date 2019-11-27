#ifndef DRAW_MESH_ALGORITHM_HPP
#define DRAW_MESH_ALGORITHM_HPP

#include <string>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

#include "Algorithms/SpecializedAlgorithm.hpp"

#include "Components/Mesh.hpp"
#include "Components/SceneObject.hpp"

namespace Gg {

namespace Algorithm {

class DrawMesh: public SpecializedAlgorithm {

	public:

		DrawMesh(const std::string componentToApply, Gg::Entity &cameraEntity, glm::mat4 &projectionMatrix, GulgEngine &gulgEngine);
		virtual ~DrawMesh();

		void apply(); 

	private:

		Gg::Entity &m_cameraEntity;
		glm::mat4 &m_projectionMatrix;
};

}}

#endif
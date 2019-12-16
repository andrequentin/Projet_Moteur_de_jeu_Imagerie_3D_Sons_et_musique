#include "Algorithms/DrawMesh.hpp"

namespace Gg {

namespace Algorithm {

DrawMesh::DrawMesh(const std::string componentToApply, Gg::Entity &cameraEntity, glm::mat4 &projectionMatrix, GulgEngine &gulgEngine):
	SpecializedAlgorithm{componentToApply, gulgEngine},
	m_cameraEntity{cameraEntity},
	m_projectionMatrix{projectionMatrix} {

	m_signature += gulgEngine.getComponentSignature("SceneObject");
}

DrawMesh::~DrawMesh() {}

void DrawMesh::apply() {

	if(m_cameraEntity != Gg::NoEntity) {

		glm::mat4 viewMatrix{

			std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(m_cameraEntity, "SceneObject"))->m_globalTransformations
		};

		for(Gg::Entity currentEntity: m_entitiesToApply) {

			std::shared_ptr<Gg::Component::Mesh> currentMesh{
				std::static_pointer_cast<Gg::Component::Mesh>(m_gulgEngine.getComponent(currentEntity, m_componentToApply))
			};

			std::shared_ptr<Gg::Component::SceneObject> currentTransformation{
				std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(currentEntity, "SceneObject"))
			};

			currentMesh->draw(glm::inverse(currentTransformation->m_globalTransformations), viewMatrix, m_projectionMatrix);
		}
	}
}

}}

#include "Algorithms/UpdateTransformations.hpp"

namespace Gg {

namespace Algorithm {

UpdateTransformations::UpdateTransformations(Gg::GulgEngine &gulgEngine): 
	AbstractAlgorithm{gulgEngine} {

	m_signature = gulgEngine.getComponentSignature("SceneObject");
	m_signature += gulgEngine.getComponentSignature("Transformations");
}

UpdateTransformations::~UpdateTransformations() {}

void UpdateTransformations::apply() {

	for(Gg::Entity currentEntity: m_entitiesToApply) { applyTransformations(currentEntity); }
}

void UpdateTransformations::applyTransformations(const Gg::Entity entity, const glm::mat4 transformation) {

	std::shared_ptr<Gg::Component::Transformation> currentTransformation{ 
			std::static_pointer_cast<Gg::Component::Transformation>(m_gulgEngine.getComponent(entity, "Transformations"))
	};

	std::shared_ptr<Gg::Component::SceneObject> currentSceneObject{ 
			std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(entity, "SceneObject"))
	};

	currentSceneObject->m_globalTransformations = currentTransformation->getTransformationMatrix()*transformation;

	for(Gg::Entity childEntity: currentSceneObject->m_children) {

		applyTransformations(childEntity, currentSceneObject->m_globalTransformations);
	}
}

}}
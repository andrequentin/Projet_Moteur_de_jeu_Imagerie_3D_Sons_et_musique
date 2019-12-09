#include "Algorithms/UpdateLight.hpp"

namespace Gg {

namespace Algorithm {

UpdateLight::UpdateLight(const std::string componentToApply, const GLuint program, GulgEngine &gulgEngine): 
	SpecializedAlgorithm{componentToApply, gulgEngine},
	m_program{program},
	m_currentLightNumber{0},
	m_currentPositionID{-1}, 
	m_currentColorID{-1}, 
	m_currentAmbientID{-1} {

	m_signature = gulgEngine.getComponentSignature("Light");
	m_signature += gulgEngine.getComponentSignature("SceneObject");
}

UpdateLight::~UpdateLight() {}

void UpdateLight::apply() {

	m_currentLightNumber = 0;

	for(Gg::Entity currentEntity: m_entitiesToApply) { 

		m_currentPositionID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].position"}.c_str());
		m_currentColorID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].color"}.c_str());
		m_currentAmbientID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].ambient"}.c_str());

		std::shared_ptr<Gg::Component::SceneObject> currentSceneObject{ 
			std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(currentEntity, "SceneObject"))
		};

		std::shared_ptr<Gg::Component::Light> currentLight{ 
			std::static_pointer_cast<Gg::Component::Light>(m_gulgEngine.getComponent(currentEntity, "Light"))
		};

		glm::vec3 position{currentSceneObject->m_globalTransformations[0][3], 
						   currentSceneObject->m_globalTransformations[1][3], 
						   currentSceneObject->m_globalTransformations[2][3]};

		glUniform3fv(m_currentPositionID, 1, &position[0]);
	    glUniform3fv(m_currentColorID, 1, &(currentLight->m_color[0]));
	    glUniform1f(m_currentAmbientID, currentLight->m_ambient);

		m_currentLightNumber++;
	}
}

}}
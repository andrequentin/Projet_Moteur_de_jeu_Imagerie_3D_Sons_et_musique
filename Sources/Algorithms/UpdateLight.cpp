#include "Algorithms/UpdateLight.hpp"

namespace Gg {

namespace Algorithm {

UpdateLight::UpdateLight(const std::string componentToApply, const GLuint program, GulgEngine &gulgEngine): 
	SpecializedAlgorithm{componentToApply, gulgEngine},
	m_program{program},
	m_currentLightNumber{0} {

	m_signature = gulgEngine.getComponentSignature("Light");
	m_signature += gulgEngine.getComponentSignature("SceneObject");
}

UpdateLight::~UpdateLight() {}

void UpdateLight::apply() {

	m_currentLightNumber = 0;

	GLint positionID, directionID, ambientID, diffuseID, specularID, constantID, linearID, quadraticID, lightTypeID;

    //Position is given by SceneObject component

	for(Gg::Entity currentEntity: m_entitiesToApply) { 

		std::shared_ptr<Gg::Component::SceneObject> currentSceneObject{ 
			std::static_pointer_cast<Gg::Component::SceneObject>(m_gulgEngine.getComponent(currentEntity, "SceneObject"))
		};

		std::shared_ptr<Gg::Component::Light> currentLight{ 
			std::static_pointer_cast<Gg::Component::Light>(m_gulgEngine.getComponent(currentEntity, "Light"))
		};

		lightTypeID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].lightType"}.c_str());


		if(currentLight->m_lightType == Gg::Component::LightType::Point) {

			constantID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].constant"}.c_str());
			linearID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].linear"}.c_str());
			quadraticID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].quadratic"}.c_str());

			positionID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].position"}.c_str());

			glm::vec3 position{currentSceneObject->m_globalTransformations[3][0], 
						   	   currentSceneObject->m_globalTransformations[3][1], 
							   currentSceneObject->m_globalTransformations[3][2]};


			glUniform3fv(positionID, 1, &position[0]);

		    glUniform1f(constantID, currentLight->m_constant);
		    glUniform1f(linearID, currentLight->m_linear);
		    glUniform1f(quadraticID, currentLight->m_quadratic);

		    glUniform1ui(lightTypeID, 0);

		}

		else {

			directionID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].direction"}.c_str());
			glUniform3fv(directionID, 1, &(currentLight->m_direction[0]));

			glUniform1ui(lightTypeID, 1);

		}

		ambientID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].ambient"}.c_str());
		diffuseID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].diffuse"}.c_str());
		specularID = glGetUniformLocation(m_program, std::string{"Lights[" + std::to_string(m_currentLightNumber) + "].specular"}.c_str());

		glUniform3fv(ambientID, 1, &(currentLight->m_ambient[0]));
		glUniform3fv(diffuseID, 1, &(currentLight->m_diffuse[0]));
		glUniform3fv(specularID, 1, &(currentLight->m_specular[0]));

		m_currentLightNumber++;
	}
}

}}
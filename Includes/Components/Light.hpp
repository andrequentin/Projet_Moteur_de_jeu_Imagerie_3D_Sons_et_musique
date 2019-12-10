#ifndef LIGHT_COMPONENT_HPP
#define LIGHT_COMPONENT_HPP

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/vec3.hpp>

#include "Components/Component.hpp"

namespace Gg {

namespace Component {

enum class LightType { Point, Directional };

struct Light: public AbstractComponent {

	Light() {}

	Light(const Light &light): 
		m_direction{light.m_direction},
		m_constant{light.m_constant},
		m_linear{light.m_linear},
		m_quadratic{light.m_quadratic},
		m_ambient{light.m_ambient},
		m_diffuse{light.m_diffuse},
		m_specular{light.m_specular},
		m_lightType{light.m_lightType}  {}

	virtual std::shared_ptr<AbstractComponent> clone() const { 

		return std::static_pointer_cast<AbstractComponent>(std::make_shared<Light>(*this)); 
	}

    //Directional light

    glm::vec3 m_direction;

    //Point light
    
    float m_constant;
    float m_linear;
    float m_quadratic;  

    //Both

	glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;


    //Position is given by SceneObject component

    LightType m_lightType;
};

}}

#endif
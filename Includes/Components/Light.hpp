#ifndef LIGHT_COMPONENT_HPP
#define LIGHT_COMPONENT_HPP

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/vec3.hpp>

#include "Components/Component.hpp"

namespace Gg {

namespace Component {

struct Light: public AbstractComponent {

	Light() {}

	Light(const Light &light): 
		m_color{light.m_color},
		m_ambient{light.m_ambient}  {}

	virtual std::shared_ptr<AbstractComponent> clone() const { 

		return std::static_pointer_cast<AbstractComponent>(std::make_shared<Light>(*this)); 
	}

    glm::vec3 m_color;
    float m_ambient;
};

}}

#endif
#ifndef TRANSFORMATION_COMPONENT_HPP
#define TRANSFORMATION_COMPONENT_HPP

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Components/Component.hpp"

namespace Gg {

namespace Component {

struct Transformation: public AbstractComponent {

	Transformation():
	    m_translation{1.f},
	    m_scale{1.f},
	    m_specificTransformation{1.f},
	    m_rotation{glm::vec3{0.f, 0.f, 0.f}} {}

	Transformation(const Transformation &trans):
	    m_translation{trans.m_translation},
	    m_scale{trans.m_scale},
	    m_specificTransformation{trans.m_specificTransformation},
	    m_rotation{trans.m_rotation} {}

	virtual std::shared_ptr<AbstractComponent> clone() const {

		return std::static_pointer_cast<AbstractComponent>(std::make_shared<Transformation>(*this));
	}

    void translate(const glm::vec3 &translation) { m_translation = glm::translate(m_translation, translation); }

    void rotate(const float angle, const glm::vec3 &axis) { m_rotation = glm::angleAxis(angle, axis)*m_rotation; }

    void scale(const float scaling) { m_scale = glm::scale(m_scale, glm::vec3{scaling, scaling, scaling}); }

    void setSpecificTransformation(const glm::mat4 trans) { m_specificTransformation = trans; }


    glm::mat4 getTransformationMatrix() const { return m_translation * glm::toMat4(m_rotation) * m_scale * m_specificTransformation; }

	glm::mat4 m_translation, m_scale, m_specificTransformation;
    glm::quat m_rotation;
};

}}

#endif

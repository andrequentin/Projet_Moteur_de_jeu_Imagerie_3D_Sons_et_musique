#ifndef SCENE_OBJECT_COMPONENT_HPP
#define SCENE_OBJECT_COMPONENT_HPP

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>

#include "Components/Component.hpp"

namespace Gg {

namespace Component {

struct SceneObject: public AbstractComponent {

	SceneObject(): m_globalTransformations{1.f} {}

	SceneObject(const SceneObject &object): m_globalTransformations{object.m_globalTransformations}  {}

	virtual std::shared_ptr<AbstractComponent> clone() const { 

		return std::static_pointer_cast<AbstractComponent>(std::make_shared<SceneObject>(*this)); 
	}

	void addChild(Entity newChild) { m_children.emplace_back(newChild); }
    void deleteChild(Entity child) {

	   std::vector<Entity>::iterator it = std::find(m_children.begin(), m_children.end(), child);
	    if(it != m_children.end()) { m_children.erase(it); }
    }

    glm::mat4 m_globalTransformations;
    std::vector<Entity> m_children;
};

}}

#endif
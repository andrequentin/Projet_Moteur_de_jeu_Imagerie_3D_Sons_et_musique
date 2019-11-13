#include "SceneObject.hpp"

SceneObject::SceneObject():
    m_localTransformations{1.f},
    m_globalTransformations{1.f},
    m_translationMatrix{1.f},
    m_rotationMatrix{1.f},
    m_scaleMatrix{1.f} {}

SceneObject::~SceneObject() {}

void SceneObject::addChild(std::shared_ptr<SceneObject> newChild) { m_childObjects.emplace_back(newChild); }
void SceneObject::deleteChild(std::shared_ptr<SceneObject> child)  {

    std::vector<std::shared_ptr<SceneObject>>::iterator it = std::find(m_childObjects.begin(), m_childObjects.end(), child);
    if(it != m_childObjects.end()) { m_childObjects.erase(it); }
}


void SceneObject::updateTransformations(const glm::mat4 transformation) {

    m_globalTransformations = m_translationMatrix * m_rotationMatrix * m_scaleMatrix * m_localTransformations * transformation;

    for(std::shared_ptr<SceneObject> currentChild: m_childObjects) { currentChild->updateTransformations(m_globalTransformations); }
}

void SceneObject::draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {

    for(std::shared_ptr<SceneObject> currentChild: m_childObjects) {

        currentChild->draw(viewMatrix, projectionMatrix);
    }
}

glm::mat4 SceneObject::getCameraTransformations() const {

    glm::mat4 result, childResult;

    for(std::shared_ptr<SceneObject> currentChild: m_childObjects) {

        childResult = currentChild->getCameraTransformations();
        if(childResult != result) { return childResult; }
    }

    return result;
}


void SceneObject::translate(const glm::vec3 &translation) { m_translationMatrix = glm::translate(m_translationMatrix, translation); }
void SceneObject::rotate(const float angle, const glm::vec3 &axis) { m_rotationMatrix = glm::rotate(m_rotationMatrix, angle, axis); }
void SceneObject::scale(const float scaling) { m_scaleMatrix = glm::scale(m_scaleMatrix, glm::vec3{scaling, scaling, scaling}); }
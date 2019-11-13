#include "Camera.hpp"

Camera::Camera():
    SceneObject{} {}

Camera::~Camera() {}

glm::mat4 Camera::getCameraTransformations() const {  return m_globalTransformations; }

void Camera::lookAt(const glm::vec3 &position, const glm::vec3 &point, const glm::vec3 &orientation) { 

	m_localTransformations = glm::lookAt(position, point, orientation);
}
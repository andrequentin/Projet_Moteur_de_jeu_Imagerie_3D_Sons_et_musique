#ifndef SCENEOBJECT_HPP
#define SCENEOBJECT_HPP

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

class SceneObject {

    public:

        SceneObject();
        virtual ~SceneObject();

        void addChild(std::shared_ptr<SceneObject> newChild);
        void deleteChild(std::shared_ptr<SceneObject> child);

        void updateTransformations(const glm::mat4 transformation = glm::mat4{1.0});

        virtual void draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

        virtual glm::mat4 getCameraTransformations() const;

        void translate(const glm::vec3 &translation);
        void rotate(const float angle, const glm::vec3 &axis);
        void scale(const float scaling);

    protected:

        glm::mat4 m_localTransformations, m_globalTransformations;
        glm::mat4 m_translationMatrix, m_scaleMatrix;
        glm::quat m_rotations;

        std::vector<std::shared_ptr<SceneObject>> m_childObjects;
};

#endif

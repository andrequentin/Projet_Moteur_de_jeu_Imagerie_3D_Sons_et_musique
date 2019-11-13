#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "SceneObject.hpp"

class Camera: public SceneObject {

    public:

        Camera();
        virtual ~Camera();

        virtual glm::mat4 getCameraTransformations() const;

        void lookAt(const glm::vec3 &position, const glm::vec3 &point, const glm::vec3 &orientation);

    protected:

};

#endif

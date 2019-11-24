#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "SceneObject.hpp"

struct ShaderLight {

	glm::vec3 position;
	glm::vec3 color;

	float ambient, diffuse, specular;
};

class Light: public SceneObject {

    public:

        Light();
        virtual ~Light();

        glm::vec3 m_color;
        float m_ambient, m_diffuse, m_specular;

        std::vector<float> getLightForShader() const;
};

#endif

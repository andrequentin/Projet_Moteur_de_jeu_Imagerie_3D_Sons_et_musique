#include "Light.hpp"

Light::Light():
    SceneObject{} {}

Light::~Light() {}

std::vector<float> Light::getLightForShader() const {

	std::vector<float> result;
	result.resize(9);

	result[0] = m_globalTransformations[0][3];
	result[1] = m_globalTransformations[1][3];
	result[2] = m_globalTransformations[2][3];
	result[3] = m_color[0];
	result[4] = m_color[1];
	result[5] = m_color[2];
	result[6] = m_ambient;
	result[7] = m_diffuse;
	result[8] = m_specular;

	return result;
}
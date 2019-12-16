#version 150
#extension GL_ARB_explicit_attrib_location : enable

const uint Point       = 0u;
const uint Directional = 1u;

struct Light {

    //Directional light

    vec3 direction;

    //Point light

    vec3 position;

    float constant;
    float linear;
    float quadratic;

    //Both

  vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    uint lightType;
};

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform Light Lights[32];
uniform uint LightNumber;

out vec3 toFragPosition;
out vec3 toFragNormal;
out vec3 toFragColor;

void main() {

   gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(vertex, 1.0);

   toFragPosition = vec3(ModelMatrix*vec4(vertex, 1.0));
   toFragNormal = normal;
   toFragColor = color;
}

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
layout(location = 3) in ivec3 bones;
layout(location = 4) in vec3 weight;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform sampler2D ObjectTexture;

uniform Light Lights[32];
uniform uint LightNumber;

uniform mat4 BonesTransformations[32];

out vec3 toFragPosition;
out vec3 toFragNormal;
out vec3 toFragColor;

void main() {

  /*toFragPosition = vec3(0.0, 0.0, 0.0);
  toFragNormal = vec3(0.0, 0.0, 0.0);

  for(int i = 0; i < 3; i++) {

    toFragPosition += vec3(BonesTransformations[bones[i]]*vec4(vertex, 1.0)*weight[i]).xyz;
    toFragNormal += vec3(BonesTransformations[bones[i]]*vec4(normal, 1.0)*weight[i]).xyz;
  }

  gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(toFragPosition, 1.0);

  toFragColor = color;*/

  gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(vertex, 1.0);

   toFragPosition = vec3(ModelMatrix*vec4(vertex, 1.0));
   toFragNormal = normal;
   toFragColor = color;
}

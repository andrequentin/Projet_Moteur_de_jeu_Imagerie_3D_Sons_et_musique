#version 150

layout (location = 0) in vec3 vertex;

uniform mat4 LightVPMatrix;
uniform mat4 ModelMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}  
#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

uniform mat4 MVPMatrix;

out vec4 toFragColor;

void main() {

   gl_Position = MVPMatrix*vec4(vertex, 1.0);
   toFragColor = vec4(color, 1.0);
}

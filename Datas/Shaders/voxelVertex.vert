#version 150

in vec3 vertex;
in vec4 color;

uniform mat4 MVPMatrix;

out vec4 toFragColor;

void main() {

   gl_Position = MVPMatrix*vec4(vertex, 1.0);
   toFragColor = color;
}

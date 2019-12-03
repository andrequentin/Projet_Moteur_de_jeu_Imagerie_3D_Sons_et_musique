#version 150

in vec3 toFragColor;

out vec4 finalColor;

void main() {

   finalColor = vec4(toFragColor, 1.0);
}

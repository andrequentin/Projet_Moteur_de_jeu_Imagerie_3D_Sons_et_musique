#version 150

in vec4 toFragColor;

out vec4 finalColor;

void main() {

	if(toFragColor.a < 0.00005) { discard; }
   finalColor = toFragColor;
}

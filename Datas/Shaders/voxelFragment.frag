#version 150

in vec3 toFragPosition;
in vec3 toFragNormal;
in vec3 toFragColor;

uniform vec3 LPosition;
uniform vec3 LColor;
uniform float LAmbient;

out vec4 finalColor;

void main() {

   vec3 Ia = LAmbient*LColor;
   vec3 Id = LColor*max(dot(normalize(toFragNormal), normalize(LPosition - toFragPosition)), 0.0);

   //finalColor = vec4((Id)*toFragColor, 1.0);
   finalColor = vec4(abs(toFragNormal), 1.0);
}

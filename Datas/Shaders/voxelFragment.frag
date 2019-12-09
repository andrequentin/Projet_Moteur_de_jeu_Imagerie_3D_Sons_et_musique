#version 150

struct Light {

    vec3 position;
    vec3 color;
    float ambient;
};

in vec3 toFragPosition;
in vec3 toFragNormal;
in vec3 toFragColor;

uniform Light Lights[32];
uniform uint LightNumber;

out vec4 finalColor;

void main() {

   /*vec3 Ia = LAmbient*LColor;
   vec3 Id = LColor*max(dot(normalize(toFragNormal), normalize(LPosition - toFragPosition)), 0.0);

   finalColor = vec4((Ia + Id)*toFragColor, 1.0);*/

   finalColor = vec4(toFragColor, 1.0);
}

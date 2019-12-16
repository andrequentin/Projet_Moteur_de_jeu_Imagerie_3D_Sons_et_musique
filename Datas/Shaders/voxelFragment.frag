#version 150

//Light types

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

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform Light Lights[32];
uniform uint LightNumber;

in vec3 toFragPosition;
in vec3 toFragNormal;
in vec3 toFragColor;

out vec4 finalColor;

vec3 computePointLight(const uint i) {

	vec3 toLightPosition = normalize(Lights[i].position - toFragPosition);
    float diffuseCoef = max(dot(toFragNormal, toLightPosition), 0.0);
    //vec3 reflectedLightDirection = reflect(-toLightPosition, toFragNormal);

    //float specularCoef = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Point attenuation
    float distance = length(Lights[i].position - toFragPosition);

    float attenuation = 1.0 / (Lights[i].constant
    						 + Lights[i].linear * distance
  			    			 + Lights[i].quadratic * (distance * distance));

    // combine results
    vec3 ambient  = Lights[i].ambient  * toFragColor;
    vec3 diffuse  = Lights[i].diffuse  * diffuseCoef * toFragColor;
    //vec3 specular = Lights[i].specular * spec * toFragColor;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    //specular *= attenuation;

    return (ambient + diffuse);

}

vec3 computeDirectionalLight(const uint i) {

	vec3 toLightPosition = normalize(Lights[i].direction);
    float diffuseCoef = max(dot(toFragNormal, toLightPosition), 0.0);
    //vec3 reflectedLightDirection = reflect(-toLightPosition, toFragNormal);

    //float specularCoef = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = Lights[i].ambient  * toFragColor;
    vec3 diffuse  = Lights[i].diffuse  * diffuseCoef * toFragColor;
    //vec3 specular = Lights[i].specular * spec * toFragColor;

    return (ambient + diffuse);

}

void main() {

    vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < int(LightNumber); i++) {

		if(Lights[i].lightType == Point) { result += computePointLight(uint(i)); }
		else { result += computeDirectionalLight(uint(i)); }
	}

   finalColor = vec4(result, 1.0);
}

﻿#version 330

out vec4 colour;

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light{
vec3 colour;
float ambientIntensity;
float diffuseIntensity;
};

struct DirectionalLight{
Light base;
vec3 direction;
};

struct PointLight{
Light base;
vec3 position;
float constant;
float linear;
float exponent;
};

struct SpotLight{
PointLight base;
vec3 direction;
float edge;
};

struct Material{
float specularIntensity;
float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;

uniform Material material;

uniform vec3 eyePos;

float CalcDirectionalShadowFactor(DirectionalLight light){
	//By dividing it with w we get position between -1 and 1
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	//But depth map needs coords between 0 and 1 so we're handling it
	projCoords = (projCoords * 0.5) + 0.5;

	float closestDepth = texture(directionalShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction){
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);
	
	vec4 specularColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if(diffuseFactor > 0.0f){
	vec3 fragToEye = normalize(eyePos - FragPos);
	vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
	
	float specularFactor = dot(fragToEye, reflectedVertex);
	if(specularFactor > 0.0f){
		specularFactor = pow(specularFactor, material.shininess);
		specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	return (ambientColour + (diffuseColour + specularColour));
}

vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(PointLight pLight){
        
        vec3 direction = FragPos - pLight.position;
        float distance = length(direction);
        direction = normalize(direction);

        vec4 colour = CalcLightByDirection(pLight.base, direction);
        
        float attenuation = pLight.exponent * distance * distance +
                            pLight.linear * distance +
                            pLight.constant;

        if (attenuation <= 0.0) {
            attenuation = 1.0; // ✅ Prevent division by zero
        }

        return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight){
	
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);

	if(slFactor > sLight.edge){
		vec4 colour = CalcPointLight(sLight.base);

		return colour * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
	}
	else{
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights() {
    vec4 totalColour = vec4(0, 0, 0, 0);

    for (int i = 0; i < pointLightCount; i++) {

		if (pointLights[i].position.x == 0.0 && pointLights[i].position.y == 0.0 && pointLights[i].position.z == 0.0) {
            return vec4(0, 0, 1, 1);  // 🔵 Blue -> Position might be incorrect
        }
        totalColour += CalcPointLight(pointLights[i]);
    
	}

    return totalColour;
}

vec4 CalcSpotLights(){
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++){
	
		totalColour += CalcSpotLight(spotLights[i]);
	}

	return totalColour;
}

void main()
{
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();


	colour = texture(theTexture, TexCoord) * finalColour;

}
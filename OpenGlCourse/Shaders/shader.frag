#version 330

out vec4 colour;

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

const int MAX_POINT_LIGHTS = 3;

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

struct Material{
float specularIntensity;
float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePos;

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
	
	return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLights() {
    vec4 totalColour = vec4(0, 0, 0, 0);

    // 🟡 Debug: Check if no point lights are being processed
    if (pointLightCount == 0) {
        return vec4(1, 1, 0, 1); // 🟡 Yellow -> No point lights
    }

    for (int i = 0; i < pointLightCount; i++) {
        // 🔵 Debug: Check if the position is (0,0,0)
        if (pointLights[i].position.x == 0.0 && pointLights[i].position.y == 0.0 && pointLights[i].position.z == 0.0) {
            return vec4(0, 0, 1, 1);  // 🔵 Blue -> Position might be incorrect
        }

        vec3 direction = FragPos - pointLights[i].position;
        float distance = length(direction);
        direction = normalize(direction);

        vec4 colour = CalcLightByDirection(pointLights[i].base, direction);
        
        float attenuation = pointLights[i].exponent * distance * distance +
                            pointLights[i].linear * distance +
                            pointLights[i].constant;

        // 🟢 Debug: Check if attenuation is too high
        if (attenuation > 100.0) {
            return vec4(0, 1, 0, 1); // 🟢 Green -> Attenuation is too high
        }

        if (attenuation <= 0.0) {
            attenuation = 1.0; // ✅ Prevent division by zero
        }

        totalColour += (colour / attenuation);
    }

    // 🔴 Debug: If lighting fails, return red
    if (totalColour == vec4(0, 0, 0, 0)) {
        return vec4(1, 0, 0, 1);  // 🔴 Red -> Lighting completely failed
    }

    return totalColour;
}

void main()
{
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();


	colour = texture(theTexture, TexCoord) * finalColour;
}
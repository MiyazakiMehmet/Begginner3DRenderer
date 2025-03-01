#version 330

out vec4 colour;

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct DirectionalLight{
vec3 colour;
float ambientIntensity;
vec3 direction;
float diffuseIntensity;
};

struct Material{
float specularIntensity;
float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePos;

void main()
{
vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

vec4 specularColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);

if(diffuseFactor > 0.0f){
vec3 fragToEye = normalize(eyePos - FragPos);
vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

float specularFactor = dot(fragToEye, reflectedVertex);
if(specularFactor > 0.0f){
specularFactor = pow(specularFactor, material.shininess);
specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
}
}

colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour + specularColour);
}
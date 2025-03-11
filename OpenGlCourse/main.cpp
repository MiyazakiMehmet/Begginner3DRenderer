#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "ShadowMap.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


//Window Dimensions
const GLint WIDTH = 1200, HEIGHT = 800;
const float toRadians = 3.14159265f / 180.f;

Window mainWindow;

Camera camera;

GLuint uniformProjection = 0, uniformView = 0, uniformModel = 0,
uniformSpecularIntensity = 0, uniformShininess = 0, uniformEyePosition = 0;

DirectionalLight directionalLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightCount;
unsigned int spotLightCount;

Material shinyMaterial;
Material dullMaterial;

Model flashlight;
Model lamp;
glm::vec3 lampPosition;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.6f;
float triIncrement = 0.005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.0f;
float maxSize = 0.8f;
float minSize = 0.1f;

//Vertex Shader
//A fragment shader is the same as pixel shader.

//One main difference is that a vertex shader can manipulate the attributes of vertices.which are the corner points of your polygons.

//The fragment shader on the other hand takes care of how the pixels between the vertices look.They are interpolated between the defined vertices following specific rules.

static const char* vShader = "Shaders/shader.vert";

static const char* fShader = "Shaders/shader.frag";

//This function will calculate each triangles' normal vectors by following the indicies. Finally it will ad it to nx, ny and nz
void CalculateAverageNormals(unsigned int* indicies, unsigned int indiciesCount, GLfloat* verticies, unsigned int vertexCount,
	unsigned int vertexLength, unsigned int normalOffset) {
	//To evaluate all triangles' normal, we use for loop
	for (size_t i = 0; i < indiciesCount; i += 3) {
		unsigned int in0 = indicies[i] * vertexLength;
		unsigned int in1 = indicies[i+1] * vertexLength;
		unsigned int in2 = indicies[i+2] * vertexLength;
		//To find normal of two vectors u should get the cross product of two vectors.
		glm::vec3 v1(verticies[in1] - verticies[in0], verticies[in1 + 1] - verticies[in0 + 1], verticies[in1 + 2] - verticies[in0 + 2]);
		glm::vec3 v2(verticies[in2] - verticies[in0], verticies[in2 + 1] - verticies[in0 + 1], verticies[in2 + 2] - verticies[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
	
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		verticies[in0] += normal.x; verticies[in0 + 1] += normal.y; verticies[in0+2] += normal.z;
		verticies[in1] += normal.x; verticies[in1 + 1] += normal.y; verticies[in1 + 2] += normal.z;
		verticies[in2] += normal.x; verticies[in2 + 1] += normal.y; verticies[in2 + 2] += normal.z;
	}
	//vertexCount/vertexLength = number of rows
	//this loop sets each normal offset in rows
	for (size_t i = 0; i < vertexCount / vertexLength; i++) {
		unsigned int nOffset = i * vertexLength + normalOffset;
		glm::vec3 vec(verticies[nOffset], verticies[nOffset+1], verticies[nOffset+2]);
		vec = glm::normalize(vec);
		verticies[nOffset] = vec.x; verticies[nOffset+1] = vec.y; verticies[nOffset+2] = vec.z;
	}
}


void CreateObject() {

	unsigned  int indicies[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat verticies[] = {
		//x     y      z      v(x for texture) u(y for texture / (u,v starts from left-bottom) /nx    ny    nz
	   -1.0f,  -1.0f, -0.6f,  0.0f,            0.0f,											0.0f, 0.0f, 0.0f,
		0.0f,  -1.0f,  1.0f,  0.5f,            0.0f,											0.0f, 0.0f, 0.0f,
		1.0f,  -1.0f, -0.6f,  1.0f,            0.0f,											0.0f, 0.0f, 0.0f,
		0.0f,   1.0f,  0.0f,  0.5f,            1.0f,											0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	CalculateAverageNormals(indicies, 12, verticies, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(verticies, indicies, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj2);

}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
}

void RenderScene() {
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 2.0f, -4.0f));
	//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

	//Binding our uniform and matrix
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	//Using texture
	meshList[0]->brickTexture.UseTexture();
	//Using material
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	//Using/drawing the mesh
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[1]->dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	//Model Flashlight
	model = glm::mat4(1.0f);
	glm::vec3 cameraPosition = camera.GetCameraPosition();

	glm::vec3 flashLightOffset = glm::vec3(0.3f, -0.1f, -0.2f);
	glm::vec3 cameraYawPitchPosition = camera.ObjectPositionAttachedToCamera(flashLightOffset);

	// Step 3: Apply the rotation and translation to the sword's position
	// First, apply the camera's rotation to the offset to transform it into world space
	glm::vec3 flashlightWorldPosition = cameraPosition + cameraYawPitchPosition;
	model = glm::translate(model, flashlightWorldPosition);
	model = glm::rotate(model, (-camera.GetYaw() + 180) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, (-camera.GetPitch()) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	flashlight.RenderModel();

	//Model Lamp
	model = glm::mat4(1.0f);

	model = glm::translate(model, lampPosition);
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));


	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	lamp.RenderModel();
}

//Drawing Shadows
void DirectionalShadowMapPass(DirectionalLight* light) {
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	//We are saying that write these on new framebuffer that we specified. Not the defaukt one
	//Note: We created frame buffer seperately because if we need a another framebuffeer instead of default one(the one glfw does it for us) we have to specify it
	light->GetShadowMap()->Write();
	//Instead of drawing colour we want to draw depth map so this func will not clear colour, it will clear existing depth buffers if there are exists
	glClear(GL_DEPTH_BUFFER_BIT);

	//Store glsl uniforms with datas
	uniformModel = directionalShadowShader.GetModelLocation();

	glm::mat4 lightTransform = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransform);

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Drawing Colours
void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
	//Using the shader
	shaderList[0].UseShader();

	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();

	glViewport(0, 0, 1366, 768);
	

	//Clear Window froom colours and depth buffers if exists (we want to draw on a clean plane)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Stores uniforms in glsl with datas
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	//Stores uniforms in glsl with datas
	shaderList[0].SetDirectionalLight(&directionalLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount); //since pointLights is array we dont have to reference it because its already referenced
	shaderList[0].SetSpotLights(spotLights, spotLightCount);
	//For shadowing, we recalculate our Projection and view matrices to look from light camera of view
	glm::mat4 lightTransform = directionalLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&lightTransform);

	//Passing Texture1 as index since Texture0 is full
	directionalLight.GetShadowMap()->Read(GL_TEXTURE1);

	//Since it's the one who carries TEXTURE0 it is bind to 0 (by default its already 0, but for debugging its convenient)
	shaderList[0].SetTexture(0);
	//Since it's the one who carries TEXTURE1 it is bind to 1
	shaderList[0].SetDirectionalShadowMap(1);

	spotLights[0].SetFlash(camera.GetCameraPosition(), camera.GetCameraDirection());
	
	RenderScene();
}

int main() {

	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	//Loading the mesh
	CreateObject();
	//Loading the shader
	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	directionalLight = DirectionalLight(1024, 1024,
										1.0f, 1.0f, 1.0f,
										0.1f,
										0.0f, -1.0f, 0.0f,
										0.6f);

	lampPosition = glm::vec3(6.0f, 1.9f, 0.0f);
	pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f,
								0.2f, 0.4f,
							   lampPosition.x, lampPosition.y - 2, lampPosition.z,
								0.3f, 0.2f, 0.1f);
	//pointLightCount++;
	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
								0.0f, 0.1f,
								2.0f, 2.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	//pointLightCount++;
	
	spotLightCount = 0;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.1f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	//spotLightCount++;
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
                        	  0.0f, 4.0f,
                        	  3.0f, 0.2f, 0.0f,
                        	  -6.0f, -1.3f, 0.0f,
                        	  1.0f, 0.0f, 0.0f,
							  20.0f);
	//spotLightCount++;

	shinyMaterial = Material(1.0f, 32.0f);
	dullMaterial = Material(0.3f, 4.0f);

	flashlight = Model();
	flashlight.LoadModel("Models/Linterna.obj");

	lamp = Model();
	lamp.LoadModel("Models/streetlamp.fbx");

	

	//projection matrix that we want to attach
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeigth(), 0.1f, 100.0f);

	//Loop until window closed 
	while (!mainWindow.getShouldClose()) {

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		camera.KeyControl(mainWindow.getKeys(), deltaTime);
		camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		curAngle += 0.09f;
		if (curAngle >= 360) {
			//Accelerates triangle
			curAngle -= 360;
		}

		DirectionalShadowMapPass(&directionalLight);
		RenderPass(projection, camera.CalculateViewMatrix());

		glUseProgram(0);

		//swap with the other scene
		mainWindow.SwapBuffers();

	}


	return 0;
}
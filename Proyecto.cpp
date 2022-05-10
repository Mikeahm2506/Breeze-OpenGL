/*
Semestre 2022-2
Pr�ctica 5: Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"


const float toRadians = 3.14159265f / 180.0f;

// variables

float puerta;

// ***** variables para mover textura ****
float cordTexU;
float cordTexV;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture pgTexture;
Texture escalonTexture;
Texture paredTexture;
Texture paredATexture;
Texture aguaTexture;



Model contenedor_M;
Model caja_M;
Model marco_M;
Model puerta_M;
Model torre_M;
Model mesa_M;
Model compu_M;
Model humo_M;
Model jett_M;
Model tiro_M;



Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];



// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-15.0f, 0.0f, -15.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		15.0f, 0.0f, -15.0f,	15.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-15.0f, 0.0f, 15.0f,	0.0f, 15.0f,	0.0f, -1.0f, 0.0f,
		15.0f, 0.0f, 15.0f,		15.0f, 15.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int floorGIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorGVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f
	};

	

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(floorGVertices, floorGIndices, 32, 6);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.65f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.65f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.51f,  0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.74f,	0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.74f,	0.65f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.51f,	0.65f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.75f,  0.34f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.34f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	0.65f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.75f,	0.65f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.01f,  0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.24f,	0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.24f,	0.65f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.01f,	0.65f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.51f,  0.01f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.74f,	0.01f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.74f,	0.32f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.51f,	0.32f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.51f,  0.67f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.74f,	0.67f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.74f,	0.99f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.51f,	0.99f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearOctaedro()
{
	unsigned int octaedro_indices[] = {
		// front up
		0, 1, 2,
		// right up
		3, 4 ,5,
		// back up
		6, 7, 8,
		// left up
		9, 10, 11,
		// front bottom
		12, 13, 14,
		// rigth bottom
		15, 16, 17,
		// back bottom
		18, 19, 20,
		// left bottom
		21, 22, 23
	};
	
// average normals
	GLfloat octaedro_vertices[] = {
		// front up
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, 0.0f,  0.5f,		0.145f, 0.665f,		0.0f,	-1.0f,	0.0f,	//0
		0.5f,  0.0f,  0.5f,		0.289f,	0.34f,		0.0f,	-1.0f,	0.0f,	//1
		0.0f,  0.5f,  0.0f,		0.428f,	0.665f,		0.0f,	-1.0f,	0.0f,	//2
	
		// right up
		//x		y		z		S		T
		0.5f,  0.0f,  0.5f,	    0.289f,	0.338f,		 1.0f,	0.0f,	0.0f,	//3
		0.5f,  0.0f, -0.5f, 	0.580f,	0.338f,		 1.0f,	0.0f,	0.0f,	//4
		0.0f,  0.5f,  0.0f,		0.428f,	0.67f,		 1.0f,	0.0f,	0.0f,	//5
	
		// back up
		0.5f,  0.0f, -0.5f,		0.575f,	0.335f,		0.0f,	0.0f,	1.0f,	//6
		-0.5f, 0.0f, -0.5f,		0.71f,	0.66f,		0.0f,	0.0f,	1.0f,	//7
		0.0f,  0.5f,  0.0f,		0.435f,	0.66f,		0.0f,	0.0f,	1.0f,	//8


		// left up
		//x		y		z		S		T
		-0.5f,  0.0f,  -0.5f,	0.71f, 0.665f,		1.0f,	0.0f,	0.0f,	//9
		-0.5f,  0.0f,  0.5f,	0.99f,	0.665f,		1.0f,	0.0f,	0.0f,	//10
		 0.0f,  0.5f,  0.0f,	0.847f,	0.999f,		1.0f,	0.0f,	0.0f,	//11
		

		// front bottom
		//x		y		z		S		T
		 0.0f, -0.5f,  0.0f,	0.001f, 0.335f,		0.0f,	1.0f,	0.0f,	//12
		 0.5f,  0.0f,  0.5f,	0.285f,	0.335f,		0.0f,	1.0f,	0.0f,	//13
		-0.5f,  0.0f,  0.5f,	0.142f,	0.67f,		0.0f,	1.0f,	0.0f,	//14


		// rigth bottom
		//x		y		z		S		T
		 0.0f, -0.5f,  0.0f,	0.004f, 0.336f,		-1.0f,	0.0f,	0.0f,	//15
		 0.5f,  0.0f, -0.5f,	0.142f,	0.005f,		-1.0f,	0.0f,	0.0f,	//16
		 0.5f,  0.0f,  0.5f,	0.281f,	0.336f,		-1.0f,	0.0f,	0.0f,	//17

		// back bottom
		//x		y		z		S		T
		 0.0f, -0.5f,  0.0f,	0.857f, 0.335f,		0.0f,	0.0f,	1.0f,	//18
		-0.5f,  0.0f, -0.5f,	0.712f,	0.665f,		0.0f,	0.0f,	1.0f,	//19
		 0.5f,  0.0f, -0.5f,	0.571f,	0.335f,		0.0f,	0.0f,	1.0f,	//20

		// left bottom
		//x		y		z		S		T
		 0.0f, -0.5f,  0.0f,	0.855f, 0.338f,		1.0f,	0.0f,	0.0f,	//21
		-0.5f,  0.0f,  0.5f,	0.991f,	0.665f,		1.0f,	0.0f,	0.0f,	//22
		-0.5f,  0.0f, -0.5f,	0.714f,	0.665f,		1.0f,	0.0f,	0.0f,	//23
		

	};
	
	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	meshList.push_back(octaedro);

}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	GLuint uniformTextureOffset = 0;
	uniformTextureOffset = shaderList[0].getoffsetLocation();
	

	camera = Camera(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso_Breze.tga");
	pisoTexture.LoadTextureA();
	pgTexture = Texture("Textures/pisog.tga");
	pgTexture.LoadTextureA();
	escalonTexture = Texture("Textures/escalon.tga");
	escalonTexture.LoadTextureA();
	paredTexture = Texture("Textures/pared.tga");
	paredTexture.LoadTextureA();
	paredATexture = Texture("Textures/paredA.tga");
	paredATexture.LoadTextureA();
	aguaTexture = Texture("Textures/agua.tga");
	aguaTexture.LoadTextureA();




	
	contenedor_M = Model();
	contenedor_M.LoadModel("Models/contenedor.obj");
	caja_M = Model();
	caja_M.LoadModel("Models/caja.obj");
	marco_M = Model();
	marco_M.LoadModel("Models/marco.obj");
	puerta_M = Model();
	puerta_M.LoadModel("Models/puerta.obj");
	torre_M = Model();
	torre_M.LoadModel("Models/torre.obj");
	mesa_M = Model();
	mesa_M.LoadModel("Models/mesa.obj");
	compu_M = Model();
	compu_M.LoadModel("Models/compu.obj");
	humo_M = Model();
	humo_M.LoadModel("Models/humo.obj");
	jett_M = Model();
	jett_M.LoadModel("Models/jett.obj");
	tiro_M = Model();
	tiro_M.LoadModel("Models/tiro.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Right.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Left.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Bottom.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Top.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Front.tga");
	skyboxFaces.push_back("Textures/Skybox/Daylight Box_Back.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.8f, 0.8f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(0.0f, 1.0f, 0.0f,
		3.0f, 3.0f,
		30.0f, 40.f, -60.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	//Declaración de segunda luz puntual
	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		3.0f, 3.0f,
		30.0f, 40.f, -60.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	
	/*
	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	

	//luz de computadora
	spotLights[1] = SpotLight(0.0f, 0.0f, 0.0f,
		0.0001f, 0.0001f,
		100.0f, 5.0, -121.0f,
		0.0f, 0.0f, 1.0f,
		0.05f, 0.0f, 0.0f,
		60.0f);
	spotLightCount++;
	*/

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	/*
	//luz de helicóptero
	spotLights[1] = SpotLight(0.0f, 0.8f, 1.0f,
		1.0f, 2.0f,
		1.5f, 4.5f, 31.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	//luz de antrocha
	pointLights[2] = PointLight(1.0f, 0.5f, 0.0f,
		1.0f, 3.0f,
		193.5f, 18.0f, -150.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	*/
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	////////////
	puerta = -1.55;
	

	////////
	cordTexU = 0.0f;
	cordTexV = 0.0f;

	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		// ***** moviemiento de puerta ****
		if (mainWindow.getapaga() == 0.0f) {
			if (puerta < 4.0f) {
				puerta += 0.05 * deltaTime;
			}
		}
		else {
			if (puerta > -1.55f) {
				puerta -= 0.05 * deltaTime;
			}
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		
		/*
		if (mainWindow.getflag() == 1.0) {
			//luz de carro hacia adelante
			spotLights[2] = SpotLight(1.0f, 0.0f, 0.5f,
				1.0f, 5.0f,
				6.0f, 6.0, 0.0f,
				1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				15.0f);
			spotLightCount=3;
		}
		else {
			//luz de carro hacia atras
			spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
				1.0f, 5.0f,
				6.0f, 6.0, 0.0f,
				-1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				15.0f);
			spotLightCount=3;
		}
		*/
		// luz ligada al helicoptero de tipo flash
		glm::vec3 heliLight(mainWindow.getmueveHelix(), mainWindow.getmueveHeliy(), 
		mainWindow.getmueveHeliz());
		spotLights[1].SetFlash(heliLight, glm::vec3(0.0f, -1.0f, 0.0f));

		if (mainWindow.getflag() == 1.0f) {
			// luz ligada al carro de tipo flash delantera
			glm::vec3 autoLightd(mainWindow.getmuevex(), 0.5f, mainWindow.getmuevez());
			spotLights[2].SetFlash(autoLightd, glm::vec3(1.0f, 0.0f, 0.0f));
		}else {
			// luz ligada al carro de tipo flash trasera
			glm::vec3 autoLightt(mainWindow.getmuevex(), 0.5f, mainWindow.getmuevez());
			spotLights[2].SetFlash(autoLightt, glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		

		// luz ligada a la primera luz puntual de tipo flash
		glm::vec3 point1Light(mainWindow.getmuevex(), 40.0f, mainWindow.getmuevez());
		pointLights[0].SetFlash(point1Light);

		// luz ligada a la segunda luz puntual de tipo flash
		glm::vec3 point2Light(mainWindow.getmueveHelix(), 40.0f, mainWindow.getmueveHeliz());
		pointLights[1].SetFlash(point2Light);
		

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 auxcolor = glm::vec3(0.0f, 0.5f, 1.0f);

		// *** piso ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared front ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(4.0f, -22.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared  back D ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-4.0f, -22.5f, -12.1f));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.695f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared back I ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-4.0f, -22.5f, 12.6f));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.66f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared derecha ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -22.5f, 4.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***  pared izquierda ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -22.5f, -4.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
		// ***  piso escalon ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***   escalon b ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.85f, 7.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***   escalon f ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(1.85f, 7.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***   escalon d ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, 1.85f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ***   escalon i ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, -1.85f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		escalonTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// *** marco de puerta  ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-23.9f, -1.55f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		marco_M.RenderModel();

		// *** puerta ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-24.2f, puerta, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		puerta_M.RenderModel();
		
		// *** piso galeria***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.5f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pgTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		// *** techo galeria***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.5f, 4.50f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pgTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		// *** pared back galeria***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.25f, -52.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.325f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pgTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		// *** pared derecha galeria***
		model = glm::mat4(1.0);
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-37.5f, -15.0f, 1.25f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.325f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		
		// *** pared izquierda galeria***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-37.5f, -15.0f, -1.25f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.325f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();


		// *** torre ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(6.5f, 4.5f, 6.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		torre_M.RenderModel();

		// *** contenedor ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 20.5f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		contenedor_M.RenderModel();
		
		// *** caja 1***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, -1.7f, 6.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 2***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -2.0f, -15.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 3 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -2.0f, -12.9f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f,1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 4 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.9f, -2.0f, -15.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 5 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.1f, -15.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 5 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -2.0f, 15.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 6 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-38.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 7***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-42.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** caja 7***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-49.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caja_M.RenderModel();

		// *** mesa ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -1.9f, -11.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		mesa_M.RenderModel();

		// *** compu ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -11.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		compu_M.RenderModel();

		// *** humo ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -16.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		humo_M.RenderModel();

		// *** jett ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		jett_M.RenderModel();

		// *** tiro 1 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 1.5f, 14.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tiro_M.RenderModel();

		// *** tiro 2 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 1.5f, 14.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tiro_M.RenderModel();

		// *** tiro 3 ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 1.5f, 14.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tiro_M.RenderModel();

		// *** pared interna 1 ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(3.0f, -16.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.7f, 1.0f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		paredTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// *** pared interna 2 ***
		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(10.0f, -13.5f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.0f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		paredTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		cordTexU += 0.005f;
		cordTexV += 0.005f;

		if (cordTexU > 1.0f) {
			cordTexU = 0.0f;
			cordTexV = 0.0f;
		}

		toffset = glm::vec2(cordTexU, cordTexV);
		// *** agua ***
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.2f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		aguaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		glDisable(GL_BLEND);


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

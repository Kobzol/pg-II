#include "demoAsteroids.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"
#include "vao_quad.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"
#include "entity_OBJ.h"
#include "entity_InstancedOBJ.h"

#include "../code/uniform.h"
#include "../code/framebuffer/framebuffer.h"
#include "../code/timer.h"
#include <sceneManager.h>
#include <random>

static int MODEL_ROCK;
static int MODEL_PLANET;
static int SHADER_SIMPLE;
static int SHADER_ROCK;
static int SHADER_PLANET;

static int VAO_INFO[2];
static int VAO_ROCK;
static int VAO_PLANET;

static int TEXTURE_ROCK;
static int TEXTURE_PLANET;
static int MATERIAL_ROCK;

static int INFO_ENTITIES[2];
static std::vector<int> SCENE_ENTITIES;

#define ROCK_AMOUNT (10000)
glm::mat4* originalMatrices;
glm::mat4* rockMatrices;
static GLuint VBO_ROCK_MATRICES;
static Timer rockTimer{ 0.05f };


void DemoAsteroids::initShaders()
{
	addResPath("shaders/");

	SHADER_SIMPLE = m_sceneData->shaderPrograms.size();
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");

	SHADER_ROCK = m_sceneData->shaderPrograms.size();
	initShaderProgram("asteroids.vert", "asteroids.frag");

	SHADER_PLANET = m_sceneData->shaderPrograms.size();
	initShaderProgram("ads_v3_n3_t3.vert", "texture_v3_n3_t3.frag");

	resetResPath();
}

void DemoAsteroids::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("planet/rock.obj"));

	MODEL_ROCK = m_sceneData->models.size();
	m_sceneData->models.push_back(m);

	m = objL.loadModel(getResFile("planet/planet.obj"));

	MODEL_PLANET = m_sceneData->models.size();
	m_sceneData->models.push_back(m);

	resetResPath();
}

static glm::mat4* initModelMatrices()
{
	glm::mat4* modelMatrices = new glm::mat4[ROCK_AMOUNT];
	originalMatrices = new glm::mat4[ROCK_AMOUNT];
	
	GLfloat radius = 10.0f;
	GLfloat offset = 2.5f;
	for (GLuint i = 0; i < ROCK_AMOUNT; i++)
	{
		glm::mat4 model;
		// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)ROCK_AMOUNT * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = displacement * 0.4f; // y value has smaller displacement
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		// 4. Now add to list of matrices
		modelMatrices[i] = model;
		originalMatrices[i] = model;
	}

	return modelMatrices;
}
static void updateMatrices()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ROCK_MATRICES);
	glBufferData(GL_ARRAY_BUFFER, ROCK_AMOUNT * sizeof(glm::mat4), &rockMatrices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void DemoAsteroids::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();

	VAO_INFO[0] = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();

	VAO_INFO[1] = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(vao1);

	VAO* vao = new VAO();
	vao->createFromModel(m_sceneData->models[MODEL_ROCK]);

	rockMatrices = initModelMatrices();

	glGenBuffers(1, &VBO_ROCK_MATRICES);
	updateMatrices();

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ROCK_MATRICES);
	glBindVertexArray(vao->m_object);
	// Vertex Attributes
	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	VAO_ROCK = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(vao);

	vao = new VAO();
	vao->createFromModel(m_sceneData->models[MODEL_PLANET]);

	VAO_PLANET = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(vao);
}

void DemoAsteroids::initTextures()
{
	addResPath("models/planet");
	//Load sprite textures
	GLuint texID;
	FIBITMAP *image = ImageManager::GenericLoader(/*getResFile(*/"Resources/models/planet/rock.jpg"/*)*/, 0);

	//TODO Create Texture:
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(image);
	TEXTURE_ROCK = m_sceneData->textures.size();
	m_sceneData->textures.push_back(texID);

	image = ImageManager::GenericLoader(/*getResFile(*/"Resources/models/planet/planet.png"/*)*/, 0);

	//TODO Create Texture:
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_RGB, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(image);
	TEXTURE_PLANET = m_sceneData->textures.size();
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

void DemoAsteroids::initMaterials()
{
	Material *m = new Material();

	m->setName("White_opaque");
	m->m_diffuse[0] = 1.0f;
	m->m_diffuse[1] = 1.0f;
	m->m_diffuse[2] = 1.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[TEXTURE_ROCK];

	MATERIAL_ROCK = m_sceneData->materials.size();
	m_sceneData->materials.push_back(m);
}

void DemoAsteroids::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[VAO_INFO[0]]);
	e0->init();
	INFO_ENTITIES[0] = m_sceneData->infoEntities.size();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[VAO_INFO[1]]);
	e1->init();
	INFO_ENTITIES[1] = m_sceneData->infoEntities.size();
	m_sceneData->infoEntities.push_back(e1);
}

void DemoAsteroids::initSceneEntities()
{
	EntityInstanced *obj = new EntityInstanced(m_sceneData->models[MODEL_ROCK], m_sceneData->vaos[VAO_ROCK], ROCK_AMOUNT);
	obj->setPosition(0.0f, 0.0, 0.0f);
	obj->setOrientation(0.0f, 0.0f, 0.0f);
	obj->m_material = m_sceneData->materials[MATERIAL_ROCK];
	obj->init();

	SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
	m_sceneData->sceneEntities.push_back(obj);

	Entity_OBJ *planet = new Entity_OBJ(m_sceneData->models[MODEL_PLANET], m_sceneData->vaos[VAO_PLANET]);
	planet->setPosition(0.0f, 0.0, 0.0f);
	planet->setOrientation(0.0f, 0.0f, 0.0f);
	planet->m_material = m_sceneData->materials[MATERIAL_ROCK];
	planet->init();

	SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
	m_sceneData->sceneEntities.push_back(planet);
}

static float angleDelta = 0.0f;
void DemoAsteroids::render()
{
	float delta = SceneManager::GetInstance()->delta;
	rockTimer.update(delta);

	angleDelta += delta;

	Entity *e = nullptr;
	SceneSetting *ss = SceneSetting::GetInstance();

#pragma region Draw Info Entities
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_SIMPLE];
	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	Uniform<glm::mat4>::bind("MVPMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getViewProjectionMatrix());

	for (unsigned int i = 0; i < 2; i++)
	{
		//m_sceneData->infoEntities[INFO_ENTITIES[i]]->draw();
	}

#pragma endregion

	const float radius = 10.0f;
	const float offset = 2.5f;
	float angle = angleDelta;

	if (rockTimer.resetIfReady())
	{
		for (int i = 0; i < ROCK_AMOUNT; i++)
		{
			glm::mat4 model;
			GLfloat angle = ((GLfloat)i / (GLfloat)ROCK_AMOUNT * 360.0f) + angleDelta;
			GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat x = sin(angle) * radius + displacement;
			displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat y = displacement * 0.4f; // y value has smaller displacement
			displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
			GLfloat z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));
			// 2. Scale: Scale between 0.05 and 0.25f
			GLfloat scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));
			// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
			GLfloat rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
			rockMatrices[i] = model;
		}
		updateMatrices();
	}

#pragma region Draw Scene Entities
	for (int i = 0; i < m_sceneData->shaderPrograms.size(); i++)
	{
		ss->m_activeShader = m_sceneData->shaderPrograms[i];
		ss->m_activeShader->enable();

		Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");
		Uniform<glm::mat4>::bind("PMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getProjectionMatrix());
		Uniform<glm::mat4>::bind("VMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getViewMatrix());
	}

	// draw rocks
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_ROCK];
	ss->m_activeShader->enable();

	e = m_sceneData->sceneEntities[SCENE_ENTITIES[0]];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[TEXTURE_ROCK]);
	e->draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	// draw planet
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_PLANET];
	ss->m_activeShader->enable();

	e = m_sceneData->sceneEntities[SCENE_ENTITIES[1]];

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[TEXTURE_PLANET]);
	e->draw();
	glBindTexture(GL_TEXTURE_2D, 0);

#pragma endregion
}

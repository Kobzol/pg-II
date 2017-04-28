#include "demoShadows.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"
#include "vao_quad.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"
#include "entity_OBJ.h"
#include "entity_Selectable.h"
#include "entity_Quad.h"

#include "../code/uniform.h"
#include "../code/framebuffer/framebuffer.h"
#include <sceneManager.h>
#include <random>

static int MODEL_SPHERE;
static int MODEL_PLANE;

static int SHADER_SIMPLE;
static int SHADER_PHONG;
static int SHADER_LIGHTSHADOW;
static int SHADER_QUAD;

static int VAO_INFO[2];
static int VAO_SPHERE;
static int VAO_QUAD;
static int VAO_PLANE;

static int LIGHT_SHADOW;

static int TEXTURE_DIFF_SPHERE;
static int TEXTURE_DIFF_PLANE;

static int MATERIAL_SPHERE;
static int MATERIAL_PLANE;

static int INFO_ENTITIES[2];
static std::vector<int> SCENE_ENTITIES;
static QuadEntity* QUAD_ENTITY;

static Framebuffer* FBO;
static int SHADOW_WIDTH = 2048;
static int SHADOW_HEIGHT = SHADOW_WIDTH;

void DemoShadows::initShaders()
{
	addResPath("shaders/");

	SHADER_SIMPLE = m_sceneData->shaderPrograms.size();
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");

	SHADER_PHONG = m_sceneData->shaderPrograms.size();
	initShaderProgram("shadow_v3_n3_t3.vert", "shadow_v3_n3_t3.frag");

	SHADER_LIGHTSHADOW = m_sceneData->shaderPrograms.size();
	initShaderProgram("lightshadow.vert", "lightshadow.frag");

	SHADER_QUAD = m_sceneData->shaderPrograms.size();
	initShaderProgram("quad_screen.vert", "quad_texture.frag");

	resetResPath();
}

void DemoShadows::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("basic/sphereTexture.obj"));

	MODEL_SPHERE = m_sceneData->models.size();
	m_sceneData->models.push_back(m);

	m = objL.loadModel(getResFile("basic/bigplane.obj"));
	
	MODEL_PLANE = m_sceneData->models.size();
	m_sceneData->models.push_back(m);

	resetResPath();
}

void DemoShadows::initVAOs()
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
	vao->createFromModel(m_sceneData->models[MODEL_SPHERE]);

	VAO_SPHERE = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(vao);

	vao = new VAO();
	vao->createFromModel(m_sceneData->models[MODEL_PLANE]);

	VAO_PLANE = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(vao);

	QuadVAO* quadVAO = new QuadVAO();
	quadVAO->init();

	VAO_QUAD = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(quadVAO);
}

void DemoShadows::initTextures()
{
	addResPath("textures/");
	//Load sprite textures
	GLuint texID;
	FIBITMAP *image = ImageManager::GenericLoader(getResFile("waterDiffuse.bmp"), 0);

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
	TEXTURE_DIFF_SPHERE = m_sceneData->textures.size();
	m_sceneData->textures.push_back(texID);

	image = ImageManager::GenericLoader(getResFile("sand.bmp"), 0);

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
	TEXTURE_DIFF_PLANE = m_sceneData->textures.size();
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

void DemoShadows::initMaterials()
{
	Material *m = new Material();

	m->setName("White_opaque");
	m->m_diffuse[0] = 1.0f;
	m->m_diffuse[1] = 1.0f;
	m->m_diffuse[2] = 1.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[TEXTURE_DIFF_SPHERE];

	MATERIAL_SPHERE = m_sceneData->materials.size();
	m_sceneData->materials.push_back(m);

	m = new Material();

	m->setName("White_opaque");
	m->m_diffuse[0] = 1.0f;
	m->m_diffuse[1] = 1.0f;
	m->m_diffuse[2] = 1.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[TEXTURE_DIFF_PLANE];

	MATERIAL_PLANE = m_sceneData->materials.size();
	m_sceneData->materials.push_back(m);
}

void DemoShadows::initInfoEntities()
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

void DemoShadows::initSceneEntities()
{
	for (int i = 0; i < 1; i++)
	{
		Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[MODEL_SPHERE], m_sceneData->vaos[VAO_SPHERE]);
		obj->setPosition(i * 2, 0.0, 1.2f);
		obj->setOrientation(0.0f, 0.0f, 0.0f);
		obj->m_material = m_sceneData->materials[MATERIAL_SPHERE];
		obj->init();

		SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
		m_sceneData->sceneEntities.push_back(obj);
	}

	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[MODEL_PLANE], m_sceneData->vaos[VAO_PLANE]);
	obj->setPosition(0.0f, 0.0, 0.0f);
	obj->setOrientation(0.0f, 0.0f, 0.0f);
	obj->m_material = m_sceneData->materials[MATERIAL_PLANE];
	obj->init();

	SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
	m_sceneData->sceneEntities.push_back(obj);

	QuadEntity* quad = new QuadEntity(m_sceneData->vaos[VAO_QUAD]);
	quad->setPosition(0.0f, 0.0f, 0.0f);
	quad->setOrientation(0.0f, 0.0f, 0.0f);
	quad->m_material = m_sceneData->materials[MATERIAL_SPHERE];
	quad->init();

	QUAD_ENTITY = quad;
	m_sceneData->sceneEntities.push_back(quad);
}

void DemoShadows::initFBOs()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	FBO = new Framebuffer();
	FBO->createDepthAttachment(SHADOW_WIDTH, SHADOW_HEIGHT);
}

void DemoShadows::initLights()
{
	Light *l = new Light();
	l->m_position[0] = 5.0f;
	l->m_position[1] = 0.0f;
	l->m_position[2] = 3.0f;

	LIGHT_SHADOW = m_sceneData->lights.size();
	m_sceneData->lights.push_back(l);
}

void DemoShadows::drawScene()
{
	SceneSetting* ss = SceneSetting::GetInstance();

	for (size_t i = 0; i < SCENE_ENTITIES.size(); i++)
	{
		Entity* e = m_sceneData->sceneEntities[SCENE_ENTITIES[i]];
		Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, e->m_material->m_diffuseTextureGL);
		e->draw();
	}
}

static float timeCounter = 0.0f;
void DemoShadows::render()
{
	timeCounter += SceneManager::GetInstance()->delta;

	Entity *e = nullptr;
	SceneSetting *ss = SceneSetting::GetInstance();

#pragma region Draw Scene Entities

	for (int i = 0; i < m_sceneData->shaderPrograms.size(); i++)
	{
		ss->m_activeShader = m_sceneData->shaderPrograms[i];
		ss->m_activeShader->enable();

		Light::setShaderUniform(m_sceneData->lights[LIGHT_SHADOW], ss->m_activeShader, "light");
		Uniform<glm::mat4>::bind("PMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getProjectionMatrix());
		Uniform<glm::mat4>::bind("VMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getViewMatrix());
	}

	// shadows
	FBO->bind();
	FBO->setViewport();
	glClear(GL_DEPTH_BUFFER_BIT);

	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_LIGHTSHADOW];
	ss->m_activeShader->enable();

	Light* light = m_sceneData->lights[LIGHT_SHADOW];
	
	float radius = 4.0f;
	glm::vec3 position = glm::vec3(
		std::sin(timeCounter) * radius,
		std::cos(timeCounter) * radius,
		3.0f + std::sin(timeCounter)
	);

	light->m_position[0] = position.x;
	light->m_position[1] = position.y;
	light->m_position[2] = position.z;

	GLfloat near_plane = 0.1f, far_plane = 10.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(light->m_position[0], light->m_position[1], light->m_position[2]),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	Uniform<glm::mat4>::bind("LightSpaceMatrix", ss, lightSpaceMatrix);

	glCullFace(GL_FRONT);
	drawScene();
	glCullFace(GL_BACK);

	FBO->unbind();

	glViewport(0, 0, ss->m_screen[0], ss->m_screen[1]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	/*ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_QUAD];
	ss->m_activeShader->enable();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBO->texture);
	QUAD_ENTITY->draw();
	glBindTexture(GL_TEXTURE_2D, 0);*/

	// scene
	
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_PHONG];
	ss->m_activeShader->enable();
	Uniform<glm::mat4>::bind("LightSpaceMatrix", ss, lightSpaceMatrix);
	Uniform<int>::bind("diffuseTexture", ss, 0);
	Uniform<int>::bind("shadowMap", ss, 1);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, FBO->texture);
	drawScene();
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion
}

#include "demoVoronoi.h"

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
static int SHADER_SIMPLE;
static int SHADER_PHONG;

static int VAO_INFO[2];
static int VAO_SPHERE;
static int VAO_QUAD;
static int SHADER_QUAD_VORONOI;

static int TEXTURE_DIFF_SPHERE;
static int MATERIAL_SPHERE;

static int INFO_ENTITIES[2];
static std::vector<int> SCENE_ENTITIES;
static QuadEntity* QUAD_ENTITY;

static Framebuffer* FBO;
static std::vector<glm::vec2> points;
static int POINTS_COUNT = 30;

float voronoiScale = 10.0f;


void DemoVoronoi::initShaders()
{
	addResPath("shaders/");

	SHADER_SIMPLE = m_sceneData->shaderPrograms.size();
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");

	SHADER_PHONG = m_sceneData->shaderPrograms.size();
	initShaderProgram("ads_v3_n3_t3.vert", "texture_v3_n3_t3.frag");

	SHADER_QUAD_VORONOI = m_sceneData->shaderPrograms.size();
	initShaderProgram("quad_screen.vert", "quad_voronoi.frag");

	resetResPath();
}

void DemoVoronoi::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("basic/sphereTexture.obj"));

	MODEL_SPHERE = m_sceneData->models.size();
	m_sceneData->models.push_back(m);

	resetResPath();
}

void DemoVoronoi::initVAOs()
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

	QuadVAO* quadVAO = new QuadVAO();
	quadVAO->init();

	VAO_QUAD = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(quadVAO);
}

void DemoVoronoi::initTextures()
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

	resetResPath();
}

void DemoVoronoi::initMaterials()
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
}

void DemoVoronoi::initInfoEntities()
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

void DemoVoronoi::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[MODEL_SPHERE], m_sceneData->vaos[VAO_SPHERE]);
	obj->setPosition(0.0f, 0.0, 0.0f);
	obj->setOrientation(0.0f, 0.0f, 0.0f);
	obj->m_material = m_sceneData->materials[MATERIAL_SPHERE];
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

void DemoVoronoi::initFBOs()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	FBO = new Framebuffer();
	FBO->createAttachments(ss->m_screen[0] * 2, ss->m_screen[1] * 2);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> pointsDist(-1.0f, 1.0f);

	for (int i = 0; i < POINTS_COUNT; i++)
	{
		points.push_back(glm::vec2(pointsDist(generator), pointsDist(generator)));
	}
}

static float timeCounter = 0.0f;
void DemoVoronoi::drawVoronoi()
{
	SceneSetting* ss = SceneSetting::GetInstance();

	FBO->bind();

	FBO->setViewport();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_QUAD_VORONOI];
	ss->m_activeShader->enable();

	for (auto& p : points)
	{
		//p.x = 0.5f * std::sin(time * 0.02f + p.x);
		//p.y = 0.5f * std::sin(time * 0.02f + p.y);
	}

	Uniform<std::vector<glm::vec2>>::bind("Points", ss, points);
	Uniform<float>::bind("Time", ss, timeCounter);
	Uniform<float>::bind("VoronoiScale", ss, voronoiScale);

	QUAD_ENTITY->draw();

	FBO->unbind();

	glViewport(0, 0, ss->m_screen[0], ss->m_screen[1]);
	glClearColor(ss->m_clearColor[0], ss->m_clearColor[1], ss->m_clearColor[2], ss->m_clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void DemoVoronoi::render()
{
	timeCounter += SceneManager::GetInstance()->delta;

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

#pragma region Draw Scene Entities

	for (int i = 0; i < m_sceneData->shaderPrograms.size(); i++)
	{
		ss->m_activeShader = m_sceneData->shaderPrograms[i];
		ss->m_activeShader->enable();

		Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");
		Uniform<glm::mat4>::bind("PMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getProjectionMatrix());
		Uniform<glm::mat4>::bind("VMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getViewMatrix());
	}

	drawVoronoi();

	for (int i = 0; i < SCENE_ENTITIES.size(); i++)
	{
		ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_PHONG];
		ss->m_activeShader->enable();

		e = m_sceneData->sceneEntities[SCENE_ENTITIES[i]];
		Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBO->texture);
		e->draw();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
#pragma endregion
}

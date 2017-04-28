#include "demoToon.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"
#include "vao_quad.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"
#include "entity_OBJ.h"

#include "../code/uniform.h"
#include "../code/framebuffer/framebuffer.h"
#include "../code/timer.h"
#include <sceneManager.h>
#include <random>

static int MODEL_TORUS;
static int SHADER_SIMPLE;
static int SHADER_PHONG;

static int VAO_INFO[2];
static int VAO_TORUS;

static int TEXTURE_TORUS;
static int MATERIAL_TORUS;

static int INFO_ENTITIES[2];
static std::vector<int> SCENE_ENTITIES;

void DemoToon::initShaders()
{
	addResPath("shaders/");

	SHADER_SIMPLE = m_sceneData->shaderPrograms.size();
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");

	SHADER_PHONG = m_sceneData->shaderPrograms.size();
	initShaderProgram("ads_v3_n3_t3.vert", "toon_v3_n3_t3.frag");

	resetResPath();
}

void DemoToon::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("basic/torus.obj"));

	MODEL_TORUS = m_sceneData->models.size();
	m_sceneData->models.push_back(m);

	resetResPath();
}

void DemoToon::initVAOs()
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
	vao->createFromModel(m_sceneData->models[MODEL_TORUS]);

	VAO_TORUS = m_sceneData->vaos.size();
	m_sceneData->vaos.push_back(vao);
}

void DemoToon::initTextures()
{
	addResPath("textures/");
	//Load sprite textures
	GLuint texID;
	FIBITMAP *image = ImageManager::GenericLoader(getResFile("waterDIFFUSE.bmp"), 0);

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
	TEXTURE_TORUS = m_sceneData->textures.size();
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

void DemoToon::initMaterials()
{
	Material *m = new Material();

	m->setName("White_opaque");
	m->m_diffuse[0] = 1.0f;
	m->m_diffuse[1] = 1.0f;
	m->m_diffuse[2] = 1.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[TEXTURE_TORUS];

	MATERIAL_TORUS = m_sceneData->materials.size();
	m_sceneData->materials.push_back(m);
}

void DemoToon::initInfoEntities()
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

void DemoToon::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[MODEL_TORUS], m_sceneData->vaos[VAO_TORUS]);
	obj->setPosition(0.0f, 0.0, 0.0f);
	obj->setOrientation(0.0f, 0.0f, 0.0f);
	obj->m_material = m_sceneData->materials[MATERIAL_TORUS];
	obj->init();

	SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
	m_sceneData->sceneEntities.push_back(obj);
}

void DemoToon::drawTorus()
{
	SceneSetting* ss = SceneSetting::GetInstance();
	
	Entity* e = m_sceneData->sceneEntities[SCENE_ENTITIES[0]];
	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

	e->draw();
}

static float deltaAngle = 0.0f;
void DemoToon::render()
{
	float delta = SceneManager::GetInstance()->delta;
	deltaAngle += delta;

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

	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_PHONG];
	ss->m_activeShader->enable();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[TEXTURE_TORUS]);
	drawTorus();
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion
}

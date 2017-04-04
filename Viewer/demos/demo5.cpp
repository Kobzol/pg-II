#include "demo5.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube_V3C4N3.h"
#include "entity_OBJ.h"

#include "../code/uniform.h"
#include "../gl_util.h"

static Entity_OBJ* transparentObjects[2];
static Entity_OBJ* opaqueObjects[1];
static Material* transparentMaterial;
static Material* opaqueMaterial;

#define SHADER_PHONG (0)
#define SHADER_TRANSPARENT (1)


void Demo5::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("ads_v3_n3_t3.vert", "ads_v3_n3_t3.frag");
	initShaderProgram("ads_v3_n3_t3.vert", "blend_v3_n3_t3.frag");
	resetResPath();
}

void Demo5::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("vase/vase.obj"));
	m_sceneData->models.push_back(m);

	resetResPath();
}

void Demo5::initVAOs()
{
	VAO* vao = new VAO();
	vao->createFromModel(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vao);
}

void Demo5::initMaterials()
{
	Material* m = new Material();
	m->setName("Opaque");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 0.0f; m->m_diffuse[2] = 0.0f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.0f;
	m_sceneData->materials.push_back(m);

	opaqueMaterial = m;

	m = new Material();
	m->setName("Transparent");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.0f; m->m_diffuse[1] = 1.0f; m->m_diffuse[2] = 0.0f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.5f;
	m_sceneData->materials.push_back(m);

	transparentMaterial = m;
}

void Demo5::initUBOs()
{

}

void Demo5::initInfoEntities()
{

}

void Demo5::initSceneEntities()
{
	// first transparent
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[0]);
	obj->setPosition(0, 0, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = transparentMaterial;
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
	transparentObjects[0] = obj;

	// middle opaque
	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[0]);
	obj->setPosition(0, 1.5f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = opaqueMaterial;
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
	opaqueObjects[0] = obj;

	// second transparent
	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[0]);
	obj->setPosition(0, 3.0f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = transparentMaterial;
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
	transparentObjects[1] = obj;
}

void Demo5::initLights()
{
	Light* l = new Light();
	l->m_spotExponent = 32.0f;
	this->m_sceneData->lights.push_back(l);
}

void Demo5::render()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	Camera* camera = ss->m_activeCamera;

	glEnable(GL_DEPTH_TEST);

	// opaque
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_PHONG];
	ss->m_activeShader->enable();
	Light::setShaderUniform(this->m_sceneData->lights.at(0), ss->m_activeShader, "light");

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	for (unsigned int i = 0; i < 1; i++)
	{
		Uniform<glm::mat4>::bind("PMatrix", ss, camera->getProjectionMatrix());
		Uniform<glm::mat4>::bind("VMatrix", ss, camera->getViewMatrix());
		Uniform<glm::mat4>::bind("MVMatrix", ss, camera->getViewMatrix());
		Uniform<glm::mat3>::bind("NormalMatrix", ss, camera->getNormalMatrix());
		Material::setShaderUniform(opaqueObjects[i]->m_material, ss->m_activeShader, "material");

		opaqueObjects[i]->draw(i);
	}

	// transparent
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_TRANSPARENT];
	ss->m_activeShader->enable();
	Light::setShaderUniform(this->m_sceneData->lights.at(0), ss->m_activeShader, "light");

	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	for (unsigned int i = 0; i < 2; i++)
	{
		Uniform<glm::mat4>::bind("PMatrix", ss, camera->getProjectionMatrix());
		Uniform<glm::mat4>::bind("VMatrix", ss, camera->getViewMatrix());
		Uniform<glm::mat4>::bind("MVMatrix", ss, camera->getViewMatrix());
		Uniform<glm::mat3>::bind("NormalMatrix", ss, camera->getNormalMatrix());
		Material::setShaderUniform(transparentObjects[i]->m_material, ss->m_activeShader, "material");

		transparentObjects[i]->draw(i);
	}

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	glUseProgram(0);	// musi se vypnout shader, aby drawPixels nešlo pøes fragment shader

	glFlush();

	glReadBuffer(GL_BACK);
	glReadPixels(512, 512, 100, 100, GL_RGBA, GL_UNSIGNED_BYTE, this->data);
	GL_CHECK_ERRORS();

	glFlush();

	glDrawBuffer(GL_BACK);
	glRasterPos3f(-1.0f, -1.0f, 0.5f);
	glDrawPixels(100, 100, GL_RGBA, GL_UNSIGNED_BYTE, this->data);

	glFlush();
}

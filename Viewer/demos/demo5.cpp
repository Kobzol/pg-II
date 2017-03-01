#include "demo5.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube_V3C4N3.h"
#include "entity_OBJ.h"

#include "../code/uniform.h"

#define SHADER_PHONG (0)
#define SHADER_GOOCH (1)

void Demo5::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("ads_v3_n3_t3.vert", "gooch_v3_n3_t3.frag");
	initShaderProgram("ads_v3_n3_t3.vert", "ads_v3_n3_t3.frag");
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
	Material *m = new Material();
	m->setName("White_opaque");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 1.0; m->m_diffuse[2] = 0.8f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.0f;
	m_sceneData->materials.push_back(m);
}

void Demo5::initUBOs()
{

}

void Demo5::initInfoEntities()
{

}

void Demo5::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[0]);
	obj->setPosition(0, 0, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[0]);
	obj->setPosition(0, 1.5f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
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
	ss->m_activeShader = m_sceneData->shaderPrograms[0];
	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	Camera* camera = ss->m_activeCamera;

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
	{
		m_sceneData->infoEntities[i]->draw();
	}
	for (unsigned int i = 0; i < m_sceneData->sceneEntities.size(); i++)
	{
		ss->m_activeShader = m_sceneData->shaderPrograms[i];
		ss->m_activeShader->enable();

		Uniform<glm::mat4>::bind("PMatrix", ss, camera->getProjectionMatrix());
		Uniform<glm::mat4>::bind("VMatrix", ss, camera->getViewMatrix());
		Uniform<glm::mat4>::bind("MVMatrix", ss, camera->getViewMatrix());
		Uniform<glm::mat3>::bind("NormalMatrix", ss, camera->getNormalMatrix());

		Light::setShaderUniform(this->m_sceneData->lights.at(0), ss->m_activeShader, "light");
		Material::setShaderUniform(m_sceneData->sceneEntities[i]->m_material, ss->m_activeShader, "material");

		m_sceneData->sceneEntities[i]->draw(i);
	}
}

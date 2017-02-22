#include "demo3.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube_V3C4N3.h"

#include "../code/uniform.h"

void Demo3::initShaders()
{
	addResPath("shaders/");
	//initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");
	initShaderProgram("lambert_v3_c4_n3.vert", "lambert_v3_c4_n3.frag");
	resetResPath();
}

void Demo3::initVAOs()
{
	VAO_CubeV3C4N3* cube = new VAO_CubeV3C4N3();
	cube->init();
	this->m_sceneData->vaos.push_back(cube);
}

void Demo3::initInfoEntities()
{
	
}

void Demo3::initSceneEntities()
{
	Entity_Cube_V3C4N3* e = new Entity_Cube_V3C4N3(m_sceneData->vaos[0]);
	e->init();
	m_sceneData->sceneEntities.push_back(e);
}

void Demo3::render()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	ss->m_activeShader = m_sceneData->shaderPrograms[0];
	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	Camera* camera = ss->m_activeCamera;

	Uniform<glm::mat4>::bind("PMatrix", ss, camera->getProjectionMatrix());
	Uniform<glm::mat4>::bind("VMatrix", ss, camera->getViewMatrix());
	Uniform<glm::mat4>::bind("MVMatrix", ss, camera->getViewMatrix());
	Uniform<glm::mat3>::bind("NormalMatrix", ss, camera->getNormalMatrix());

	Light::setShaderUniform(this->m_sceneData->lights.at(0), ss->m_activeShader, "light");

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
		m_sceneData->infoEntities[i]->draw();

	for (unsigned int i = 0; i < m_sceneData->sceneEntities.size(); i++)
		m_sceneData->sceneEntities[i]->draw(i);
}


#include "demo10.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"
#include "entity_OBJ.h"

#include "../code/uniform.h"

float displacementFactor = 0.3f;

void Demo10::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");
	initShaderProgram("adsOBJ_v3_n3_t3_displacement.vert", "adsOBJ_v3_n3_t3_displacement.frag", nullptr, "adsOBJ_v3_n3_t3_displacement.cont", "adsOBJ_v3_n3_t3_displacement.eval");
	resetResPath();
}

void Demo10::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("basic/plane.obj"));
	m_sceneData->models.push_back(m);

	resetResPath();
}

void Demo10::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO* vao = new VAO();
	vao->createFromModel(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vao);
}

void Demo10::initTextures()
{
	addResPath("textures/");
	//Load sprite textures
	GLuint texID;
	FIBITMAP *image = ImageManager::GenericLoader(getResFile("stonewallDiffuse.bmp"), 0);

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
	m_sceneData->textures.push_back(texID);

	image = ImageManager::GenericLoader(getResFile("stonewallNormal.bmp"), 0);

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
	m_sceneData->textures.push_back(texID);

	image = ImageManager::GenericLoader(getResFile("stonewallDepth.bmp"), 0);

	//TODO Create Texture:
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_RED, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);
	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(image);
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

void Demo10::initMaterials()
{
	Material *m = new Material();

	m->setName("White_opaque");
	m->m_diffuse[0] = 1.0f;
	m->m_diffuse[1] = 1.0f;
	m->m_diffuse[2] = 1.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[0];
	m_sceneData->materials.push_back(m);
}

void Demo10::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo10::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0.0f, 0.0, 0.0f);
	obj->setOrientation(0.0f, 0.0f, 90.0f);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
}

void drawTesselated(Entity_OBJ* entity)
{
	SceneSetting *ss = SceneSetting::GetInstance();

	glm::mat4 tmp = ss->m_activeCamera->getVM() * entity->m_modelMatrix;
	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&entity->m_modelMatrix[0]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&tmp[0]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix");
	glUniformMatrix3fv(uniform, 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(tmp)))[0]);

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrixObj");
	glUniformMatrix3fv(uniform, 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(entity->m_modelMatrix)))[0]);

	glBindVertexArray(entity->m_vao->m_object);

	glPatchParameteri(GL_PATCH_VERTICES, 3);
	for (unsigned int i = 0; i < entity->m_vao->m_eai->size(); i++)
	{
		glDrawArrays(GL_PATCHES, entity->m_vao->m_eai->at(i).m_startIndex, entity->m_vao->m_eai->at(i).m_noIndices);
	}
	glBindVertexArray(0);
}

void Demo10::render()
{
	Entity *e = nullptr;
	SceneSetting *ss = SceneSetting::GetInstance();

#pragma region Draw Info Entities

	ss->m_activeShader = m_sceneData->shaderPrograms[0];
	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	Uniform<glm::mat4>::bind("MVPMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getViewProjectionMatrix());

	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++)
		m_sceneData->infoEntities[i]->draw();

#pragma endregion

#pragma region Draw Scene Entities

	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();

	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");
	Uniform<glm::mat4>::bind("PMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getProjectionMatrix());
	Uniform<glm::mat4>::bind("VMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getViewMatrix());

	Uniform<int>::bind("texDiffuse", ss->m_activeShader->m_programObject, 0);
	Uniform<int>::bind("texNormal", ss->m_activeShader->m_programObject, 1);
	Uniform<int>::bind("texDepth", ss->m_activeShader->m_programObject, 2);

	Uniform<glm::vec3>::bind("CameraPosition", ss->m_activeShader->m_programObject, ss->m_activeCamera->getPosition());
	Uniform<float>::bind("DisplacementFactor", ss->m_activeShader->m_programObject, displacementFactor);

	e = m_sceneData->sceneEntities[0];
	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[2]);

	drawTesselated(static_cast<Entity_OBJ*>(e));
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion
}

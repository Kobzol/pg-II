#include "demoText.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"
#include "vao_quad.h"
#include "../code/text/vao_text.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"
#include "entity_OBJ.h"
#include "entity_Selectable.h"
#include "entity_Quad.h"

#include "../code/text/text.h"
#include "../code/uniform.h"
#include "../code/framebuffer/framebuffer.h"
#include "../code/text/text_generator.h"
#include "../code/timer.h"
#include <sceneManager.h>

static int MODEL_SPHERE;
static int SHADER_SIMPLE;
static int SHADER_PHONG;
static int SHADER_TEXT;
static int SHADER_QUAD_TEXTURE;

static int VAO_INFO[2];
static int VAO_SPHERE;
static int VAO_QUAD;
static TextVAO* VAO_TEXT;

static int TEXTURE_DIFF_SPHERE;
static int MATERIAL_SPHERE;

static int INFO_ENTITIES[2];
static std::vector<int> SCENE_ENTITIES;
static Entity* ENTITY_SPHERE;
static Camera* ORTHO_CAMERA;

static Text* font;
static Framebuffer* FBO;
static TextGenerator textGenerator(50, 50);
static Timer matrixTextTimer(0.1f);

void DemoText::initShaders()
{
	addResPath("shaders/");

	SHADER_SIMPLE = m_sceneData->shaderPrograms.size();
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");

	SHADER_PHONG = m_sceneData->shaderPrograms.size();
	initShaderProgram("ads_v3_n3_t3.vert", "texture_v3_n3_t3.frag");

	SHADER_TEXT = m_sceneData->shaderPrograms.size();
	initShaderProgram("text.vert", "text.frag");

	SHADER_QUAD_TEXTURE = m_sceneData->shaderPrograms.size();
	initShaderProgram("quad.vert", "quad_texture.frag");

	resetResPath();
}

void DemoText::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("basic/sphereTexture.obj"));

	MODEL_SPHERE = m_sceneData->models.size();
	m_sceneData->models.push_back(m);

	resetResPath();
}

void DemoText::initVAOs()
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

	TextVAO* textVAO = new TextVAO();
	textVAO->init();

	VAO_TEXT = textVAO;
	m_sceneData->vaos.push_back(textVAO);
}

void DemoText::initTextures()
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
	TEXTURE_DIFF_SPHERE = m_sceneData->textures.size();
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

void DemoText::initMaterials()
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

void DemoText::initInfoEntities()
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

void DemoText::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[MODEL_SPHERE], m_sceneData->vaos[VAO_SPHERE]);
	obj->setPosition(0.0f, 0.0f, 0.0f);
	obj->setOrientation(0.0f, 0.0f, 0.0f);
	obj->m_material = m_sceneData->materials[MATERIAL_SPHERE];
	obj->init();

	SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
	m_sceneData->sceneEntities.push_back(obj);

	ENTITY_SPHERE = obj;

	/*QuadEntity* quad = new QuadEntity(m_sceneData->vaos[VAO_QUAD]);
	quad->setPosition(0.0f, 0.0f, 0.0f);
	quad->setOrientation(0.0f, 0.0f, 0.0f);
	quad->m_material = m_sceneData->materials[MATERIAL_SPHERE];
	quad->init();

	SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
	m_sceneData->sceneEntities.push_back(quad);*/
}

void DemoText::initCameras()
{
	SceneInitializer::initCameras();

	ORTHO_CAMERA = new Camera();
	ORTHO_CAMERA->ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.01f, 100.0f);
	ORTHO_CAMERA->setBehavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
	ORTHO_CAMERA->setPosition(glm::vec3(1.0f, 1.0f, 2.0f));
	ORTHO_CAMERA->lookAt(glm::vec3(1.0f, 1.0f, 0.0f));

	m_sceneData->cameras.push_back(ORTHO_CAMERA);
}

void DemoText::initFonts()
{
	font = new Text("Resources/fonts/matrix.ttf");
}

void DemoText::initFBOs()
{
	SceneSetting *ss = SceneSetting::GetInstance();
	FBO = new Framebuffer();
	FBO->createAttachments(ss->m_screen[0] * 2, ss->m_screen[1] * 2);
}

void DemoText::renderText(const std::string& text, float x, float y, float scale, float space, glm::vec3 color)
{
	SceneSetting *ss = SceneSetting::GetInstance();
	Camera* camera = ss->m_activeCamera;
	ss->m_activeCamera = ORTHO_CAMERA;

	ShaderProgram* program = ss->m_activeShader;
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_TEXT];
	ss->m_activeShader->enable();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho(0.0f, (float) ss->m_screen[0], 0.0f, (float) ss->m_screen[1]);

	Uniform<glm::mat4>::bind("TextProjection", ss->m_activeShader->m_programObject, projection);
	Uniform<glm::vec3>::bind("TextColor", ss->m_activeShader->m_programObject, color);
	Uniform<int>::bind("TextTexture", ss->m_activeShader->m_programObject, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO_TEXT->VAO);

	// Iterate through all characters
	for (auto c : text)
	{
		Character ch = font->Characters[c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VAO_TEXT->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		y -= space * scale;//(space >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);

	ss->m_activeShader = program;
	ss->m_activeShader->enable();
	ss->m_activeCamera = camera;
}

void DemoText::render()
{
	float delta = SceneManager::GetInstance()->delta * 0.5f;
	matrixTextTimer.update(delta);

	Entity *e = nullptr;
	SceneSetting *ss = SceneSetting::GetInstance();

#pragma region Draw Scene Entities

	for (int i = 0; i < m_sceneData->shaderPrograms.size(); i++)
	{
		ss->m_activeShader = m_sceneData->shaderPrograms[i];
		ss->m_activeShader->enable();

		Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");
		Uniform<glm::mat4>::bind("PMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getProjectionMatrix());
		Uniform<glm::mat4>::bind("VMatrix", ss->m_activeShader->m_programObject, ss->m_activeCamera->getViewMatrix());
	}

	if (matrixTextTimer.resetIfReady())
	{
		FBO->bind();

		FBO->setViewport();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		float x = 0.0f;
		float y = 0.0f;
		float scale = 0.2f;
		float spaceVertical = (ss->m_screen[1] / (textGenerator.rows)) * (25.0f * scale);
		float spaceHorizontal = (ss->m_screen[0] / (textGenerator.cols)) * (6.0f * scale);

		textGenerator.move();

		for (auto& col : textGenerator.matrix)
		{
			renderText(col, x, ss->m_screen[1] - y, scale, spaceVertical, glm::vec3(0.0f, 0.6f, 0.0f));
			x += spaceHorizontal;
		}

		FBO->unbind();

		glViewport(0, 0, ss->m_screen[0], ss->m_screen[1]);
		glClearColor(ss->m_clearColor[0], ss->m_clearColor[1], ss->m_clearColor[2], ss->m_clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

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

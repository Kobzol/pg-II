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
#include "../code/cycle.h"
#include <sceneManager.h>

static int MODEL_SPHERE;
static int SHADER_SIMPLE;
static int SHADER_PHONG;
static int SHADER_TEXT;
static int SHADER_QUAD_INTERFERENCE;
static int SHADER_QUAD_BLEND;

static int VAO_INFO[2];
static int VAO_SPHERE;
static int VAO_QUAD;
static TextVAO* VAO_TEXT;

static int TEXTURE_DIFF_SPHERE;
static int MATERIAL_SPHERE;

static int INFO_ENTITIES[2];
static std::vector<int> SCENE_ENTITIES;
static Entity* ENTITY_SPHERE;
static QuadEntity* ENTITY_QUAD;
static Camera* ORTHO_CAMERA;

static Text* font;
#define FBO_COUNT (3)
static Framebuffer* FBOS[FBO_COUNT];
static int FBO_TEXT = 0;
static int FBO_EFFECT = 1;
static int FBO_ACCUM = 2;
static TextGenerator* textGenerator;

const int TEXT_HEIGHT = 1024;
const int TEXT_WIDTH = TEXT_HEIGHT;
static float TEXT_SCALE = 0.3f;
static float TEXT_SPACE_VERTICAL;
static float TEXT_SPACE_HORIZONTAL;
static float TEXT_COLS = 80;
static float TEXT_ACCUM_DECAY_SPEED = 1.0f;
#define DRAW_INTERFERENCE
#define DRAW_ACCUM

float textSpeed = 50.0f;
float textInterference = 0.0f;

static Cycle interferenceCycle{ 2.0f, 0.4f };

void DemoText::initShaders()
{
	addResPath("shaders/");

	SHADER_SIMPLE = m_sceneData->shaderPrograms.size();
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");

	SHADER_PHONG = m_sceneData->shaderPrograms.size();
	initShaderProgram("ads_v3_n3_t3.vert", "texture_v3_n3_t3.frag");

	SHADER_TEXT = m_sceneData->shaderPrograms.size();
	initShaderProgram("text.vert", "text.frag");

	SHADER_QUAD_INTERFERENCE = m_sceneData->shaderPrograms.size();
	initShaderProgram("quad_screen.vert", "quad_text_interference.frag");

	SHADER_QUAD_BLEND = m_sceneData->shaderPrograms.size();
	initShaderProgram("quad_screen.vert", "quad_text_blend.frag");

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
	/*Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[MODEL_SPHERE], m_sceneData->vaos[VAO_SPHERE]);
	obj->setPosition(0.0f, 0.0f, 0.0f);
	obj->setOrientation(0.0f, 0.0f, 0.0f);
	obj->m_material = m_sceneData->materials[MATERIAL_SPHERE];
	obj->init();

	SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
	m_sceneData->sceneEntities.push_back(obj);

	ENTITY_SPHERE = obj;*/

	QuadEntity* quad = new QuadEntity(m_sceneData->vaos[VAO_QUAD]);
	quad->setPosition(0.0f, 0.0f, 0.0f);
	quad->setOrientation(0.0f, 0.0f, 0.0f);
	quad->m_material = m_sceneData->materials[MATERIAL_SPHERE];
	quad->init();

	SCENE_ENTITIES.push_back(m_sceneData->sceneEntities.size());
	m_sceneData->sceneEntities.push_back(quad);

	ENTITY_QUAD = quad;
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
	
	TEXT_SPACE_VERTICAL = TEXT_HEIGHT / (55.0f * TEXT_SCALE);
	TEXT_SPACE_HORIZONTAL = (TEXT_WIDTH / TEXT_COLS);
	textGenerator = new TextGenerator(TEXT_COLS, TEXT_HEIGHT, TEXT_SPACE_VERTICAL / 4.0f);
}

void DemoText::initFBOs()
{
	SceneSetting *ss = SceneSetting::GetInstance();

	for (int i = 0; i < FBO_COUNT; i++)
	{
		FBOS[i] = new Framebuffer();
		FBOS[i]->createAttachments(TEXT_WIDTH, TEXT_HEIGHT);
	}
}

void DemoText::renderText(const TextColumn& text, float x, float y, float scale, float space)
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
	Uniform<int>::bind("TextTexture", ss->m_activeShader->m_programObject, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO_TEXT->VAO);

	// Iterate through all characters
	for (auto& c : text.particles)
	{
		if (c.character != ' ')
		{
			Character ch = font->Characters[c.character];

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

			Uniform<float>::bind("TextAlpha", ss, c.alpha);
			Uniform<glm::vec3>::bind("TextColor", ss, c.color);

			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		}
		y -= space * scale;//(space >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);

	ss->m_activeShader = program;
	ss->m_activeShader->enable();
	ss->m_activeCamera = camera;
}

float DemoText::updateText(float delta)
{
	textGenerator->update(delta * textSpeed);
	float interferenceRatio = 0.0f;
	if (interferenceCycle.isActive(1))
	{
		interferenceRatio = interferenceCycle.getRatio();
		this->lastInterference = true;
	}
	else if (this->lastInterference)
	{
		this->lastInterference = false;
		interferenceCycle.timers[0] = Timer(this->interWaitDistribution(this->generators[0]));
		interferenceCycle.timers[1] = Timer(this->interApplyDistribution(this->generators[1]));
		this->currentEffect = this->effectDistribution(this->generators[2]);
		std::cerr << this->currentEffect << std::endl;
	}

	if (textInterference > 0.0f)
	{
		interferenceRatio = textInterference;
	}

	return interferenceRatio;
}

static void enableFBO(int fbo)
{
	FBOS[fbo]->bind();
	FBOS[fbo]->setViewport();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
static void disableFBO(int fbo)
{
	SceneSetting *ss = SceneSetting::GetInstance();
	FBOS[fbo]->unbind();
	glViewport(0, 0, ss->m_screen[0], ss->m_screen[1]);
}

void DemoText::drawText()
{
	enableFBO(FBO_TEXT);

	float x = 0.0f;
	for (auto& col : textGenerator->matrix)
	{
		renderText(col, x, TEXT_HEIGHT - col.y, TEXT_SCALE, TEXT_SPACE_VERTICAL);
		x += TEXT_SPACE_HORIZONTAL;
	}

	disableFBO(FBO_TEXT);
}
void DemoText::drawEffect(float interferenceRatio, int interferenceEffect, int fbo)
{
	enableFBO(fbo);

	SceneSetting *ss = SceneSetting::GetInstance();
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_QUAD_INTERFERENCE];
	ss->m_activeShader->enable();

	Uniform<float>::bind("InterferenceRatio", ss, interferenceRatio);
	Uniform<int>::bind("InterferenceEffect", ss, interferenceEffect);
	Uniform<int>::bind("Frame", ss, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOS[FBO_TEXT]->texture);
	ENTITY_QUAD->draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	disableFBO(fbo);
}
void DemoText::drawScreen(float delta)
{
	SceneSetting *ss = SceneSetting::GetInstance();
	ss->m_activeShader = m_sceneData->shaderPrograms[SHADER_QUAD_BLEND];
	ss->m_activeShader->enable();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Uniform<float>::bind("AccumDecayRatio", ss, this->accumDecayRatio);
	Uniform<int>::bind("Frame", ss, 0);
	Uniform<int>::bind("FrameAccum", ss, 1);

	for (int i = 1; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE0 + (i - 1));
		glBindTexture(GL_TEXTURE_2D, FBOS[i]->texture);
	}
	
	ENTITY_QUAD->draw();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DemoText::render()
{
	float delta = SceneManager::GetInstance()->delta;
	interferenceCycle.update(delta);

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

	// update text
	this->accumDecayRatio -= delta * TEXT_ACCUM_DECAY_SPEED;
	this->accumDecayRatio = std::max(0.0f, this->accumDecayRatio);

	bool effectPeak = this->lastInterference;
	int effect = this->currentEffect;
	float interferenceRatio = this->updateText(delta);

#ifndef DRAW_INTERFERENCE
	interferenceRatio = 0.0f;
#endif

	// draw text
	this->drawText();

	// draw interference
	this->drawEffect(interferenceRatio, this->currentEffect, FBO_EFFECT);

#ifdef DRAW_ACCUM
	if (effect == 0 && textInterference == 0.0f && effectPeak && effectPeak != this->lastInterference)
	{
		this->accumDecayRatio = 1.0f;
		this->drawEffect(1.0f, effect, FBO_ACCUM);
	}
#endif

	// draw screen
	this->drawScreen(delta);
#pragma endregion
}

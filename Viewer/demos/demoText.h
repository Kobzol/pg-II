#pragma once

#include <sceneInitializer.h>
#include <string>
#include <glm.hpp>
#include <random>

#include "../code/text/text_generator.h"

class DemoText : public SceneInitializer
{
private:
	void initShaders();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initModels();
	void initInfoEntities();
	void initSceneEntities();
	void initFonts();
	void initCameras();
	void initFBOs();

	void initLights()
	{
		
	}

	void renderText(const TextColumn& text, float x, float y, float scale, float space);
	float updateText(float delta);

	void drawText();
	void drawEffect(float interferenceRatio, int fbo);
	void drawScreen(float delta);

	default_random_engine generators[2];
	uniform_real_distribution<float> interWaitDistribution{ 2.0f, 6.0f };
	uniform_real_distribution<float> interApplyDistribution{ 0.2f, 0.4f };
	bool lastInterference = false;
	float accumDecayRatio = 0.0f;

public:
	DemoText(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
	glm::vec4 getClearColor() override
	{
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
};

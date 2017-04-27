#pragma once

#include <string>
#include <random>
#include <ctime>

#include <glm.hpp>

#include <sceneInitializer.h>
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
	void drawEffect(float interferenceRatio, int interferenceEffect, int fbo);
	void drawScreen(float delta);

	default_random_engine generators[3];
	uniform_real_distribution<float> interWaitDistribution{ 2.0f, 3.5f };
	uniform_real_distribution<float> interApplyDistribution{ 0.3f, 0.4f };
	uniform_int_distribution<int> effectDistribution{ 0, 2 };
	bool lastInterference = false;
	float accumDecayRatio = 0.0f;
	int currentEffect = 0;

public:
	DemoText(SceneData *sdPtr) : SceneInitializer(sdPtr)
	{
		for (int i = 0; i < 3; i++)
		{
			this->generators[i].seed((unsigned int) time(nullptr));
		}
	}

	void render();
	glm::vec4 getClearColor() override
	{
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
};

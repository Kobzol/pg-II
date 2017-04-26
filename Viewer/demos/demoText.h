#pragma once

#include <sceneInitializer.h>
#include <string>
#include <glm.hpp>

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

public:
	DemoText(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
	glm::vec4 getClearColor() override
	{
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
};

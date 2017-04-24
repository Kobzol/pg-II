#pragma once

#include <sceneInitializer.h>
#include <string>
#include <glm.hpp>

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

	void renderText(const std::string& text, float x, float y, float scale, float space, glm::vec3 color);

public:
	DemoText(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

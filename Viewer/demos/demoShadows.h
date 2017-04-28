#pragma once

#include <sceneInitializer.h>

class DemoShadows : public SceneInitializer
{
private:
	void initShaders();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initModels();
	void initInfoEntities();
	void initSceneEntities();
	void initFBOs();
	void initLights();

	void drawScene();

public:
	DemoShadows(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

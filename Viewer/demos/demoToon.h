#pragma once

#include <sceneInitializer.h>

class DemoToon : public SceneInitializer
{
private:
	void initShaders();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initModels();
	void initInfoEntities();
	void initSceneEntities();

	void drawTorus();

public:
	DemoToon(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

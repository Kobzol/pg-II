#pragma once

#include <sceneInitializer.h>

class DemoGyroscope : public SceneInitializer
{
private:
	void initShaders();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initModels();
	void initInfoEntities();
	void initSceneEntities();

public:
	DemoGyroscope(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

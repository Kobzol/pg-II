#pragma once

#include <sceneInitializer.h>

class DemoExplode : public SceneInitializer
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
	DemoExplode(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

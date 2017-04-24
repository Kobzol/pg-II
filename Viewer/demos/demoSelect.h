#pragma once

#include <sceneInitializer.h>

class DemoSelect : public SceneInitializer
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
	DemoSelect(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

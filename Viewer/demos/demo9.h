#pragma once

#include <sceneInitializer.h>

class Demo9 : public SceneInitializer
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
	Demo9(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

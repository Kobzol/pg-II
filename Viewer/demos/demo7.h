#pragma once

#include <sceneInitializer.h>

class Demo7 : public SceneInitializer
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
	Demo7(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

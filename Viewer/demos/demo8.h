#pragma once

#include <sceneInitializer.h>

class Demo8 : public SceneInitializer
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
	Demo8(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

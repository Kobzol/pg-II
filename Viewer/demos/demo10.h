#pragma once

#include <sceneInitializer.h>

class Demo10 : public SceneInitializer
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
	Demo10(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};
#pragma once

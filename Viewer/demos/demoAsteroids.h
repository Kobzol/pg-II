#pragma once

#include <sceneInitializer.h>

class DemoAsteroids : public SceneInitializer
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
	DemoAsteroids(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

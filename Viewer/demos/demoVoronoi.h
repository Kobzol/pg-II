#pragma once

#include <sceneInitializer.h>

class DemoVoronoi : public SceneInitializer
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

	void drawVoronoi();

public:
	DemoVoronoi(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

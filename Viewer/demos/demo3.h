#pragma once

#include <sceneInitializer.h>

class Demo3 : public SceneInitializer
{
private:
	void initShaders();
	void initVAOs();
	void initUBOs() override;
	void initInfoEntities();
	void initSceneEntities();

public:
	Demo3(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

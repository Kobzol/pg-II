#pragma once

#include <sceneInitializer.h>

class Demo4 : public SceneInitializer
{
private:
	void initShaders() override;
	void initVAOs() override;
	void initUBOs() override;
	void initModels() override;
	void initMaterials() override;
	void initInfoEntities() override;
	void initSceneEntities() override;
	void initLights() override;

public:
	Demo4(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

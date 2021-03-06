#pragma once

#include <sceneInitializer.h>

class Demo5 : public SceneInitializer
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

	char* data = nullptr;

public:
	Demo5(SceneData *sdPtr) : SceneInitializer(sdPtr)
	{
		this->data = new char[100 * 100 * 4];
	}

	void render();
};

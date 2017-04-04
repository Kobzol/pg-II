#ifndef __DEMO6_H_
#define __DEMO6_H_

#include <sceneInitializer.h>

class Demo6 : public SceneInitializer
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
	Demo6(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif

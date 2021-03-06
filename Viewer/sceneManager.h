////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	sceneManager.h
//
// summary:	Declaration and implementation of SceneManager class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __SCENEMANAGER_H_
#define __SCENEMANAGER_H_

#include <singletonDestroyer.h>
#include <dynArray.h>
#include <AntTweakBar.h>

#include "sceneData.h"
#include "sceneSetting.h"
#include <sceneInitializer.h>
#include "guiManager.h"

class SceneManager;
class Entity;
class Mouse;

class SceneManager
{
private:

	static SceneManager *SINGLETON_INSTANCE;
	static SingletonDestroyer<SceneManager> destroyer;

	//GUI
	GUIManager *m_guiManager;
	SceneInitializer *m_sceneInitializer;

public:
	SceneSetting *m_sceneSetting;
	SceneData *m_sceneData;

	unsigned int m_fpsCounter;
	GLuint	m_mainWindow;
	char m_windowsTitle[512];

	unsigned int deltaTimer = 0;
	float delta;

private: 
	bool m_isRunning;
	static char resourcePath[1024];
	static char resourceFile[1024];


	SceneManager();

public:
	~SceneManager();

	static SceneManager* GetInstance();

	void Init(int argc, char* argv[]);
	void Close();
	void Display();
	void Redisplay();
	void TimeFunc(int value);
	void Idle();
	void Reshape(int width, int height);
	void KeyTyped(unsigned char key, int x, int y);
	void InputKey(int key, int x, int y);

	//STATIC CALLBACKS
	static void	CloseCB() { SceneManager::GetInstance()->Close(); }
	static void	DisplayCB() { SceneManager::GetInstance()->Display(); }
	static void	RedisplayCB() { SceneManager::GetInstance()->Redisplay(); }
	static void	TimeFuncCB(int value) { SceneManager::GetInstance()->TimeFunc(value); }
	static void	IdleCB() { SceneManager::GetInstance()->Idle(); }
	static void	ReshapeCB(int width, int height) { SceneManager::GetInstance()->Reshape(width, height); }
	static void	KeyTypedCB(unsigned char key, int x, int y) { SceneManager::GetInstance()->KeyTyped(key,x,y); }
	static void	InputKeyCB(int key, int x, int y) { SceneManager::GetInstance()->InputKey(key,x,y); };

	void addEntity(Entity *e, const unsigned int shaderProgramID = 0);
	void addMenu(TwBar* _twMenu) { m_guiManager->m_bars.push_back(_twMenu); }
	void start();
	void stop();

private:
	void updateCameraPerspective(Camera* c, bool firstInit);
};


#endif
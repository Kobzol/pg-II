////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	SceneGUI.h
//
// summary:	Declaration and implementation of SceneGUI class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __SCENEGUI_H_
#define __SCENEGUI_H_

#include <AntTweakBar.h>
#include <iostream>

extern int renderShader;

static class SceneGUI
{
public:
	static inline TwBar* createBar();

private:
	static inline void TW_CALL toggleFullscreen(void* tw_satisfy);
} _SceneGUI;

inline void TW_CALL SceneGUI::toggleFullscreen(void* tw_satisfy)
{ 
	glutFullScreenToggle();
}

void TW_CALL toggleShader(void* tw_satisfy)
{
	if (renderShader == 1) renderShader = 2;
	else renderShader = 1;
}

inline TwBar* SceneGUI::createBar()
{
	TwBar* bar = TwNewBar("sceneBar");
	TwDefine("sceneBar                 "
		"size          = '200 200'     "
		"position      = '20 20'      "
		"color         = '0 0 0'  "
		"alpha         = 50           "
		"label         = 'Scene'  "
		"resizable     = False         "
		"fontresizable = True         "
		"iconifiable   = True          ");

	TwAddButton(bar, "Fullscreen", toggleFullscreen, NULL,
		"group = 'Screen' "
		"label = 'Toggle Fullscreen' "
		"help  = 'Toggle Fullscreen' ");

	TwAddButton(bar, "Toggle", toggleShader, NULL,
		"group = 'Screen' "
		"label = 'Toggle shader' "
		"help  = 'Toggle shader' ");
	return bar;
}

#endif

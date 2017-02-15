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
	return bar;
}



#endif
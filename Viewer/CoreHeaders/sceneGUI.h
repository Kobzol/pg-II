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
extern float parallaxScale;
extern float parallaxBias;
extern float parallaxInvertDepth;
extern float parallaxEffectHeight;
extern float displacementFactor;

extern unsigned int activeDemo;
extern float voronoiScale;

extern float textSpeed;
extern float textInterference;

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

void TW_CALL toggleParallaxInvertDepth(void* tw_satisfy)
{
	parallaxInvertDepth = 1.0f - parallaxInvertDepth;
	std::cerr << parallaxInvertDepth << std::endl;
}

inline TwBar* SceneGUI::createBar()
{
	TwBar* bar = TwNewBar("sceneBar");
	TwDefine("sceneBar                 "
		"size          = '200 250'     "
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
	TwAddVarRW(bar, "ParallaxScale", TW_TYPE_FLOAT, &parallaxScale, " min=0 max=1.0 step=0.01 label='Parallax scale' ");
	TwAddVarRW(bar, "ParallaxBias", TW_TYPE_FLOAT, &parallaxBias, " min=0 max=1.0 step=0.01 label='Parallax bias' ");
	TwAddVarRW(bar, "ParallaxEffectHeight", TW_TYPE_FLOAT, &parallaxEffectHeight, " min=0 max=100.0 step=1.0 label='Parallax effect height' ");
	TwAddButton(bar, "ParallaxInvertDepth", toggleParallaxInvertDepth, NULL,
		"label = 'Parallax invert depth' "
		"help  = 'Parallax invert depth' ");

	TwAddVarRW(bar, "DisplacementFactor", TW_TYPE_FLOAT, &displacementFactor, " min=0 max=2.0 step=0.1 label='Tessellation displacement factor' ");

	TwAddVarRW(bar, "VoronoiScale", TW_TYPE_FLOAT, &voronoiScale, " min=1.0 max=100.0 step=1.0 label='Voronoi scale' ");

	TwAddVarRW(bar, "TextSpeed", TW_TYPE_FLOAT, &textSpeed, " min=1.0 max=250.0 step=10.0 label='Text speed' ");
	TwAddVarRW(bar, "TextInterference", TW_TYPE_FLOAT, &textInterference, " min=0.0 max=4.0 step=0.1 label='Text interference' ");

	TwAddVarRW(bar, "ActiveDemo", TW_TYPE_UINT32, &activeDemo, " min=0 max=4 step=1 label='Active demo'");

	return bar;
}

#endif

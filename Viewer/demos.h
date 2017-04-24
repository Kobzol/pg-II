#ifndef __DEMOS_H_
#define __DEMOS_H_

#include <vector>

#include "demos/demo2.h"
#include "demos/demo3.h"
#include "demos/demo4.h"
#include "demos/demo5.h"
#include "demos/demo6.h"
#include "demos/demo7.h"
#include "demos/demo8.h"
#include "demos/demo9.h"
#include "demos/demo10.h"

#include "demos/demoSelect.h"
#include "demos/demoText.h"
#include "demos/demoExplode.h"
#include "demos/demoVoronoi.h"

//#define LOAD_DEMO DemoSelect(m_sceneData)

#define DEMOS(data) { new DemoText(data), new DemoVoronoi(data), new DemoSelect(data), new DemoExplode(data) }

#endif

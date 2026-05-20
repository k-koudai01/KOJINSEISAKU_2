#pragma once
#include "UIBase.h"
#include "appframe.h"
class UIRender : public UIBase
{
	static void DrawPanel(int x, int y, int w, int h, unsigned int bg, unsigned int border);
	static void DrawGauge(int x, int y, int w, int h, float rate, unsigned int back, unsigned int fill, unsigned int border);
};


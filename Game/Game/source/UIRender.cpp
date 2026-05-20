#include "UIRender.h"

void UIRender::DrawPanel(int x, int y, int w, int h, unsigned int bg, unsigned int border)
{
	DrawBox(x, y, x + w, y + h, bg, TRUE);
	DrawBox(x, y, x + w, y + h, border, FALSE);
}

void UIRender::DrawGauge(int x, int y, int w, int h, float rate, unsigned int back, unsigned int fill, unsigned int border)
{
	if(rate < 0.0f) rate = 0.0f;
	if(rate > 1.0f) rate = 1.0f;

	DrawBox(x, y, x + w, y + h, back, TRUE);
	int fillW = static_cast<int>(w * rate);
	DrawBox(x, y, x + fillW, y + h, fill, TRUE);
	DrawBox(x, y, x + w, y + h, border, FALSE);
}
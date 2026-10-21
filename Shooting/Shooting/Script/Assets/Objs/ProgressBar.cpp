#include "ProgressBar.h"

ProgressBar::ProgressBar(VEC2 pos)
	: GameObject(pos),
	  _background("rsc/imgs/ProgressBar/Bar_6.png", pos),
	  _fill("rsc/imgs/ProgressBar/Bar_0.png", pos)
{
}

void ProgressBar::SetProgress(float progress)
{
	_progress = progress < 0.0f ? 0.0f : (progress > 1.0f ? 1.0f : progress);
}

void ProgressBar::Render()
{
	_background.Print();
	_fill.PrintHorizontal(_progress);
}

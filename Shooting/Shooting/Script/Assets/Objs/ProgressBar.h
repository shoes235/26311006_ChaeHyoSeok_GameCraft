#pragma once

#include "GameObject.h"
#include "Texture.h"

class ProgressBar : public GameObject
{
private:
	Texture _background;
	Texture _fill;
	float _progress = 1.0f;
public :
	ProgressBar(VEC2 Pos);

	void SetProgress(float progress);
	void Render() override;
};


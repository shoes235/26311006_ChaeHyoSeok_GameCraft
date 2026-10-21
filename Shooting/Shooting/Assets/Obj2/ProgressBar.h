#pragma once

#include "GameObject.h"
#include <memory>
#include <string>
using std::string;

class ProgressBar : public GameObject
{
private:
	AnimationClip* _progressClip;

	float _maxTime;
	float _remainTime;
	int _frame = 0;
	bool _manualProgress = false;
public :
	ProgressBar(VEC2 Pos);

	void SetTime(float time);
	void SetProgress(float progress);
	void Render() override;
};


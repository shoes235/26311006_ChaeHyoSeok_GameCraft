#pragma once
#include "GameObject.h"
#include <string>
#include <vector>

class Particle : public GameObject
{
private:
	float _lifeTime;
	float _maxLifeTime;

public:
	Particle(VEC2 pos, const std::vector<std::string>& frames,
		float frameDuration = 0.08f, float scale = 1.0f);
	void Update(float delta);
	void Reset(VEC2 pos);
	bool IsAlive() const { return _lifeTime > .0f; }
};

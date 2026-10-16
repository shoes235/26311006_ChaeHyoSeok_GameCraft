#pragma once
#include <glc2d.h>

class Texture;

class Bullet
{
private:
	VEC2 _pos;
	VEC2 _velocity;
	float _radius;

public:
	Bullet(VEC2 pos, VEC2 velocity, float radius = 4.0f);
	bool Update(float delta);
	void Render(Texture& texture) const;
	VEC2 GetPos() const { return _pos; }
	VEC2 GetVelocity() const { return _velocity; }
	float GetRadius() const { return _radius; }
};

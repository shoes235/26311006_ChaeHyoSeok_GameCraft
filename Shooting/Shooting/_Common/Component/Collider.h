#pragma once

#include <glc2d.h>

class Collider
{
public:
	explicit Collider(float radius = 0.0f) : Radius(radius) {}
	bool IsEntered(VEC2 position, const Collider& other, VEC2 otherPosition) const;

	float Radius;
};

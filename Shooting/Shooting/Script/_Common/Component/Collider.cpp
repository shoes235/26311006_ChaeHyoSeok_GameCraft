#include "Collider.h"

bool Collider::IsEntered(VEC2 firstPos, const Collider& other, VEC2 secondPos) const
{
	const float dx = firstPos.x - secondPos.x;
	const float dy = firstPos.y - secondPos.y;
	const float radius = Radius + other.Radius;
	return dx * dx + dy * dy <= radius * radius;
}

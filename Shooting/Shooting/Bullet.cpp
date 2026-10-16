#include "Bullet.h"
#include "Texture.h"

Bullet::Bullet(VEC2 pos, VEC2 velocity, float radius)
	: _pos(pos), _velocity(velocity), _radius(radius)
{
}

bool Bullet::Update(float delta)
{
	_pos.x += _velocity.x * delta;
	_pos.y += _velocity.y * delta;
	return _pos.x >= -20.0f && _pos.x <= g2_GetScnW() + 20.0f
		&& _pos.y >= -20.0f && _pos.y <= g2_GetScnH() + 20.0f;
}

void Bullet::Render(Texture& texture) const
{
	texture.SetPos(_pos);
	texture.Print();
}

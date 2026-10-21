#include "Bullet.h"
#include "Texture.h"

Bullet::Bullet(VEC2 pos, VEC2 velocity, float radius)
	: _pos(pos), _velocity(velocity), _collider(radius), _active(true), _grazed(false)
{
}

void Bullet::Reset(VEC2 pos, VEC2 velocity, float radius)
{
	_pos = pos;
	_velocity = velocity;
	_collider.Radius = radius;
	_active = true;
	_grazed = false;
}

bool Bullet::Update(float delta)
{
	if (!_active) return false;
	_pos.x += _velocity.x * delta;
	_pos.y += _velocity.y * delta;
	_active = _pos.x >= -20.0f && _pos.x <= g2_GetScnW() + 20.0f
		&& _pos.y >= -20.0f && _pos.y <= g2_GetScnH() + 20.0f;
	return _active;
}

void Bullet::Render(Texture& texture) const
{
	if (!_active) return;
	texture.SetPos(_pos);
	texture.Print();
}

void BulletPool::Spawn(VEC2 pos, VEC2 velocity, float radius)
{
	for (Bullet& bullet : _bullets)
	{
		if (!bullet.IsActive())
		{
			bullet.Reset(pos, velocity, radius);
			return;
		}
	}
	_bullets.emplace_back(pos, velocity, radius);
}

void BulletPool::Update(float delta)
{
	for (Bullet& bullet : _bullets)
		if (bullet.IsActive()) bullet.Update(delta);
}

void BulletPool::Render(Texture& texture) const
{
	for (const Bullet& bullet : _bullets)
		if (bullet.IsActive()) bullet.Render(texture);
}

void BulletPool::Clear()
{
	for (Bullet& bullet : _bullets) bullet.Deactivate();
}

#pragma once
#include <glc2d.h>
#include <vector>
#include "Collider.h"

class Texture;

class Bullet
{
private:
	VEC2 _pos;
	VEC2 _velocity;
	Collider _collider;
	bool _active;
	bool _grazed;

public:
	Bullet(VEC2 pos, VEC2 velocity, float radius = 4.0f);
	void Reset(VEC2 pos, VEC2 velocity, float radius = 4.0f);
	bool Update(float delta);
	void Render(Texture& texture) const;
	void Deactivate() { _active = false; }
	bool IsActive() const { return _active; }
	bool HasGrazed() const { return _grazed; }
	void MarkGrazed() { _grazed = true; }
	VEC2 GetPos() const { return _pos; }
	VEC2 GetVelocity() const { return _velocity; }
	const Collider& GetCollider() const { return _collider; }
};

class BulletPool
{
private:
	std::vector<Bullet> _bullets;

public:
	BulletPool() { _bullets.reserve(64); }
	void Spawn(VEC2 pos, VEC2 velocity, float radius = 4.0f);
	void Update(float delta);
	void Render(Texture& texture) const;
	void Clear();
	std::vector<Bullet>& GetBullets() { return _bullets; }
};

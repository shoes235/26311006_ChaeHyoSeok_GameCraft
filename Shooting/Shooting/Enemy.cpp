#include "Enemy.h"
#include <cmath>

namespace
{
	const float PI = 3.14159265358979323846f;
}

Enemy::Enemy(VEC2 pos, const EnemyConfig& config)
	: GameObject(pos), _config(config), _hp(config.hp), _maxHp(config.hp), _radius(config.radius),
	_fireCooldown(config.initialFireDelay),
	_bulletTexture(config.bulletTexturePath, VEC2(0.0f, 0.0f))
{
	auto clip = std::make_unique<AnimationClip>("Idle", 0.1f, true);
	auto texture = std::make_unique<Texture>(_config.spritePath, pos);
	texture->Load();
	clip->AddTexture(std::move(texture));
	_anim->AddClip("Idle", std::move(clip));
	_anim->ChangeClip("Idle");
	_bulletTexture.Load();
}

void Enemy::Update(float delta, VEC2 playerPos)
{
	if (!IsAlive()) return;
	GameObject::Update(delta);

	if (_pos.y < _config.stopY)
		_pos.y += _config.moveSpeed * delta;

	if (_config.fireInterval > 0.0f)
	{
		_fireCooldown -= delta;
		if (_fireCooldown <= 0.0f)
		{
			FirePattern(playerPos);
			_fireCooldown = _config.fireInterval;
		}
	}

	_bullets.Update(delta);
}

void Enemy::FirePattern(VEC2 playerPos)
{
	const float aimAngle = atan2f(playerPos.y - (_pos.y + 24.0f), playerPos.x - (_pos.x + 24.0f));
	const float straightAngle = PI * 0.5f;

	switch (_config.shotPattern)
	{
	case E_EnemyShotPattern::AIMED_SINGLE:
		AddBullet(aimAngle);
		break;
	case E_EnemyShotPattern::AIMED_SPREAD:
	case E_EnemyShotPattern::STRAIGHT_SPREAD:
	{
		const float center = _config.shotPattern == E_EnemyShotPattern::AIMED_SPREAD ? aimAngle : straightAngle;
		const int count = (_config.bulletCount > 0) ? _config.bulletCount : 1;
		for (int i = 0; i < count; ++i)
		{
			const float offset = count == 1 ? 0.0f
				: -_config.spreadAngle * 0.5f + _config.spreadAngle * i / (count - 1);
			AddBullet(center + offset);
		}
		break;
	}
	case E_EnemyShotPattern::STRAIGHT_RAPID:
		AddBullet(straightAngle);
		break;
	case E_EnemyShotPattern::RADIAL_BURST:
	{
		const int count = (_config.bulletCount > 0) ? _config.bulletCount : 1;
		for (int i = 0; i < count; ++i)
			AddBullet(2.0f * PI * i / count);
		break;
	}
	}
}

void Enemy::AddBullet(float angle)
{
	_bullets.Spawn(
		VEC2(_pos.x + 24.0f, _pos.y + 24.0f),
		VEC2(cosf(angle) * _config.bulletSpeed, sinf(angle) * _config.bulletSpeed),
		_config.bulletRadius);
}

void Enemy::Render()
{
	if (!IsAlive()) return;
	GameObject::Render();
	_bullets.Render(_bulletTexture);
}

void Enemy::TakeDamage(int damage)
{
	if (damage > 0 && IsAlive())
	{
		_hp -= damage;
		if (!IsAlive())
			_bullets.Clear();
	}
}

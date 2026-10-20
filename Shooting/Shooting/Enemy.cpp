#include "Enemy.h"
#include "EnemyData.h"
#include <cmath>

namespace
{
	const float PI = 3.14159265358979323846f;
}

Enemy::Enemy(VEC2 pos, const EnemyConfig& config)
	: GameObject(pos, config.radius), _config(config), _hp(config.hp), _maxHp(config.hp),
	_fireCooldown(config.initialFireDelay),
	_bulletTexture(config.bulletTexturePath, VEC2(0.0f, 0.0f))
{
	auto clip = std::make_unique<AnimationClip>("Idle", _config.animationFrameDuration, true);
	if (_config.animationPaths.empty())
	{
		auto texture = std::make_unique<Texture>(_config.spritePath, pos);
		texture->Load();
		clip->AddTexture(std::move(texture));
	}
	else
	{
		for (const std::string& path : _config.animationPaths)
		{
			auto texture = std::make_unique<Texture>(path, pos);
			texture->Load();
			clip->AddTexture(std::move(texture));
		}
	}
	_anim->AddClip("Idle", std::move(clip));
	_anim->ChangeClip("Idle");
	_bulletTexture.Load();
	if (_config.cycleBossPatterns) SetBossPattern();
}

void Enemy::Update(float delta, VEC2 playerPos)
{
	if (!IsAlive()) return;
	GameObject::Update(delta);

	if (_pos.y < _config.stopY)
		_pos.y += _config.moveSpeed * delta;

	if (_config.cycleBossPatterns)
	{
		_bossPhaseTimer += delta;
		const float phaseDuration = GetStageBossPatternData(_config.bossType, _bossPhase).duration;
		if (_bossPhaseTimer >= phaseDuration)
		{
			_bossPhaseTimer -= phaseDuration;
			_bossPhase = (_bossPhase + 1) % 3;
			SetBossPattern();
		}
	}

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

void Enemy::SetBossPattern()
{
	const BossPatternData pattern = GetStageBossPatternData(_config.bossType, _bossPhase);
	_config.shotPattern = pattern.shotPattern;
	_config.bulletCount = pattern.bulletCount;
	_config.spreadAngle = pattern.spreadAngle;
	_config.fireInterval = pattern.fireInterval * _config.difficultyFireIntervalScale;
	_config.bulletSpeed = pattern.bulletSpeed * _config.difficultyBulletSpeedScale;
	_fireCooldown = pattern.startDelay * _config.difficultyFireIntervalScale;

	_bossPatternAngle = {};
}

const char* Enemy::GetBossPatternName() const
{
	if (!_config.cycleBossPatterns) return "";
	return GetStageBossPatternData(_config.bossType, _bossPhase).name;
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
		
	}break;
	case E_EnemyShotPattern::STRAIGHT_RAPID:
		AddBullet(straightAngle);
		break;
	case E_EnemyShotPattern::RADIAL_BURST:
	{
		const int count = (_config.bulletCount > 0) ? _config.bulletCount : 1;
		for (int i = 0; i < count; ++i)
			AddBullet(2.0f * PI * i / count);
	}break;
	case E_EnemyShotPattern::ROTATING_BURST:
	{

		const int count = (_config.bulletCount > 0)
			? _config.bulletCount : 1;

		for (int i = 0; i < count; ++i)
		{
			const float angle =
				2.0f * PI * i / count + _bossPatternAngle;

			AddBullet(angle);
		}

		_bossPatternAngle += 0.15f;

		if (_bossPatternAngle >= 2.0f * PI)
			_bossPatternAngle -= 2.0f * PI;


	}break;
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

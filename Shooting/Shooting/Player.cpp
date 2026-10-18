#include "Player.h"
#include <cmath>

Player::Player()
	:
	DEF_SPEED(400.0f),
	LOW_SPEED(200.0f),
	_speedCur(400.0f),
	_isFocused(false),
	_hurtBoxRadius(2.0f),
	_grazeRadius(25.0f),
	_isInvincible(false),
	_invincibleTimer(0.0f)

{
	_pos = VEC2(400.0f, 600.0f);
	_moveDirection = VEC2(0.0f, 0.0f);

	float halfSprite = 32.0f;

	_areaMin = VEC2(320.0f + halfSprite,.0f + halfSprite);
	_areaMax = VEC2( 960.0f - halfSprite, 720.0f - halfSprite * 2);

	string prefix = "rsc/imgs/Player/cirno/";

	//auto idleTex = std::make_unique<Texture>(prefix +)
	auto idleClip = std::make_unique<AnimationClip>("Idle", .05f, true);
	for (int i = 0; i < 11; i++)
	{
		auto idleTex = std::make_unique<Texture>(prefix + std::to_string(i) + ".png", _pos);
		idleTex->Load();
		idleClip->AddTexture(std::move(idleTex));
	}

	_anim->AddClip("Idle", std::move(idleClip));
	_anim->ChangeClip("Idle");

	_hurtBoxTex.Load();
	_shotTex.Load();

}

Player::~Player() = default;

void Player::Update(float delta)
{
	GameObject::Update(delta);
	if (_shotCooldown > 0.0f)
		_shotCooldown -= delta;
	if (_rapidFireTimer > 0.0f) _rapidFireTimer -= delta;
	if (_shotgunTimer > 0.0f) _shotgunTimer -= delta;
	if (_powerLevel > 0)
	{
		_powerDecayTimer -= delta;
		while (_powerDecayTimer <= 0.0f && _powerLevel > 0)
		{
			--_powerLevel;
			_powerDecayTimer += POWER_DECAY_INTERVAL;
		}
		if (_powerLevel == 0) _powerDecayTimer = 0.0f;
	}
	_shots.Update(delta);

	float length = sqrtf(_moveDirection.x * _moveDirection.x + _moveDirection.y * _moveDirection.y);
	if (length > 0.0f)
	{
		float scale = _speedCur * delta / (length > 1.0f ? length : 1.0f);
		_pos.x += _moveDirection.x * scale;
		_pos.y += _moveDirection.y * scale;
		if (_pos.x < _areaMin.x) _pos.x = _areaMin.x;
		if (_pos.x > _areaMax.x) _pos.x = _areaMax.x;
		if (_pos.y < _areaMin.y) _pos.y = _areaMin.y;
		if (_pos.y > _areaMax.y) _pos.y = _areaMax.y;
	}
	_moveDirection = VEC2(0.0f, 0.0f);
	if (_isInvincible && (_invincibleTimer -= delta) <= 0.0f)
		_isInvincible = false;
}

void Player::Render()
{
	// Blink while invincible.
	if (!_isInvincible || ((int)(_invincibleTimer * 12.0f) % 2 == 0))
	{
		GameObject::Render();
		_hurtBoxTex.SetPos(VEC2(_pos.x + 24 ,_pos.y + 24));
		_hurtBoxTex.Print();
	}
	_shots.Render(_shotTex);
}


void Player::Move(VEC2 dir, float)
{
	_moveDirection.x += dir.x;
	_moveDirection.y += dir.y;
}
void Player::SetFocus(bool t)
{
	_isFocused = t;
	_speedCur = (_isFocused) ? LOW_SPEED : DEF_SPEED;
}

bool Player::OnHit()
{
	if (_isInvincible) return false;
	_isInvincible = true;
	_invincibleTimer = 2.0f;
	return true;
}

void Player::OnGraze()
{
	if (_powerLevel >= MAX_POWER) return;
	_grazePower += 0.05f;
	if (_grazePower >= 1.0f)
	{
		_grazePower -= 1.0f;
		AddPower();
	}
}

void Player::Shoot()
{
	if (_shotCooldown > 0.0f) return;
	const VEC2 origin(_pos.x + 26.0f, _pos.y - 12.0f);
	if (_shotgunTimer > 0.0f)
	{
		constexpr float spread[] = { -0.32f, -0.16f, 0.0f, 0.16f, 0.32f };
		for (float angle : spread)
			_shots.Spawn(origin, VEC2(sinf(angle) * 720.0f, -cosf(angle) * 720.0f));
	}
	else
		_shots.Spawn(origin, VEC2(0.0f, -720.0f));
	_shotCooldown = _rapidFireTimer > 0.0f ? 0.08f : 0.16f;
}

bool Player::UseBomb()
{
	if (_bombs <= 0) return false;
	--_bombs;
	_isInvincible = true;
	_invincibleTimer = 3.5f;
	return true;
}

void Player::AddPower()
{
	if (_powerLevel < MAX_POWER) ++_powerLevel;
	_powerDecayTimer = POWER_DECAY_INTERVAL;
}

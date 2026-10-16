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
	for (auto it = _shots.begin(); it != _shots.end();)
	{
		if (!it->Update(delta))
			it = _shots.erase(it);
		else
			++it;
	}

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
	for (const Bullet& shot : _shots)
		shot.Render(_shotTex);
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

void Player::OnHit()
{
	if (_isInvincible) return;
	_isInvincible = true;
	_invincibleTimer = 2.0f;
}

void Player::OnGraze()
{
	// Score and graze counters belong to the game-state system.
}

void Player::Shoot()
{
	if (_shotCooldown > 0.0f) return;
	_shots.emplace_back(VEC2(_pos.x + 26.0f, _pos.y - 12.0f), VEC2(0.0f, -720.0f));
	_shotCooldown = 0.16f;
}

void Player::UseBomb()
{
	// Connect to the bomb and enemy-bullet systems when they are added.
}

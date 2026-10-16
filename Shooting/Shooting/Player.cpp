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
	_invincibleTimer(0.0f),
	_texPy("rsc/imgs/Cats/Player/Idle.png", VEC2(0.0f, 0.0f))
{
	_pos = VEC2(300.0f, 600.0f);
	_moveDirection = VEC2(0.0f, 0.0f);
	_texPy.Load();
	_areaMin = VEC2(16.0f, 16.0f);
	_areaMax = VEC2((float)g2_GetScnW() - 16.0f, (float)g2_GetScnH() - 16.0f);
}

Player::~Player() = default;

void Player::Update(float delta)
{
	GameObject::Update(delta);
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
		_texPy.SetPos(_pos);
		_texPy.Print();
	}
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
	// Connect to the player-shot system when it is added to the game.
}

void Player::UseBomb()
{
	// Connect to the bomb and enemy-bullet systems when they are added.
}

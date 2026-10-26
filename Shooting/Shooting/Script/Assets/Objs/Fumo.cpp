#include "Fumo.h"
#include "AnimationClip.h"
#include <algorithm>
#include <string>

Fumo::Fumo(float frameDuration)
	: GameObject(VEC2(0.0f, 720.0f))
{
	auto clip = std::make_unique<AnimationClip>("Idle", frameDuration, true);
	for (int frame = 0; frame < 11; ++frame)
	{
		const std::string path = "rsc/imgs/Player/cirno/" + std::to_string(frame) + ".png";
		auto texture = std::make_unique<Texture>(path, _pos);
		texture->SetScale(VEC2(2.0f,2.0f));
		clip->AddTexture(std::move(texture));
	}
	_anim->AddClip("Idle", std::move(clip));
	_anim->ChangeClip("Idle");
}

void Fumo::Reset(float x, float stopY, float speed)
{
	_pos = VEC2(x, 720.0f);
	_stopY = stopY;
	_speed = speed;
	_state = State::RISING;
	_anim->Restart();
}

void Fumo::Update(float delta)
{
	if (_state == State::INACTIVE) return;

	GameObject::Update(delta);
	if (_state == State::RISING)
	{
		_pos.y = (std::max)(_stopY, _pos.y - _speed * delta);
		if (_pos.y <= _stopY) _state = State::SINKING;
	}
	else
	{
		_pos.y += _speed * delta;
		if (_pos.y >= 720.0f)
		{
			_state = State::INACTIVE;
			return;
		}
	}
	return;
}

void Fumo::Render()
{
	if (IsActive()) GameObject::Render();
}

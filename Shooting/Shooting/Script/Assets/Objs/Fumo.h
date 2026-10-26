#pragma once

#include "GameObject.h"
class Fumo : public GameObject
{
private:
	enum class State { INACTIVE, RISING, SINKING };

	State _state = State::INACTIVE;
	float _stopY = 0.0f;
	float _speed = 500.0f;

public:
	explicit Fumo(float frameDuration = 0.1f);

	void Reset(float x, float stopY);
	void Update(float delta) override;
	void Render() override;
	bool IsActive() const { return _state != State::INACTIVE; }
};

#pragma once
#include <glc2d.h>
#include "Collider.h"

#include "Animator.h"

class GameObject
{
protected:
	VEC2 _pos;
	Collider _collider;
	Animator* _anim;
public:
	GameObject(VEC2 pos = { 0,0 }, float colliderRadius = 0.0f)
		: _pos(pos), _collider(colliderRadius)
	{
		_anim = new Animator;
	}

	virtual ~GameObject()
	{
		printf(
			"GameObject Delete : %p / Animator = %p\n",
			this,
			_anim
		);
		if (_anim)
		{
			delete _anim;
			_anim = nullptr;
		}
	}

	virtual void Update(float delta);
	virtual void Render();
	
	void SetPos(VEC2 pos);
	VEC2 GetPos() const;
	const Collider& GetCollider() const { return _collider; }
	Animator* GetAnimator() const;
};


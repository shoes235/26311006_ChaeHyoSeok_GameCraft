#pragma once

#include <string>
using std::string;

#include <glc2d.h>
#include "Collider.h"


enum class E_ItemType
{
	LIFE,
	BOMB,
	RAPID_FIRE,
	POWER,
	SHOTGUN,
	STAR
};

class Item
{
private:
	VEC2 _pos;
	Collider _collider{14.0f};
	E_ItemType _type;
	float _speed = 120.0f;
	bool _active = true;

public:
	Item(VEC2 pos, E_ItemType type) : _pos(pos), _type(type) {}
	void Update(float delta);
	void Render(int font) const;
	VEC2 GetPos() const { return _pos; }
	E_ItemType GetType() const { return _type; }
	const Collider& GetCollider() const { return _collider; }
	bool IsActive() const { return _active; }
	void Collect() { _active = false; }
};

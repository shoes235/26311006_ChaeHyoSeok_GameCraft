#pragma once
#include <glc2d.h>

enum class ItemType
{
	LIFE,
	BOMB,
	RAPID_FIRE,
	POWER
};

class Item
{
private:
	VEC2 _pos;
	ItemType _type;
	float _speed = 120.0f;
	bool _active = true;

public:
	Item(VEC2 pos, ItemType type) : _pos(pos), _type(type) {}
	void Update(float delta);
	void Render(int font) const;
	VEC2 GetPos() const { return _pos; }
	ItemType GetType() const { return _type; }
	float GetRadius() const { return 14.0f; }
	bool IsActive() const { return _active; }
	void Collect() { _active = false; }
};

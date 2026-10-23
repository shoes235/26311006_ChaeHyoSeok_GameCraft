#include "Item.h"

void Item::Update(float delta)
{
	_pos.y += _speed * delta;
	if (_pos.y > g2_GetScnH() + 30.0f) _active = false;
}

void Item::Render(int font) const
{
	string label = "";
	unsigned int color = 0xFFFFFFFF;
	switch (_type)
	{
		case E_ItemType::LIFE:
		{
			label = "1UP";
			color = 0xFFFF6060;
		}break;

		case E_ItemType::BOMB:
		{
			label = "B";
			color = 0xFFFFAA40;
		}break;

		case E_ItemType::RAPID_FIRE:
		{
			label = "Rp";
			color = 0xFF60FFFF;
		}break;

		case E_ItemType::POWER:
		{
			label = "P";
			color = 0xFFFF60FF;
		}break;

		case E_ItemType::SHOTGUN: {
			label = "SG";
			color = 0xFFFFE060;

		}break;

		case E_ItemType::STAR: {
			label = "º°";
			color = 0xFF0000FF;
		}break;


	}
	const int x = static_cast<int>(_pos.x);
	const int y = static_cast<int>(_pos.y);
	g2_FontDrawText(font, { x - 28, y - 16, x + 28, y + 16 }, color, "%s", label.c_str());
}

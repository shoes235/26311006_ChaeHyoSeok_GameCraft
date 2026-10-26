#include "InputManager.h"
#include <utility>

InputManager::InputManager() : _holdBind{}, _downBind{}, _preKeyUp{} {}
InputManager::~InputManager()
{
	_holdBind.clear();
	_downBind.clear();
}

void InputManager::RegisterAction(const vector<unsigned char>& keys,E_KeyType type, function<void()> callback)
{
	switch (type)
	{
	case E_KeyType::HOLD:
		_holdBind.emplace_back(keys, std::move(callback));
		break;
	case E_KeyType::DOWN:
		_downBind.emplace_back(keys, std::move(callback));
		break;
	}


	return;
}

void InputManager::UpdateInput()
{
	GetKey();
	GetKeyDown();
}


void InputManager::GetKey()
{
	const KEYCODE* pkey = g2_GetKeyboard();
	if (!pkey) return;

	for (const auto& bind : _holdBind)
	{
		bool isTriggered = false;

		for (auto key : bind.keys)
		{
			if (pkey[key])
			{
				isTriggered = true;
				break;
			}


		}
		if (isTriggered) bind.callback();
	}

}

void InputManager::GetKeyDown()
{
	const KEYCODE* pkey = g2_GetKeyboard();
	if (!pkey) return;
	
	for (const auto& bind : _downBind)
	{
		bool isTriggered = false;

		for (auto key : bind.keys)
		{
			if (pkey[key] && !_preKeyUp[key])
			{
				isTriggered = true;
				break;
			}
		}

		if (isTriggered)
			bind.callback();
	}

	for (int i = 0; i < 256; ++i)
		_preKeyUp[i] = pkey[i];

}
#pragma once
#include <glc2d.h>
#include <functional>
#include <vector>

using std::vector;
using std::function;


enum class E_KeyType
{
	HOLD,
	DOWN
};

class InputManager
{
public :
	struct ActionBinding
	{
		vector<unsigned char> keys;
		function<void()> callback;

		ActionBinding(const vector<unsigned char>& k,function<void()> cb)
			: keys(k),callback(cb) { }
	};

	InputManager();
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager&&) = delete;
	~InputManager();

	void RegisterAction(const vector<unsigned char>& keys, E_KeyType type, function<void()> callback);
	void UpdateInput();

private:
	vector<ActionBinding> _holdBind;
	vector<ActionBinding> _downBind;

	KEYCODE _preKeyUp[256]{};

	void GetKey();
	void GetKeyDown();
};


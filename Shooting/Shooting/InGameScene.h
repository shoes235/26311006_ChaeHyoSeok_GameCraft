#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

// === SingleTone ===
#include "IScene.h"
#include "InputManager.h"
#include "DeltaTimer.h"

// === Datas ===
#include "Randomizer.h"
#include "GameData.h"

#include "GameObject.h"
// === GameObjs ===
#include "Player.h"

// === Scene ===
#include "JumpScareScene.h"

class InGameScene : public IScene
{
private:
	vector<GameObject> objs;
	Player _player;

	//UI
	Texture _layout{ "rsc/imgs/Layout.png", VEC2(0,0)};

	InputManager _inputMgr;
private:

public:
	InGameScene() : IScene(E_SceneType::IN_GAME)
	{
		_winName = "InGame";
		_winColor = 0xE1E1E1;
	}
public:
	void Init() override;
	void Render() override;
	void Update() override;
	void Destroy() override;
};


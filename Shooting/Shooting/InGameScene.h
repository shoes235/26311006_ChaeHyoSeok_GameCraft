#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;
#include <memory>
#include <algorithm>

// === SingleTone ===
#include "IScene.h"
#include "InputManager.h"
#include "DeltaTimer.h"

// === Datas ===
#include "Randomizer.h"
#include "GameData.h"
#include "EnemyData.h"

#include "GameObject.h"
// === GameObjs ===
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "ProgressBar.h"

// === Scene ===
#include "JumpScareScene.h"

class InGameScene : public IScene
{
private:
	vector<GameObject> objs;
	Player _player;
	std::vector<std::unique_ptr<Enemy>> _enemies;
	std::vector<Item> _items;
	std::unique_ptr<ProgressBar> _bossHealthBar;
	Randomizer _randomizer;
	enum class StagePhase { WAVE, BOSS, CLEAR };
	StagePhase _phase = StagePhase::WAVE;

	const float STAGE_TIME{60.0f};
	float _stageTimer = STAGE_TIME;

	float _spawnTimer = 1.0f;
	float _clearTimer = 0.0f;

	const int MAX_LIVE{ 10 };
	int _lives = MAX_LIVE;


	int _uiFont = -1;
	void SpawnRandomEnemy();
	void SpawnBoss();
	void TryDropItem(VEC2 pos);

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


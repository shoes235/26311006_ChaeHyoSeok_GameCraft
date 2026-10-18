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
#include "Particle.h"
#include "BomParticle.h"

// === Scene ===
#include "JumpScareScene.h"

class InGameScene : public IScene
{
private:
	//objs
	vector<GameObject> objs;
	Player _player;
	std::vector<std::unique_ptr<Enemy>> _enemies;
	std::vector<Item> _items;
	std::vector<std::unique_ptr<Particle>> _particles;
	std::unique_ptr<ProgressBar> _bossHealthBar;
	Randomizer _randomizer;
	enum class StagePhase { WAVE, BOSS, CLEAR };
	StagePhase _phase = StagePhase::WAVE;

	//time
	const float STAGE_TIME{30.0f};
	float _stageTimer = STAGE_TIME;

	float _spawnTimer = 1.0f;
	float _clearTimer{};

	bool _T_gameOver{};
	const float SCENE_CHANGE_TIME {1.0f};
	float _changeTime{};

	//liv
	const int MAX_LIVE{ 8 };
	int _lives = MAX_LIVE;
	int _grazeCount = 0;



	void SpawnRandomEnemy();
	void SpawnBoss();
	void TryDropItem(VEC2 pos);
	void SpawnDeathParticle(VEC2 pos);
	float GetRandomSpawnDelay();

	//UI
	int _uiFont = -1;
	Texture _layout{ "rsc/imgs/Layout.png", VEC2(0,0)};
	Texture _cloud{ "rsc/imgs/cloud.jpg",VEC2(120,0) };
	Texture _cloud2{ "rsc/imgs/cloud.jpg",VEC2(120,-720) };

	//input
	InputManager _inputMgr;


	//Sounds
	//bgm
	vector<int> _sounds;

	vector<int> _bgms{};

	int _curBgm{};

	//SFX
	

	//py
	int _shotSFX{};
	int _bomSFX{};

	int _deathSFX{};
	int _grazeSFX{};
	int _itemGetSFX{};

	//E
	int _EDeathSFX{};



private:

public:
	InGameScene() : IScene(E_SceneType::IN_GAME)
	{
		_winName = "InGame";
		_winColor = 0x1A2835;
	}
public:
	void Init() override;
	void Render() override;
	void Update() override;
	void Destroy() override;
};


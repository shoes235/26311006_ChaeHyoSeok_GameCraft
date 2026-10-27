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
#include "Collider.h"
#include "MonitorScreen.h"
#include "ObjectPool.h"

// === Scene ===
class JumpScareScene;


enum class E_StageType{ START = 1,MIDDLE = 6,LAST = 9};

class InGameScene : public IScene
{
private:
	Randomizer _randomizer;

	//objs
	vector<GameObject> objs;

	Player _player;

	vector<Enemy*> _enemies;

	struct EnemyPool
	{
		EnemyConfig assetConfig;
		ObjectPool<Enemy> objects;

		EnemyPool(const EnemyConfig& config, VEC2 pos)
			: assetConfig(config), objects([config, pos] { return std::make_unique<Enemy>(pos, config); }, 0, 1) {}
		EnemyPool(const EnemyPool&) = delete;
		EnemyPool& operator=(const EnemyPool&) = delete;
		EnemyPool(EnemyPool&&) noexcept = default;
		EnemyPool& operator=(EnemyPool&&) noexcept = default;
	};

	vector<EnemyPool> _enemyPools;

	vector<Item> _items;

	vector<BomParticle*> _deathParticles;
	vector<BomParticle*> _hitParticles;
	ObjectPool<BomParticle> _deathParticlePool{
		[] { return std::make_unique<BomParticle>(VEC2(0, 0), 2.0f); }, 0, 1 };
	ObjectPool<BomParticle> _hitParticlePool{
		[] { return std::make_unique<BomParticle>(VEC2(0, 0), 0.35f); }, 0, 1 };

	std::unique_ptr<ProgressBar> _bossHealthBar;

	//Spooky Scary
	vector<Texture*> _horrorTextures;
	static vector<std::unique_ptr<Texture>>& GetHorrorTextureCache();
	vector<string> _horrorSoundKeys;
	Texture* _activeHorrorTexture = nullptr;

	//GAME

	//Pause
	bool _isPaused{};
	int _pauseUIFont{};

	//Horror
	const float MAX_HORROR_EFFECT_TIME{ 1.0f };
	float _horrorEffectTimer{};

	const float MAX_HORROR_SOUND_TIME{ 3.0f };
	float _horrorSoundTimer{};
	string _activeHorrorSound;

	//Stage
	enum class E_StagePhase { WAVE, BOSS, CLEAR };
	E_StagePhase _phase = E_StagePhase::WAVE;


	//time
	const float STAGE_TIME{30.0f};
	float _stageTimer = STAGE_TIME;

	float _spawnTimer{1.0f};
	float _clearTimer{};

	//gameOver
	bool _isGameOvered{};
	const float SCENE_CHANGE_TIME {1.0f};
	float _changeTime{};

	//liv
	const int MAX_LIVE{ 8 };
	int _lives = MAX_LIVE;
	int _grazeCount{};


	//UI
	int _uiFont = -1;
	Texture _layout{ "rsc/imgs/UI/Layout.png", VEC2(0,0)};
	Texture _pauseUI{ "rsc/imgs/UI/Black2.png",VEC2(0,0) };

	//Cloud
	Texture _cloud{ "rsc/imgs/cloud.jpg",VEC2(120,0) };
	Texture _cloud2{ "rsc/imgs/cloud.jpg",VEC2(120,-720) };
	Texture _secondCloud{ "rsc/imgs/cloud2.jpg", VEC2(120, 0) };
	Texture _secondCloud2{ "rsc/imgs/cloud2.jpg", VEC2(120, -720) };
	Texture _thirdCloud{ "rsc/imgs/cloud3.jpg", VEC2(120, 0) };
	Texture _thirdCloud2{ "rsc/imgs/cloud3.jpg", VEC2(120, -720) };

	Texture _monitor{ "rsc/imgs/UI/monitor3.png",VEC2(25,400) };
	MonitorScreen _monitorScreen{ VEC2(109,435) };

	//input
	InputManager _inputMgr;
private:
	void UpdateGame();

	E_StageType GetCurrentStageType() const;

	void SpawnRandomEnemy();
	void SpawnBoss();
	void TryDropItem(VEC2 pos);
	void SpawnDeathParticle(VEC2 pos);
	void SpawnHitParticle(VEC2 pos);
	Enemy* AcquireEnemy(VEC2 pos, const EnemyConfig& config);
	BomParticle* AcquireParticle(VEC2 pos, bool deathEffect);
	void ReturnEnemy(Enemy* enemy);
	float GetRandomSpawnDelay();
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


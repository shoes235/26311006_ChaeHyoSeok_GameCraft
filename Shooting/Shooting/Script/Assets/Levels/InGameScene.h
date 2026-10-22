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
#include "JumpScareScene.h"

class InGameScene : public IScene
{
private:
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

	//objs
	vector<GameObject> objs;
	Player _player;
	std::vector<Enemy*> _enemies;
	std::vector<EnemyPool> _enemyPools;
	std::vector<Item> _items;
	std::vector<BomParticle*> _deathParticles;
	std::vector<BomParticle*> _hitParticles;
	ObjectPool<BomParticle> _deathParticlePool{
		[] { return std::make_unique<BomParticle>(VEC2(0, 0), 2.0f); }, 0, 1 };
	ObjectPool<BomParticle> _hitParticlePool{
		[] { return std::make_unique<BomParticle>(VEC2(0, 0), 0.35f); }, 0, 1 };
	std::unique_ptr<ProgressBar> _bossHealthBar;
	Randomizer _randomizer;

	//Spooky Scary
	std::vector<Texture*> _horrorTextures;
	static std::vector<std::unique_ptr<Texture>>& GetHorrorTextureCache();
	std::vector<string> _horrorSoundKeys;
	Texture* _activeHorrorTexture = nullptr;

	const float MAX_HORROR_EFFECT_TIME{ 1.0f };
	float _horrorEffectTimer = 0.0f;

	const float MAX_HORROR_SOUND_TIME{ 3.0f };
	float _horrorSoundTimer = 0.0f;
	string _activeHorrorSound;

	enum class E_StagePhase { WAVE, BOSS, CLEAR };
	E_StagePhase _phase = E_StagePhase::WAVE;

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
	void SpawnHitParticle(VEC2 pos);
	Enemy* AcquireEnemy(VEC2 pos, const EnemyConfig& config);
	BomParticle* AcquireParticle(VEC2 pos, bool deathEffect);
	void ReturnEnemy(Enemy* enemy);
	float GetRandomSpawnDelay();

	//UI
	int _uiFont = -1;
	Texture _layout{ "rsc/imgs/UI/Layout.png", VEC2(0,0)};
	Texture _cloud{ "rsc/imgs/cloud.jpg",VEC2(120,0) };
	Texture _cloud2{ "rsc/imgs/cloud.jpg",VEC2(120,-720) };
	Texture _monitor{ "rsc/imgs/UI/monitor3.png",VEC2(25,400) };
	MonitorScreen _monitorScreen{ VEC2(109,435) };



	//input
	InputManager _inputMgr;






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


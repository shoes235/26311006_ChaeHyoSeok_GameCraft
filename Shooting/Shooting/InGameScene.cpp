#include "InGameScene.h"
#include "SoundData.h"

void InGameScene::Init()
{
	SetFullScreen();

	_enemies.clear();
	_items.clear();
	_particles.clear();
	_bossHealthBar.reset();
	_grazeCount = 0;
	_phase = E_StagePhase::WAVE;
	_stageTimer = STAGE_TIME;
	_clearTimer = 0.0f;
	if (GameData::g_stageLast < 1) GameData::g_stageLast = 1;
	_spawnTimer = GetRandomSpawnDelay();

	const string bgmKeys[] = { SoundData::BGM_Game, SoundData::BGM_GameAlt, SoundData::BGM_Title };
	g_SoundManager.PlayBGM(bgmKeys[_randomizer.Rand(0, 2)], false);

	//input
	_inputMgr.RegisterAction
	(
		{ VK_UP },
		E_KeyType::HOLD,
		[this]()
		{
			_player.Move(VEC2(.0f,-1.0f),
			g_deltaTimer.GetDeltaTime());
		}
	);

	_inputMgr.RegisterAction
	(
		{VK_DOWN},
		E_KeyType::HOLD,
		[this]()
		{
			_player.Move(VEC2(.0f, 1.0f),
				g_deltaTimer.GetDeltaTime());
		}
	);

	_inputMgr.RegisterAction
	(
		{VK_LEFT},
		E_KeyType::HOLD,
		[this]()
		{
			_player.Move(VEC2(-1.0f, .0f),
				g_deltaTimer.GetDeltaTime());
		}
	);


	_inputMgr.RegisterAction
	(
		{VK_RIGHT},
		E_KeyType::HOLD,
		[this]()
		{
			_player.Move(VEC2(1.0f, .0f),
				g_deltaTimer.GetDeltaTime());
		}
	);


	_inputMgr.RegisterAction
	(
		{VK_SHIFT},
		E_KeyType::HOLD,
		[this]()
		{
			_player.SetFocus(true);
		}
	);

	_inputMgr.RegisterAction
	(
		{ 'Z' },
		E_KeyType::HOLD,
		[this]()
		{
			_player.Shoot();

			g_SoundManager.PlaySFX(SoundData::SFX_Shot);
		}
	);

	_inputMgr.RegisterAction
	(
		{ 'X' },
		E_KeyType::DOWN,
		[this]()
		{
			if (!_player.UseBomb()) return;
			for (const auto& enemy : _enemies)
				for (Bullet& bullet : enemy->GetBullets()) bullet.Deactivate();

			g_SoundManager.PlaySFX(SoundData::SFX_Bomb);
		}
	);

	_inputMgr.RegisterAction
	(
		{ VK_ESCAPE },
		E_KeyType::DOWN,
		[this]()
		{
			JumpScareScene* nextScene = new JumpScareScene();
			g_SceneMgr.ChangeScene(nextScene, E_SceneType::JUMP_SCARE);

		}
	);



	_cloud.Load();
	_cloud2.Load();

	_layout.Load();
	_uiFont = g2_FontCreate("굴림", 20, 0);


	return;
}
void InGameScene::Render()
{
	IScene::Render();

	//============================
	//OBJS
	_cloud.Print();
	_cloud2.Print();

	_player.Render();
	for (const auto& enemy : _enemies) enemy->Render();
	for (const Item& item : _items) item.Render(_uiFont);
	for (const auto& particle : _particles) particle->Render();
	_layout.Print();
	if (_phase == E_StagePhase::BOSS && _bossHealthBar)
	{
		g2_FontDrawText(_uiFont, { 350, 0, 920, 30 }, 0xFFFFFFFF, "BOSS HP");
		_bossHealthBar->Render();
	}

	//============================
	//UI


	g2_FontDrawText
	(
		_uiFont,
		{ 1000, 25, 1250, 125 },
		0xFFFFFFFF,
		"SCORE \n %010d", GameData::g_score
	);


	string liv{};

	for (int i = 0; i < MAX_LIVE; ++i)
	{
		if (i < _lives)
			liv += "[*]";
		else
			liv += "[ ]";
	}

	g2_FontDrawText
	(
		_uiFont, 
		{ 1000, 100, 1250, 200 },
		0xFFFF0000,
		"LIVES\n%s",liv.c_str()
	);


	string bom{};

	for (int i = 0; i < _player.GetMaxBomb(); ++i)
	{
		if (i < _player.GetBombs())
			bom += "[*]";
		else
			bom += "[ ]";
	}


	g2_FontDrawText
	(
		_uiFont,
		{ 1000, 200, 1270, 300 },
		0xFFFFAA40,
		"BOMBS\n%s",bom.c_str()
	);


	string pow{};

	pow += "[";
	for (int i = 0; i < _player.GetMaxPower(); ++i)
	{
		if (i < _player.GetPowerLevel())
			pow += "#";
		else
			pow += " ";
	}
	pow += "]";


	//POWER
	g2_FontDrawText
	(
		_uiFont, 
		{ 1000, 330, 1270, 390 },
		0xFFFF60FF,
		"POWER\n%s", pow.c_str()
	);

	//아이템
	if (_player.GetRapidFireTime() > 0.0f)
		g2_FontDrawText(_uiFont, { 1000, 260, 1270, 290 }, 0xFF60FFFF, "RAPID: %.0fs", _player.GetRapidFireTime());
	if (_player.GetShotgunTime() > 0.0f)
		g2_FontDrawText(_uiFont, { 1000, 292, 1270, 322 }, 0xFFFFE060, "SHOTGUN: %.0fs", _player.GetShotgunTime());

	//Power-DECAY
	if (_player.GetPowerLevel() > 0)
		g2_FontDrawText(_uiFont, { 1000, 390, 1270, 430 }, 0xFFFF60FF,
			"DECAY: %.1fs", _player.GetPowerDecayTime());

	//GRAZE
	g2_FontDrawText
	(
		_uiFont, 
		{ 1000, 440, 1270, 480 },
		0xFF80FFFF,
		"GRAZE: %d", _grazeCount
	);

	g2_FontDrawText
	(
		_uiFont, 
		{ 1000, 480, 1270, 520 },
		0xFF80FFFF,
		"GRAZE POWER: %d%%", static_cast<int>(_player.GetGrazePower() * 100.0f)
	);



	g2_FontDrawText(_uiFont, { 0, 200, 1270, 235 }, 0xFFFFFFFF, "STAGE: %d", GameData::g_stageLast);
	if (_phase == E_StagePhase::WAVE)
		g2_FontDrawText(_uiFont, { 0, 230, 1270, 265 }, 0xFFFFFFFF, "BOSS IN: %.0f", _stageTimer);
	else if (_phase == E_StagePhase::BOSS)
		g2_FontDrawText(_uiFont, { 0, 230, 1270, 265 }, 0xFFFFFFFF, "BOSS - %s",
			_enemies.empty() ? "" : _enemies.front()->GetBossPatternName());
	else
		g2_FontDrawText(_uiFont, { 430, 300, 900, 360 }, 0xFFFFFFFF, "STAGE CLEAR!");


	//============================




	return;
}
void InGameScene::Update()
{
	IScene::Update();
	float dt = g_deltaTimer.GetDeltaTime();

	if (!g_SoundManager.IsBGMPlaying())
	{
		const string bgmKeys[] = { SoundData::BGM_Game, SoundData::BGM_GameAlt, SoundData::BGM_Title };
		g_SoundManager.PlayBGM(bgmKeys[_randomizer.Rand(0, 2)], false);
	}

	VEC2 posC = _cloud.GetPos();
	posC.y += dt * 1000;

	if (posC.y > 720.0f)
		posC.y = -720.0f;

	_cloud.SetPos(posC);


	VEC2 posC2 = _cloud2.GetPos();
	posC2.y += dt * 1000;

	if (posC2.y > 720.0f)
		posC2.y = -720.0f;

	_cloud2.SetPos(posC2);


	//py

	_player.SetFocus(false);

	_inputMgr.UpdateInput();

	_player.Update(dt);

	if (_phase == E_StagePhase::WAVE)
	{
		_stageTimer -= dt;
		_spawnTimer -= dt;
		if (_stageTimer <= 0.0f)
		{
			_enemies.clear();
			_phase = E_StagePhase::BOSS;
			SpawnBoss();
		}
		else if (_spawnTimer <= 0.0f && _enemies.size() < 6)
		{
			SpawnRandomEnemy();
			_spawnTimer = GetRandomSpawnDelay();
		}
	}
	else if (_phase == E_StagePhase::CLEAR)
	{
		_clearTimer -= dt;
		if (_clearTimer <= .0f)
		{
			++GameData::g_stageLast;
			_phase = E_StagePhase::WAVE;
			_stageTimer = STAGE_TIME;
			_spawnTimer = GetRandomSpawnDelay();
		}
	}

	for (const auto& enemy : _enemies) enemy->Update(dt, _player.GetCorePos());
	if (_phase == E_StagePhase::BOSS && _bossHealthBar && !_enemies.empty())
		_bossHealthBar->SetProgress(static_cast<float>(_enemies.front()->GetHP()) /
			_enemies.front()->GetMaxHP());

	for (Bullet& shot : _player.GetShots())
	{
		if (!shot.IsActive()) continue;
		for (const auto& enemy : _enemies)
		{
			if (!enemy->IsAlive()) continue;
			const VEC2 enemyPos(enemy->GetPos().x + 24.0f, enemy->GetPos().y + 24.0f);
			const VEC2 shotPos(shot.GetPos().x + 6.0f, shot.GetPos().y + 10.0f);
			if (!shot.GetCollider().IsEntered(shotPos, enemy->GetCollider(), enemyPos)) continue;

			enemy->TakeDamage(_player.GetShotDamage());
			if (!enemy->IsAlive())
			{
				GameData::g_score += _randomizer.Rand(GameData::g_stageLast * 100, GameData::g_stageLast * 200);
				SpawnDeathParticle(enemyPos);
				TryDropItem(enemyPos);
				g_SoundManager.PlaySFX(SoundData::SFX_EnemyDeath);
			}
			shot.Deactivate();
			break;
		}
	}
	const Collider itemPickupCollider(_player.GetCollider().Radius + 32.0f);
	for (Item& item : _items)
	{
		if (!item.IsActive()) continue;
		item.Update(dt);
		if (!item.IsActive()) continue;
		if (!item.GetCollider().IsEntered(item.GetPos(), itemPickupCollider, _player.GetCorePos())) continue;

		g_SoundManager.PlaySFX(SoundData::SFX_ItemGet);
		switch (item.GetType())
		{
		case ItemType::LIFE: ++_lives; break;
		case ItemType::BOMB: _player.AddBomb(); break;
		case ItemType::RAPID_FIRE: _player.AddRapidFire(); break;
		case ItemType::POWER: _player.AddPower(); break;
		case ItemType::SHOTGUN: _player.AddShotgun(); break;
		}
		item.Collect();
	}
	_items.erase(std::remove_if(_items.begin(), _items.end(),
		[](const Item& item) { return !item.IsActive(); }), _items.end());

	for (const auto& enemy : _enemies)
	{
		for (Bullet& bullet : enemy->GetBullets())
		{
			if (!bullet.IsActive()) continue;
			const VEC2 bulletPos(bullet.GetPos().x + 6.0f, bullet.GetPos().y + 10.0f);
			const VEC2 core = _player.GetCorePos();
			if (bullet.GetCollider().IsEntered(bulletPos, _player.GetCollider(), core))
			{
				if (_player.OnHit()) --_lives;
				bullet.Deactivate();
			}
			else if (!bullet.HasGrazed() && bullet.GetCollider().IsEntered(
				bulletPos, _player.GetGrazeCollider(), core))
			{
				bullet.MarkGrazed();
				_player.OnGraze();
				++_grazeCount;
				g_SoundManager.PlaySFX(SoundData::SFX_Graze);
				GameData::g_score += 100;
			}
		}
	}
	for (auto particle = _particles.begin(); particle != _particles.end();)
	{
		(*particle)->Update(dt);
		if (!(*particle)->IsAlive())
			particle = _particles.erase(particle);
		else
			++particle;
	}
	if (_lives <= 0)
	{
		if(!_T_gameOver)
			g_SoundManager.PlaySFX(SoundData::SFX_PlayerDeath);

		_T_gameOver = true;

		_changeTime += dt;

		if(_changeTime > SCENE_CHANGE_TIME)
			g_SceneMgr.ChangeScene(new JumpScareScene(), E_SceneType::JUMP_SCARE);
		return;
	}
	_enemies.erase(std::remove_if(_enemies.begin(), _enemies.end(),
		[](const std::unique_ptr<Enemy>& enemy) { return !enemy->IsAlive(); }), _enemies.end());
	if (_phase == E_StagePhase::BOSS && _enemies.empty())
	{
		_phase = E_StagePhase::CLEAR;
		GameData::g_score += 1000;
		_clearTimer = 3.0f;
		_bossHealthBar.reset();
	}




	return;
}

void InGameScene::SpawnRandomEnemy()
{
	static EnemyConfig(*factories[])() = {
		MakeShotGunEnemy, MakeAimedFiveShotEnemy, MakeRapidStraightEnemy,
		MakeStraightShotgunEnemy, MakeRadialShotgunEnemy
	};
	const int x = _randomizer.Rand(350, 900);
	const int y = _randomizer.Rand(-80, -24);
	const int type = _randomizer.Rand(0, 4);
	EnemyConfig config = ScaleEnemyForStage(factories[type](), GameData::g_stageLast);
	_enemies.emplace_back(new Enemy(VEC2(static_cast<float>(x), static_cast<float>(y)), config));
}

void InGameScene::SpawnBoss()
{
	const int bossType = _randomizer.Rand(0, 2);
	EnemyConfig config;
	switch (bossType)
	{
	case 1: config = MakeStageBoss2(); break;
	case 2: config = MakeStageBoss3(); break;
	default: config = MakeStageBoss(); break;
	}
	config = ScaleEnemyForStage(config, GameData::g_stageLast);
	_enemies.emplace_back(new Enemy(VEC2(610.0f, -60.0f), config));
	_bossHealthBar.reset(new ProgressBar(VEC2(400.0f, 30.0f)));
	_bossHealthBar->SetProgress(1.0f);
}

void InGameScene::TryDropItem(VEC2 pos)
{
	if (_randomizer.Rand(0, 99) >= 35) return;
	_items.emplace_back(pos, static_cast<ItemType>(_randomizer.Rand(0, 4)));
}

void InGameScene::SpawnDeathParticle(VEC2 pos)
{
	_particles.emplace_back(new BomParticle(pos));
}

float InGameScene::GetRandomSpawnDelay()
{
	const int stage = GameData::g_stageLast > 0 ? GameData::g_stageLast : 1;
	const int minTenths = (std::max)(3, 10 - (stage - 1));
	const int maxTenths = (std::max)(minTenths, 40 - 3 * (stage - 1));
	return _randomizer.Rand(minTenths, maxTenths) / 10.0f;
}

void InGameScene::Destroy()
{
	g_SoundManager.StopBGM();
}

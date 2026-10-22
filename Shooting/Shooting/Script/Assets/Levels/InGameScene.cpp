#include "InGameScene.h"
#include "SoundData.h"

std::vector<std::unique_ptr<Texture>>& InGameScene::GetHorrorTextureCache()
{
	static std::vector<std::unique_ptr<Texture>> textures;
	if (textures.empty())
	{
		for (int i = 0; i <= 9; ++i)
			textures.emplace_back(new Texture("rsc/imgs/Cats/" + std::to_string(i) + ".png", VEC2(0, 0)));
		textures.emplace_back(new Texture("rsc/imgs/Cats/Uncanny.png", VEC2(0, 0)));
	}
	return textures;
}

void InGameScene::Init()
{
	SetFullScreen();

	for (Enemy* enemy : _enemies) ReturnEnemy(enemy);
	_enemies.clear();
	_items.clear();
	for (BomParticle* particle : _deathParticles) _deathParticlePool.Return(particle);
	for (BomParticle* particle : _hitParticles) _hitParticlePool.Return(particle);
	_deathParticles.clear();
	_hitParticles.clear();
	_bossHealthBar.reset();
	_grazeCount = 0;
	_phase = E_StagePhase::WAVE;
	_stageTimer = STAGE_TIME;
	_clearTimer = 0.0f;
	if (GameData::g_stageLast < 1) GameData::g_stageLast = 1;
	_spawnTimer = GetRandomSpawnDelay();
	_horrorTextures.clear();
	for (const auto& texture : GetHorrorTextureCache())
		_horrorTextures.push_back(texture.get());
	_horrorSoundKeys = { SoundData::SFX_JumpScareProwler, SoundData::SFX_JumpScareScream,
		SoundData::SFX_JumpScareLuigi, SoundData::SFX_JumpScareDoors, SoundData::SFX_JumpScareFreddy,
		SoundData::SFX_JumpScareWarn, SoundData::SFX_JumpScareFolder };
	_activeHorrorTexture = nullptr;
	_horrorEffectTimer = 0.0f;
	_horrorSoundTimer = 0.0f;
	_activeHorrorSound.clear();

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
			_monitorScreen.Play(MonitorScreenType::USE_BOM);
			for (const auto& enemy : _enemies)
				for (Bullet* bullet : enemy->GetBullets()) bullet->Deactivate();

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

	_monitorScreen.LoadAnimations();
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
	for (auto* particle : _deathParticles) particle->Render();
	for (auto* particle : _hitParticles) particle->Render();
	_layout.Print();
	if (_phase == E_StagePhase::BOSS && _bossHealthBar)
	{
		g2_FontDrawText(_uiFont, { 350, 0, 920, 30 }, 0xFFFFFFFF, "BOSS HP");
		_bossHealthBar->Render();
	}

	//============================
	//UI
	_monitorScreen.Render();
	_monitor.Print();

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



	g2_FontDrawText(_uiFont, { 0, 230 , 1270, 250}, 0xFFFFFFFF, "STAGE: %d", GameData::g_stageLast);
	const char* difficultyNames[] = { "EASY", "NORMAL", "HARD" };
	g2_FontDrawText(_uiFont, { 0, 200, 400, 250 }, 0xFFFFFFFF, "DIFFICULTY: %s",
		difficultyNames[GameData::g_diff]);
	if (_phase == E_StagePhase::WAVE)
		g2_FontDrawText(_uiFont, { 0, 260, 1270, 280}, 0xFFFFFFFF, "BOSS IN: %.0f", _stageTimer);
	else if (_phase == E_StagePhase::BOSS)
		g2_FontDrawText(_uiFont, { 0, 260, 1270, 280 }, 0xFFFFFFFF, "BOSS - %s",
			_enemies.empty() ? "" : _enemies.front()->GetBossPatternName());
	else
		g2_FontDrawText(_uiFont, { 430, 300, 900, 360 }, 0xFFFFFFFF, "STAGE CLEAR!");


	//============================

	if (_activeHorrorTexture) _activeHorrorTexture->Print();


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

	if (GameData::g_doJumpScare)
	{
		if (_horrorEffectTimer > 0.0f)
		{
			_horrorEffectTimer -= dt;
			if (_horrorEffectTimer <= 0.0f) _activeHorrorTexture = nullptr;
		}
		if (_horrorSoundTimer > 0.0f)
		{
			_horrorSoundTimer -= dt;
			if (_horrorSoundTimer <= 0.0f && !_activeHorrorSound.empty())
			{
				g_SoundManager.StopSFX(_activeHorrorSound);
				_activeHorrorSound.clear();
			}
		}
		if (_horrorEffectTimer <= 0.0f && _horrorSoundTimer <= 0.0f &&
			!_horrorTextures.empty() && !_horrorSoundKeys.empty() &&
			_randomizer.Rand(0, 999 * 2) == 0)
		{
			const int textureIndex = _randomizer.Rand(0, static_cast<int>(_horrorTextures.size()) - 1);
			_activeHorrorTexture = _horrorTextures[textureIndex];
			const int catWidths[] = { 812, 736, 387, 374, 162, 287, 287, 498, 735, 736, 800 };
			const int catHeights[] = { 992, 1229, 516, 498, 283, 252, 292, 498, 602, 626, 756 };

			const float scale = (std::max)(1280.0f / catWidths[textureIndex], 720.0f / catHeights[textureIndex]);

			_activeHorrorTexture->SetScale(VEC2(scale, scale));
			_activeHorrorTexture->SetPos(VEC2((1280.0f - catWidths[textureIndex] * scale) * 0.5f,
				(720.0f - catHeights[textureIndex] * scale) * 0.5f));

			_horrorEffectTimer = _randomizer.Rand(0.5f, MAX_HORROR_EFFECT_TIME);

			_activeHorrorSound = _horrorSoundKeys[_randomizer.Rand(0, static_cast<int>(_horrorSoundKeys.size()) - 1)];
			g_SoundManager.PlaySFX(_activeHorrorSound);
			_horrorSoundTimer = MAX_HORROR_EFFECT_TIME;
		}
	}
	else
	{
		_activeHorrorTexture = nullptr;
		_horrorEffectTimer = 0.0f;
		if (!_activeHorrorSound.empty())
		{
			g_SoundManager.StopSFX(_activeHorrorSound);
			_activeHorrorSound.clear();
			_horrorSoundTimer = 0.0f;
		}
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
	_monitorScreen.Update(dt);

	if (_phase == E_StagePhase::WAVE)
	{
		_stageTimer -= dt;
		_spawnTimer -= dt;
		if (_stageTimer <= 0.0f)
		{
			for (Enemy* enemy : _enemies) ReturnEnemy(enemy);
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

	for (Bullet* shotPtr : _player.GetShots())
	{
		Bullet& shot = *shotPtr;
		if (!shot.IsActive()) continue;
		for (const auto& enemy : _enemies)
		{
			if (!enemy->IsAlive()) continue;
			const VEC2 enemyPos(enemy->GetPos().x + 24.0f, enemy->GetPos().y + 24.0f);
			const VEC2 shotPos(shot.GetPos().x + 6.0f, shot.GetPos().y + 10.0f);
			if (!shot.GetCollider().IsEntered(shotPos, enemy->GetCollider(), enemyPos)) continue;

			enemy->TakeDamage(_player.GetShotDamage());
			g_SoundManager.PlaySFX(SoundData::SFX_EnemyHit);
			SpawnHitParticle(shotPos);
			if (!enemy->IsAlive())
			{
				GameData::g_score += _randomizer.Rand(GameData::g_stageLast * 100, GameData::g_stageLast * 200);
				SpawnDeathParticle(enemyPos -VEC2(48,48));
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
		_monitorScreen.Play(MonitorScreenType::ITEM_GET);
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
		for (Bullet* bulletPtr : enemy->GetBullets())
		{
			Bullet& bullet = *bulletPtr;
			if (!bullet.IsActive()) continue;
			const VEC2 bulletPos(bullet.GetPos().x + 6.0f, bullet.GetPos().y + 10.0f);
			const VEC2 core = _player.GetCorePos();
			if (bullet.GetCollider().IsEntered(bulletPos, _player.GetCollider(), core))
			{
				if (_player.OnHit())
				{
					--_lives;
					_monitorScreen.Play(MonitorScreenType::HIT);
				}
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
	auto updateParticles = [dt](std::vector<BomParticle*>& active, ObjectPool<BomParticle>& pool) {
		for (auto particle = active.begin(); particle != active.end();)
		{
			(*particle)->Update(dt);
			if (!(*particle)->IsAlive())
			{
				pool.Return(*particle);
				particle = active.erase(particle);
			}
			else ++particle;
		}
	};
	updateParticles(_deathParticles, _deathParticlePool);
	updateParticles(_hitParticles, _hitParticlePool);
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
	for (auto enemy = _enemies.begin(); enemy != _enemies.end();)
	{
		if (!(*enemy)->IsAlive())
		{
			ReturnEnemy(*enemy);
			enemy = _enemies.erase(enemy);
		}
		else ++enemy;
	}
	if (_phase == E_StagePhase::BOSS && _enemies.empty())
	{
		_phase = E_StagePhase::CLEAR;
		_monitorScreen.Play(MonitorScreenType::BOSS_CLEAR);
		GameData::g_score += 1000;
		_clearTimer = 3.0f;
		_bossHealthBar.reset();
	}




	return;
}

Enemy* InGameScene::AcquireEnemy(VEC2 pos, const EnemyConfig& config)
{
	for (auto& pooled : _enemyPools)
	{
		if (pooled.assetConfig.spritePath == config.spritePath
			&& pooled.assetConfig.animationPaths == config.animationPaths
			&& pooled.assetConfig.animationFrameDuration == config.animationFrameDuration
			&& pooled.assetConfig.bulletTexturePath == config.bulletTexturePath)
		{
			Enemy* enemy = pooled.objects.Get();
			if (!enemy) return nullptr;
			enemy->Reset(pos, config);
			return enemy;
		}
	}
	_enemyPools.emplace_back(config, pos);
	Enemy* enemy = _enemyPools.back().objects.Get();
	if (enemy) enemy->Reset(pos, config);
	return enemy;
}

BomParticle* InGameScene::AcquireParticle(VEC2 pos, bool deathEffect)
{
	ObjectPool<BomParticle>& pool = deathEffect ? _deathParticlePool : _hitParticlePool;
	BomParticle* particle = pool.Get();
	if (particle) particle->Reset(pos);
	return particle;
}

void InGameScene::ReturnEnemy(Enemy* enemy)
{
	if (!enemy) return;
	for (auto& pool : _enemyPools)
	{
		if (enemy->UsesAssets(pool.assetConfig))
		{
			pool.objects.Return(enemy);
			return;
		}
	}
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
	EnemyConfig config = ScaleEnemyForStage(
		ScaleEnemyForDifficulty(factories[type](), GameData::g_diff), GameData::g_stageLast);
	_enemies.push_back(AcquireEnemy(VEC2(static_cast<float>(x), static_cast<float>(y)), config));
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
	config = ScaleEnemyForStage(ScaleEnemyForDifficulty(config, GameData::g_diff), GameData::g_stageLast);
	_enemies.push_back(AcquireEnemy(VEC2(610.0f, -60.0f), config));
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
	_deathParticles.push_back(AcquireParticle(pos, true));
}

void InGameScene::SpawnHitParticle(VEC2 pos)
{
	_hitParticles.push_back(AcquireParticle(pos, false));
}

float InGameScene::GetRandomSpawnDelay()
{
	const int stage = GameData::g_stageLast > 0 ? GameData::g_stageLast : 1;
	const int minTenths = (std::max)(3, 10 - (stage - 1));
	const int maxTenths = (std::max)(minTenths, 40 - 3 * (stage - 1));
	const float spawnRateScale[] = { 1.25f, 1.0f, 0.8f };
	const int difficulty = (GameData::g_diff >= 0 && GameData::g_diff <= 2) ? GameData::g_diff : 0;
	return _randomizer.Rand(minTenths, maxTenths) / 10.0f * spawnRateScale[difficulty];
}

void InGameScene::Destroy()
{
	g_SoundManager.StopAll();
}

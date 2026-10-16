#include "InGameScene.h"

void InGameScene::Init()
{
	SetFullScreen();

	_enemies.clear();
	_items.clear();
	_bossHealthBar.reset();
	_phase = StagePhase::WAVE;
	_stageTimer = STAGE_TIME;
	_spawnTimer = 1.0f;
	_clearTimer = 0.0f;
	if (GameData::g_stageLast < 1) GameData::g_stageLast = 1;

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
		}
	);

	_inputMgr.RegisterAction
	(
		{ VK_ESCAPE },
		E_KeyType::DOWN,
		[this]()
		{

			//정지
		}
	);













	




	_layout.Load();
	_uiFont = g2_FontCreate("굴림", 28, 0);


	return;
}
void InGameScene::Render()
{
	IScene::Render();
	_player.Render();
	for (const auto& enemy : _enemies) enemy->Render();
	for (const Item& item : _items) item.Render(_uiFont);
	_layout.Print();
	if (_phase == StagePhase::BOSS && _bossHealthBar)
	{
		g2_FontDrawText(_uiFont, { 350, 0, 920, 30 }, 0xFFFFFFFF, "BOSS HP");
		_bossHealthBar->Render();
	}

	
	
	
	g2_FontDrawText
	(
		_uiFont,
		{ 980, 0, 1250, 125 },
		0xFFFFFFFF,
		"SCORE \n%010d", GameData::g_score
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
		{ 980, 100, 1250, 200 },
		0xFFFFFFFF,
		"LIVES \n%s",liv.c_str()
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
		{ 980, 200, 1270, 300 },
		0xFFFFAA40,
		"BOMB \n%s",bom.c_str()
	);

	//아이템
	if (_player.GetRapidFireTime() > 0.0f)
		g2_FontDrawText(_uiFont, { 980, 280, 1270, 325 }, 0xFF60FFFF, "RAPID: %.0fs", _player.GetRapidFireTime());
	if (_player.GetPowerTime() > 0.0f)
		g2_FontDrawText(_uiFont, { 980, 330, 1270, 375 }, 0xFFFF60FF, "POWER: %.0fs", _player.GetPowerTime());
	
	
	
	g2_FontDrawText(_uiFont, { 0, 140, 1270, 185 }, 0xFFFFFFFF, "STAGE: %d", GameData::g_stageLast);
	if (_phase == StagePhase::WAVE)
		g2_FontDrawText(_uiFont, { 0, 190, 1270, 235 }, 0xFFFFFFFF, "BOSS IN: %.0f", _stageTimer);
	else if (_phase == StagePhase::BOSS)
		g2_FontDrawText(_uiFont, { 0, 190, 1270, 235 }, 0xFFFFFFFF, "BOSS!");
	else
		g2_FontDrawText(_uiFont, { 430, 300, 900, 360 }, 0xFFFFFFFF, "STAGE CLEAR!");


	return;
}
void InGameScene::Update()
{
	IScene::Update();
	float dt = g_deltaTimer.GetDeltaTime();

	_player.SetFocus(false);

	_inputMgr.UpdateInput();

	_player.Update(dt);
	if (_phase == StagePhase::WAVE)
	{
		_stageTimer -= dt;
		_spawnTimer -= dt;
		if (_stageTimer <= 0.0f)
		{
			_enemies.clear();
			_phase = StagePhase::BOSS;
			SpawnBoss();
		}
		else if (_spawnTimer <= 0.0f && _enemies.size() < 6)
		{
			SpawnRandomEnemy();
			_spawnTimer = static_cast<float>(_randomizer.Rand(1, 4));
		}
	}
	else if (_phase == StagePhase::CLEAR)
	{
		_clearTimer -= dt;
		if (_clearTimer <= 0.0f)
		{
			++GameData::g_stageLast;
			_phase = StagePhase::WAVE;
			_stageTimer = STAGE_TIME;
			_spawnTimer = 1.0f;
		}
	}

	for (const auto& enemy : _enemies) enemy->Update(dt, _player.GetCorePos());
	if (_phase == StagePhase::BOSS && _bossHealthBar && !_enemies.empty())
		_bossHealthBar->SetProgress(static_cast<float>(_enemies.front()->GetHP()) /
			_enemies.front()->GetMaxHP());

	std::vector<Bullet>& playerShots = _player.GetShots();
	for (Bullet& shot : playerShots)
	{
		if (!shot.IsActive()) continue;
		for (const auto& enemy : _enemies)
		{
			if (!enemy->IsAlive()) continue;
			VEC2 enemyPos = enemy->GetPos();
			float dx = shot.GetPos().x + 6.0f - (enemyPos.x + 24.0f);
			float dy = shot.GetPos().y + 10.0f - (enemyPos.y + 24.0f);
			float radius = shot.GetRadius() + enemy->GetRadius();
			if (dx * dx + dy * dy <= radius * radius)
			{
				enemy->TakeDamage(_player.GetShotDamage());
				if (!enemy->IsAlive())
				{
					GameData::g_score += _randomizer.Rand(100,500);
					printf("점수 증가 확인: %d\n", GameData::g_score);
					TryDropItem(VEC2(enemyPos.x + 24.0f, enemyPos.y + 24.0f));
				}
				shot.Deactivate();
				break;
			}
		}
	}
	for (Item& item : _items)
	{
		if (!item.IsActive()) continue;
		item.Update(dt);
		if (!item.IsActive()) continue;
		VEC2 itemPos = item.GetPos();
		VEC2 core = _player.GetCorePos();
		const float dx = itemPos.x - core.x;
		const float dy = itemPos.y - core.y;
		const float radius = item.GetRadius() + _player.GetCoreRadius() + 8.0f;
		if (dx * dx + dy * dy <= radius * radius)
		{
			switch (item.GetType())
			{
			case ItemType::LIFE: ++_lives; break;
			case ItemType::BOMB: _player.AddBomb(); break;
			case ItemType::RAPID_FIRE: _player.AddRapidFire(); break;
			case ItemType::POWER: _player.AddPower(); break;
			}
			item.Collect();
		}
	}
	_items.erase(std::remove_if(_items.begin(), _items.end(),
		[](const Item& item) { return !item.IsActive(); }), _items.end());

	for (const auto& enemy : _enemies)
	{
		std::vector<Bullet>& enemyBullets = enemy->GetBullets();
		for (Bullet& bullet : enemyBullets)
		{
			if (!bullet.IsActive()) continue;
			VEC2 core = _player.GetCorePos();
			float dx = bullet.GetPos().x + 6.0f - core.x;
			float dy = bullet.GetPos().y + 10.0f - core.y;
			float radius = bullet.GetRadius() + _player.GetCoreRadius();
			if (dx * dx + dy * dy <= radius * radius)
			{
				if (_player.OnHit()) --_lives;
				bullet.Deactivate();
			}
		}
	}
	if (_lives <= 0)
	{
		g_SceneMgr.ChangeScene(new JumpScareScene(), E_SceneType::JUMP_SCARE);
		return;
	}
	_enemies.erase(std::remove_if(_enemies.begin(), _enemies.end(),
		[](const std::unique_ptr<Enemy>& enemy) { return !enemy->IsAlive(); }), _enemies.end());
	if (_phase == StagePhase::BOSS && _enemies.empty())
	{
		_phase = StagePhase::CLEAR;
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
	_enemies.emplace_back(new Enemy(VEC2(static_cast<float>(x), static_cast<float>(y)), factories[type]()));
}

void InGameScene::SpawnBoss()
{
	_enemies.emplace_back(new Enemy(VEC2(610.0f, -60.0f), MakeStageBoss()));
	_bossHealthBar.reset(new ProgressBar(VEC2(400.0f, 30.0f)));
	_bossHealthBar->SetProgress(1.0f);
}

void InGameScene::TryDropItem(VEC2 pos)
{
	if (_randomizer.Rand(0, 99) >= 35) return;
	_items.emplace_back(pos, static_cast<ItemType>(_randomizer.Rand(0, 3)));
}

void InGameScene::Destroy()
{

}

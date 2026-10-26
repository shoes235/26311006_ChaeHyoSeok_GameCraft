#include "ResultScene.h"
#include "InGameScene.h"
#include "SoundData.h"

void ResultScene::SpawnFumo()
{
	if (_fumos.size() >= MAX_ACTIVE_FUMOS) return;

	Fumo* fumo = nullptr;
	for (const auto& pooledFumo : _fumoPool)
	{
		if (!pooledFumo->IsActive())
		{
			fumo = pooledFumo.get();
			break;
		}
	}

	if (!fumo)
	{
		if (_fumoPool.size() >= MAX_ACTIVE_FUMOS) return;
		_fumoPool.push_back(std::make_unique<Fumo>(0.05f));
		fumo = _fumoPool.back().get();
	}

	fumo->Reset(static_cast<float>(_randomizer.Rand(0, _winSize.cx - 64)),
		_randomizer.Rand(250.0f, 560.0f), 220.0f);
	_fumos.push_back(fumo);
}

void ResultScene::UpdateFumos(float delta)
{
	for (auto it = _fumos.begin(); it != _fumos.end();)
	{
		(*it)->Update(delta);
		if (!(*it)->IsActive()) it = _fumos.erase(it);
		else ++it;
	}

	_fumoSpawnTimer += delta;
	if (_fumoSpawnTimer >= FUMO_SPAWN_INTERVAL)
	{
		_fumoSpawnTimer -= FUMO_SPAWN_INTERVAL;
		SpawnFumo();
	}
}
void ResultScene::Init()
{
	SetWindow(true);
	_fumos.clear();
	_fumoPool.clear();
	_fumoSpawnTimer = FUMO_SPAWN_INTERVAL;

	_inputMgr.RegisterAction
	(
		{ 'Z' },
		E_KeyType::DOWN,
		[=]()
		{
			InGameScene* nextScene = new InGameScene();
			g_SceneMgr.ChangeScene(nextScene, E_SceneType::IN_GAME);
		}
	);


	_inputMgr.RegisterAction
	(
		{ 'X' },
		E_KeyType::DOWN,
		[=]()
		{
			TitleScene* nextScene = new TitleScene();
			g_SceneMgr.ChangeScene(nextScene, E_SceneType::TITLE);
		}
	);


	SpawnFumo();

	_font = g2_FontCreate("굴림", 48, 0);
	_rankFont = g2_FontCreate("궁서", 100, 0);

	_finalScore = static_cast<int>(GameData::g_score * GameData::GetScoreMultiplier() + 0.5f);
	_rank = GetRank(_finalScore);

	g_SoundManager.PlaySFX(SoundData::SFX_ResultDrum);

	_state = E_ResultState::BEFORE_SHOW;
}

void ResultScene::Update()
{
	float dt = g_deltaTimer.GetDeltaTime();

	switch (_state)
	{
	case E_ResultState::BEFORE_SHOW:
	{
		_drumTimer += dt;
		//drum
		if (_drumTimer > DRUM_TIME)
		{
			_showRank = true;
			_state = E_ResultState::SHOW_RANK;
		}

	}break;

	case E_ResultState::SHOW_RANK:
	{

		if (static_cast<int>(_rank) < static_cast<int>(E_Rank::C))
			g_SoundManager.PlaySFX(SoundData::SFX_ResultBad);
		else
			g_SoundManager.PlaySFX(SoundData::SFX_ResultGood);

		_waitTimer = .0f;
		_state = E_ResultState::PLAY_SFX;

	} break;
	case E_ResultState::PLAY_SFX:
	{
		_waitTimer += dt;

		if (_waitTimer > WAIT_TIME)
		{
			_state = E_ResultState::PLAY_BGM;
		}

	} break;
	case E_ResultState::PLAY_BGM:
	{
		g_SoundManager.PlayBGM(SoundData::BGM_Result, true);

		_showInputTxt = true;

		_state = E_ResultState::GET_INPUT;

	}break;
	case E_ResultState::GET_INPUT:
	{
		_inputMgr.UpdateInput();
		UpdateFumos(dt);

	}break;
	}
}


void ResultScene::Render()
{
	if (_state == E_ResultState::GET_INPUT)
		for (Fumo* fumo : _fumos) fumo->Render();

	g2_FontDrawText(_font, { 380,200,1000,400}, 0xFF000000, "당신의 랭크는...");

	//g2_FontDrawText(_font, { 380, 130, 1000, 180 }, 0xFF000000, "SCORE: %010d", _finalScore);

	if (_showRank)
	{
		g2_FontDrawText
		(
			_rankFont,
			{ 500, 300, 800, 500 },
			0xFF000000,
			RANK_TEXT_LIST[static_cast<int>(_rank)].c_str()
		);
	}

	if (_showInputTxt)
	{
		g2_FontDrawText
		(
			_font,
			{ 380, 500, 1000, 800 },
			0xFF000000,
			"Z 를 눌러 다시 시작\nX 를 눌러 타이틀로"
		);
	}


}

void ResultScene::Destroy()
{
	_fumos.clear();
	_fumoPool.clear();
	g_SoundManager.StopAll();
}


E_Rank ResultScene::GetRank(float score)
{
	for (int i = static_cast<int>(E_Rank::S);
		i >= static_cast<int>(E_Rank::F); --i)
		if (score >= RANK_BY_SCORE[i])
			return static_cast<E_Rank>(i);

	return E_Rank::F;
}


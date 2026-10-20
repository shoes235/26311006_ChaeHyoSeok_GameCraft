#include "ResultScene.h"
#include "SoundData.h"

void ResultScene::Init()
{
	SetWindow(true);

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

	}break;
	}
}


void ResultScene::Render()
{
	g2_FontDrawText(_font, { 380,200,1000,400}, 0xFF000000, "당신의 랭크는...");

	g2_FontDrawText(_font, { 380, 130, 1000, 180 }, 0xFF000000, "SCORE: %010d", _finalScore);

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
			"Z 를 눌러 다시 시작"
		);
	}


}

void ResultScene::Destroy()
{
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


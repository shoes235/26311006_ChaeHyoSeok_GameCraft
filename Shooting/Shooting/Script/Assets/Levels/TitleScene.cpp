#include "TitleScene.h"
#include "SoundData.h"


void TitleScene::Init()
{
	IScene::Init();

	//audio
	g_SoundManager.PlayBGM(SoundData::BGM_Title, true);


	//font
	nFont = g2_FontCreate("굴림", 48, 0);

	//input 등록
	_inputMgr.RegisterAction
	(
		{ 'Z' },
		E_KeyType::DOWN,
		[=]() 
		{
			g_SoundManager.PlaySFX(SoundData::SFX_SelectAction);
			switch (_curType)
			{
			case E_TitleSceneType::MAIN:
			{
				_curType = E_TitleSceneType::SELECTOR;
			}break;

			case E_TitleSceneType::SELECTOR:
			{
				if (_selectWheel == 3)
					GameData::g_skipIntro = !GameData::g_skipIntro;
				else if (_selectWheel == 4)
					GameData::g_doJumpScare = !GameData::g_doJumpScare;
				else
				{
					_curType = E_TitleSceneType::SELCTED;
					GameData::g_diff = _selectWheel;
				}
			}break;
			case E_TitleSceneType::SELCTED:
			{

				_curType = E_TitleSceneType::GO;
				
				
			}break;

			}

			
		}
	);

	_inputMgr.RegisterAction
	(
		{ 'X' },
		E_KeyType::DOWN,
		[=]()
		{
			switch (_curType)
			{
			case E_TitleSceneType::SELECTOR:
			{
				_curType = E_TitleSceneType::MAIN;
			}break;
			case E_TitleSceneType::SELCTED:
			{
				_curType = E_TitleSceneType::SELECTOR;
			}break;
			}

		}
	);

	_inputMgr.RegisterAction
	(
		{ VK_UP },
		E_KeyType::DOWN,
		[=]()
		{
			if (_curType == E_TitleSceneType::SELECTOR)
			{
				g_SoundManager.PlaySFX(SoundData::SFX_Select);
				_selectWheel--;

				if (_selectWheel > MAX_SELECT_WHEEL)
					_selectWheel = 0;
				if (_selectWheel < 0)
					_selectWheel = MAX_SELECT_WHEEL;

			}



		}
	);


	_inputMgr.RegisterAction
	(
		{ VK_DOWN },
		E_KeyType::DOWN,
		[=]()
		{
			if (_curType == E_TitleSceneType::SELECTOR)
			{
				g_SoundManager.PlaySFX(SoundData::SFX_Select);
				_selectWheel++;

				if (_selectWheel > MAX_SELECT_WHEEL)
					_selectWheel = 0;
				if (_selectWheel < 0)
					_selectWheel = MAX_SELECT_WHEEL;

			}


		}
	);









	VEC2 zero = { 0,0 };

	string prefix = "rsc/imgs/cats/";
	vector<Texture*>texes(10);

	for (int i = 0; i < texes.size(); ++i)
	{
		texes[i] = new Texture(prefix + std::to_string(i) + ".png",zero);
		texes[i]->Load();

		_textures.push_back(texes[i]);
	}

	_background->Load();

	return;
}

void TitleScene::Update()
{
	_inputMgr.UpdateInput();

	switch (_curType)
	{
	case E_TitleSceneType::MAIN: 
	{
		_layOut.SetPos(VEC2(1280, 0));

		_blinkTimer += 0.016f;

		if (_blinkTimer >= 10.0f)
		{
			_blinkTimer = 0.0f;
			_showText = !_showText;
		}

		_colTime += 0.016f;

		if (_colTime >= 0.1f)
		{
			_colTime = 0;

			switch (_colorDir)
			{
			case 0:
				_g++;
				if (_g >= 255) _colorDir++;
				break;

			case 1:
				_r--;
				if (_r <= 0) _colorDir++;
				break;

			case 2:
				_b++;
				if (_b >= 255) _colorDir++;
				break;

			case 3:
				_g--;
				if (_g <= 0) _colorDir++;
				break;

			case 4:
				_r++;
				if (_r >= 255) _colorDir++;
				break;

			case 5:
				_b--;
				if (_b <= 0) _colorDir = 0;
				break;
			}

			_color = 0xFF000000 | (_r << 16) | (_g << 8) | _b;
		}

		//텍스쳐 랜덤 이동
		Randomizer randSys;

		_texTimer += 0.016f;
		if (_texTimer >= TEXTURE_WAIT_TIME)
		{
			_texTimer = 0.0f;

			for (auto* texture : _textures)
			{
				VEC2 rnPos;
				rnPos.x = randSys.Rand(0, _winSize.cx);
				rnPos.y = randSys.Rand(0, _winSize.cy);
				texture->SetPos(rnPos);
			}
		}
	}
	break;
	case E_TitleSceneType::SELECTOR:
	{
		VEC2 pos = _layOut.GetPos();
		if (pos.x > 720)
		{
			pos.x -= g_deltaTimer.GetDeltaTime() * 1000;
			_layOut.SetPos(pos);
		}
			
	}
	break;
	case E_TitleSceneType::SELCTED:
	{
		_blinkTimer += 0.016f;

		if (_blinkTimer >= 1.0f)
		{
			_blinkTimer = 0.0f;
			_showText = !_showText;
		}
	}
	break;
	case E_TitleSceneType::GO:
	{
		VEC2 pos = _layOut.GetPos();
		if (pos.x > 0)
		{
			pos.x -= g_deltaTimer.GetDeltaTime() * 1500;
			_layOut.SetPos(pos);
		}

		if (pos.x <= 0)
		{

			if (GameData::g_skipIntro)
			{
				InGameScene* nextScene = new InGameScene();
				g_SceneMgr.ChangeScene(nextScene, E_SceneType::IN_GAME);
			}
			else
			{
				HowToPlayScene* nextScene = new HowToPlayScene();
				g_SceneMgr.ChangeScene(nextScene, E_SceneType::HOW_2_PLAY);
			}
		}
	}break;


	}

	


	return;
}

void TitleScene::Render()
{
	IScene::Render();

	Randomizer randSys;

	_background->Print();

	for (auto* texture : _textures) texture->Print();

	_layOut.Print();

	if (_curType != E_TitleSceneType::MAIN)
		_layOut2.Print();

	switch (_curType)
	{
	case E_TitleSceneType::MAIN:
	{
		if (_showText)
			g2_FontDrawText(nFont, { 380,500,1000,800 }, _color, "Z 를 눌러 게임시작");

	}break;
	case E_TitleSceneType::SELECTOR:
	{
		if (_layOut.GetPos().x > 720)
			break;;
		DrawMenu();



	}break;
	case E_TitleSceneType::SELCTED:
	{


		DrawMenu();
	}break;

	}

	return;
}

void TitleScene::DrawMenu()
{
	g2_FontDrawText
		(
			nFont,
			{ 750,50,1200,800 },
			0xFFFFFFFF,
			"난이도 선택"
		);

	const string difficultyText[5] = {
		"EASY - Score x0.5",
		"NORMAL - Score x1.0",
		"HARD - Score x1.5",
		"조작 설명을 스킵",
		"쫄보는 하지마셈 \n점수 배율 + 0.5"
	};
	g2_FontDrawText(nFont, { 40, 40, 700, 140 }, 0xFFFFFFFF, "%s", difficultyText[_selectWheel].c_str());
	g2_FontDrawText
	(
		nFont,
		{ 50,550,1200,720},
		0xFFFFFFFF,
		"현재 점수 배율 : X %.1f",GameData::GetScoreMultiplier()
	);


	if (_curType == E_TitleSceneType::SELECTOR)
	{

		g2_FontDrawText
		(
			nFont,
			{ 800,50 + 75,1200,800 },
			(_selectWheel == 0)? 0xFFFFFF00 : 0xFFFFFFFF,
			"EASY"
		);

		g2_FontDrawText
		(
			nFont,
			{ 800,50 + 75 * 2,1200,800 },
			(_selectWheel == 1) ? 0xFFFFFF00 : 0xFFFFFFFF,
			"NORMAL"
		);


		g2_FontDrawText
		(
			nFont,
			{ 800,50 + 75 * 3,1200,800 },
			(_selectWheel == 2) ? 0xFFFFFF00 : 0xFFFFFFFF,
			"HARD"
		);

	}else if (_curType == E_TitleSceneType::SELCTED)
	{

		if (_showText && _selectWheel == 0)
		{
			g2_FontDrawText
			(
				nFont,
				{ 800,50 + 75,1200,800 },
				(_selectWheel == 0)? 0xFFFFFF00 : 0xFFFFFFFF,
				"EASY"
			);
		}

		if (_showText && _selectWheel == 1)
		{
			g2_FontDrawText
			(
				nFont,
				{ 800,50 + 75 * 2,1200,800 },
				(_selectWheel == 1) ? 0xFFFFFF00 : 0xFFFFFFFF,
				"NORMAL"
			);
		}

		if (_showText && _selectWheel == 2)
		{
			g2_FontDrawText
			(
				nFont,
				{ 800,50 + 75 * 3,1200,800 },
				(_selectWheel == 2) ? 0xFFFFFF00 : 0xFFFFFFFF,
				"HARD"
			);
		}


	}



	g2_FontDrawText
	(
		nFont,
		{ 750,400,1200,800 },
		0xFFFFFFFF,
		"설정"
	);

	g2_FontDrawText
	(
		nFont,
		{ 800,400 + 75,1200,800 },
		(_selectWheel == 3) ? 0xFFFFFF00 : 0xFFFFFFFF,
		"[%s] 조작 설명 스킵",(GameData::g_skipIntro)? "X" : " "
	);

	g2_FontDrawText
	(
		nFont,
		{ 800,400 + 75 * 2,1200,800 },
		(_selectWheel == 4) ? 0xFFFFFF00 : 0xFFFFFFFF,
		"[%s] 공포 모드",(GameData::g_doJumpScare)? "X" : " "
	);

	g2_FontDrawText
	(
		nFont,
		{ 50,650,1200,720},
		0xFFFFFFFF,
		"[Z] 선택 [X] 취소"
	);

}




void TitleScene::Destroy()
{
	delete _background;
	_background = nullptr;

	for (auto& texture : _textures)
	{
		delete texture;
		texture = nullptr;
	}

	_textures.clear();

	g_SoundManager.StopAll();

	return;
}
#pragma once
#include <glc2d.h>
#include "IScene.h"
#include "HowToPlayScene.h"
#include "Texture.h"
#include "Randomizer.h"
#include "InputManager.h"

#include <vector>
using std::vector;

class TitleScene : public IScene
{
private:
	enum class E_TitleSceneType
	{
		MAIN,
		SELECTOR,
		SELCTED,
		GO
	};

	E_TitleSceneType _curType{};



	vector<Texture*> _textures;
	float _texTimer = 0.0f;
	const float TEXTURE_WAIT_TIME = 1.5f;

	//Tex
	Texture* _background = new Texture("rsc/imgs/MainMenu.png", VEC2(0, 0));
	Texture _layOut{ "rsc/imgs/UI/Black.png",VEC2(1280,0) };
	Texture _layOut2{ "rsc/imgs/UI/Black2.png",VEC2(0,0) };


	//text
	int nFont = 0;
	float _blinkTimer = .0f;
	bool _showText {true};

	//Select
	const int MAX_SELECT_WHEEL{ 4 };
	// EASY , NOR, HARD, 설정
	int _selectWheel{};

	// bool _skipIntro{};

	//text-col

	float _colTime = 0.0f;
	DWORD _color = 0xFFFFFFFF;

	int _r = 255;
	int _g = 0;
	int _b = 0;
	int _colorDir = 0;




	//Input
	InputManager _inputMgr;
public :
	TitleScene() : IScene(E_SceneType::TITLE)
	{
		_winName = "Title 창";
		_winColor = 0x000000; // 검은색

		_winPos =
		{
			(GetSystemMetrics(SM_CXSCREEN) - _winSize.cx) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - _winSize.cy) / 2,
		};
	}
public:
	void Init() override;
	void Render() override;
	void Update() override;
	void Destroy() override;
private:
	void DrawMenu();

};


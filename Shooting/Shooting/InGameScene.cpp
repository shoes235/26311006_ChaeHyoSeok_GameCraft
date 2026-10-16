#include "InGameScene.h"

void InGameScene::Init()
{
	SetWindow(false);

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
			_player.UseBomb();
		}
	);

	_inputMgr.RegisterAction
	(
		{ VK_ESCAPE },
		E_KeyType::DOWN,
		[this]()
		{
			//������
		}
	);





















	return;
}
void InGameScene::Render()
{
	IScene::Render();
	_player.Render();
	

	return;
}
void InGameScene::Update()
{
	IScene::Update();
	float dt = g_deltaTimer.GetDeltaTime();

	_player.SetFocus(false);

	_inputMgr.UpdateInput();

	_player.Update(dt);

	return;
}

void InGameScene::Destroy()
{

}

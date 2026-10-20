#include "JumpScareScene.h"
#include "SoundData.h"

void JumpScareScene::Init()
{
	SetFullScreen();

	_img->Load();

	g_SoundManager.StopBGM();

	return;
}

void JumpScareScene::Update()
{
	float dt = g_deltaTimer.GetDeltaTime();
	_timer += dt;

	if (_timer >= JUMP_SCARE_TIME)
	{
		if(!_trigger)
			g_SoundManager.PlaySFX(SoundData::SFX_JumpScare);
		_trigger = true;

	}

	if (_timer >=
		JUMP_SCARE_TIME + SCENE_CHANGE_TIME)
	{
		ResultScene* nextScene = new ResultScene();
		g_SceneMgr.ChangeScene(nextScene, E_SceneType::RESULT);
	}

		return;
}

void JumpScareScene::Render()
{
	if(_trigger)
		_img->Print();

	return;
}

void JumpScareScene::Destroy()
{
	g_SoundManager.StopAll();
}
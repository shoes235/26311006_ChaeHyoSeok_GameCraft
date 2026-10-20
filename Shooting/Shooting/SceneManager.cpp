#include "SceneManager.h"
#include "SoundManager.h"
#include <windows.h>

int FrameMove()
{
	g_deltaTimer.Update();

	IScene* scene = g_SceneMgr.GetCurrentScene();

	if (scene != nullptr)
	{
		scene->Update();
	}

	g_SceneMgr.ApplySceneChange();

	return 0;
}

int G_AppRender()
{
	if (g_SceneMgr.GetCurrentScene() != nullptr)
	{
		g_SceneMgr.GetCurrentScene()->Render();
	}
	return 0;
}

void SceneManager::InitEngine()
{
	g2_InitSdk();
	g2_CreateWin(100, 100, 1280, 720, "None");

	// Place the custom window icon at rsc/icon.ico.
	HWND hwnd = GetActiveWindow();
	HICON icon = static_cast<HICON>(LoadImageW(
		nullptr, L"rsc/icon.ico", IMAGE_ICON, 0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE));
	if (hwnd != nullptr && icon != nullptr)
	{
		SendMessageW(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
		SendMessageW(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
	}


	g2_SetRender(G_AppRender);
	g2_SetFrameMove(FrameMove);

}

void SceneManager::Run()
{
	g2_Run();

	if (_curScenePtr != nullptr)
	{
		_curScenePtr->Destroy();
		delete _curScenePtr;
		_curScenePtr = nullptr;
	}
	g_SoundManager.Clear();
	g2_DestroyWin();
}

void SceneManager::ChangeScene(IScene* nextScene, E_SceneType targetType)
{

	if (_changeRequested)
	{
		delete nextScene;
		return;
	}

	_nextScenePtr = nextScene;
	_nextSceneType = targetType;
	_changeRequested = true;
}

void SceneManager::ApplySceneChange()
{
    if (!_changeRequested)
        return;

    if (_curScenePtr != nullptr)
    {
        _curScenePtr->Destroy();
        delete _curScenePtr;
        _curScenePtr = nullptr;
    }

    _curScenePtr = _nextScenePtr;
    _nextScenePtr = nullptr;

    _curSceneType = _nextSceneType;
    _changeRequested = false;

	_curScenePtr->Init();
}

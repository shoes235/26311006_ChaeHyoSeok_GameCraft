#include <iostream>

#include <string>
using std::string;

#include <glc2d.h>

// link the 2d game library
#if defined(_DEBUG)
#if defined(_M_X64) // 64-bit 아키텍처
#pragma comment(lib, "glc2d_x64_debug.lib")
#elif defined(_M_IX86) // 32-bit 아키텍처
#pragma comment(lib, "glc2d_win32_debug.lib")
#endif
#else
#if defined(_M_X64)
#pragma comment(lib, "glc2d_x64_release.lib")
#elif defined(_M_IX86)
#pragma comment(lib, "glc2d_win32_release.lib")
#endif
#endif


#include "SceneManager.h"
#include "DeltaTimer.h"

#include "TitleScene.h"
#include "SoundData.h"



//===============================================================================================

//g_var

SceneManager g_SceneMgr;

static int g_verFont{};
const string VERSION {"v1.0.2"};

void DrawVersion()
{
	g2_FontDrawText(g_verFont, { 1180, 680, 1280, 720 }, 0xFFFFFFFF, VERSION.c_str());
}

//===============================================================================================

//Life Cycle

int main()
{
	std::cout << VERSION << std::endl;
	std::cout << " \"애플리케이션 제어 정책에서 이 파일을 차단했습니다\" 문구가 뜬다면 \n [방법 1.]윈도우 보안에서 스마트 앱 컨트롤 끔으로 설정\n [방법 2.]Windows 보안의 '바이러스 및 위협 방지' 설정에서 차단된 파일을 제외 목록에 추가\n";
	
	//Birth
	g_SceneMgr.InitEngine();
	SoundData::RegisterAll(g_SoundManager);
	g_deltaTimer.Init();
	g_verFont = g2_FontCreate("굴림",18,0);


	TitleScene* title = new TitleScene();
	g_SceneMgr.ChangeScene(title, E_SceneType::TITLE);



	//Life::Update
	g_SceneMgr.Run();



	//Death
	return 0;
}
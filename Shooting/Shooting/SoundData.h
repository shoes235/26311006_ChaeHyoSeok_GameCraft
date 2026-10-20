#pragma once

#include <string>
using std::string;

#include "SoundManager.h"

namespace SoundData
{
	constexpr char BGM_Title[] = "BGM_Title";
	constexpr char BGM_Result[] = "BGM_Result";
	constexpr char BGM_Game[] = "BGM_Game";
	constexpr char BGM_GameAlt[] = "BGM_GameAlt";

	constexpr char SFX_Shot[] = "SFX_Shot";
	constexpr char SFX_Bomb[] = "SFX_Bomb";
	constexpr char SFX_PlayerDeath[] = "SFX_PlayerDeath";
	constexpr char SFX_Graze[] = "SFX_Graze";
	constexpr char SFX_ItemGet[] = "SFX_ItemGet";
	constexpr char SFX_EnemyDeath[] = "SFX_EnemyDeath";
	constexpr char SFX_JumpScare[] = "SFX_JumpScare";
	constexpr char SFX_ResultDrum[] = "SFX_ResultDrum";
	constexpr char SFX_ResultBad[] = "SFX_ResultBad";
	constexpr char SFX_ResultGood[] = "SFX_ResultGood";

	constexpr char SFX_Select[] = "SFX_Select";
	constexpr char SFX_SelectAction[] = "SFX_SelectAction";

	inline bool RegisterAll(SoundManager& soundManager)
	{
		bool success = true;

		//&= 연산자
		//true / false 를 반환
		//전체 로딩 실패시 무조건 false 
		success &= soundManager.AddBGM(BGM_Title, "rsc/audio/MainTheme.wav");
		success &= soundManager.AddBGM(BGM_Result, "rsc/audio/Result.mp3");
		success &= soundManager.AddBGM(BGM_Game, "rsc/audio/InGameTheme.wav");
		success &= soundManager.AddBGM(BGM_GameAlt, "rsc/audio/theme.wav");

		success &= soundManager.AddSFX(SFX_Shot, "rsc/audio/SFX/baka-cirno.mp3");
		success &= soundManager.AddSFX(SFX_Bomb, "rsc/audio/SFX/fart.mp3");
		success &= soundManager.AddSFX(SFX_PlayerDeath, "rsc/audio/SFX/pichuun.mp3");
		success &= soundManager.AddSFX(SFX_Graze, "rsc/audio/SFX/graze.mp3");
		success &= soundManager.AddSFX(SFX_ItemGet, "rsc/audio/SFX/Get.mp3");
		success &= soundManager.AddSFX(SFX_EnemyDeath, "rsc/audio/SFX/deltarune-explosion.mp3");
		success &= soundManager.AddSFX(SFX_JumpScare, "rsc/audio/SFX/JumpScare.mp3");
		success &= soundManager.AddSFX(SFX_ResultDrum, "rsc/audio/SFX/drum.mp3");
		success &= soundManager.AddSFX(SFX_ResultBad, "rsc/audio/SFX/fart.mp3");
		success &= soundManager.AddSFX(SFX_ResultGood, "rsc/audio/SFX/yay.mp3");

		success &= soundManager.AddSFX(SFX_Select, "rsc/audio/SFX/select06.mp3");
		success &= soundManager.AddSFX(SFX_SelectAction, "rsc/audio/SFX/coin01.mp3");

		return success;
	}
}

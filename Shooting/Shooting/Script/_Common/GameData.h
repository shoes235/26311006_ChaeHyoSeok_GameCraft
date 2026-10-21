#pragma once

enum class E_GaneDifficulty
{
	EASY,
	NORMAL,
	HARD
};

class GameData
{
public:
	static int g_score;
	static int g_stageLast;

	//Setting
	static int g_diff;

	static float g_scoreMult;
	static float GetScoreMultiplier();

	static bool g_skipIntro;
	static bool g_doJumpScare;
};


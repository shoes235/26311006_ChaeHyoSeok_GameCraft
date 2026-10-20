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
	static int g_diff;
	static float GetScoreMultiplier();
};


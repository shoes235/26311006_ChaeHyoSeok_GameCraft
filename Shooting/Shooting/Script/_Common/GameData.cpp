#include "GameData.h"
int GameData::g_score{};
int GameData::g_stageLast{};
int GameData::g_diff{static_cast<int>(E_GaneDifficulty::EASY)};

bool GameData::g_skipIntro{};
bool GameData::g_doJumpScare{};

float GameData::g_scoreMult{};

float GameData::GetScoreMultiplier()
{
	switch (static_cast<E_GaneDifficulty>(g_diff))
	{
	case E_GaneDifficulty::NORMAL: g_scoreMult = 1.0f;
			break;
	case E_GaneDifficulty::HARD: g_scoreMult = 1.5f;
			break;
	default: g_scoreMult = 0.5f;
			break;
	}

	if (g_doJumpScare)
		g_scoreMult += 0.5f;

	return g_scoreMult;
}

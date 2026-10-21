#include "GameData.h"
int GameData::g_score{};
int GameData::g_stageLast{};
int GameData::g_diff{static_cast<int>(E_GaneDifficulty::EASY)};

bool GameData::g_skipIntro{};
bool GameData::g_doJumpScare{};

float GameData::GetScoreMultiplier()
{
	switch (static_cast<E_GaneDifficulty>(g_diff))
	{
	case E_GaneDifficulty::NORMAL: return 1.0f;
	case E_GaneDifficulty::HARD: return 1.5f;
	default: return 0.5f;
	}
}

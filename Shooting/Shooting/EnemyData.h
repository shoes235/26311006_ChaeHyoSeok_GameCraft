#pragma once
#include "Enemy.h"
#include "Randomizer.h"
#include <cmath>

struct BossPatternData
{
	const char* name;
	float duration;
	E_EnemyShotPattern shotPattern;
	int bulletCount;
	float spreadAngle;
	float fireInterval;
	float bulletSpeed;
	float startDelay;
};

inline EnemyConfig ScaleEnemyForStage(EnemyConfig config, int stage)
{
	if (stage < 1) stage = 1;
	config.hp = static_cast<int>(std::ceil(config.hp * (1.0f + 0.12f * (stage - 1))));
	return config;
}

inline BossPatternData GetStageBossPatternData(int bossType, int phase)
{
	if (bossType == 1)
	{
		switch (phase)
		{
		case 1: return { "SPELL 1 - GASTER BLAST", 10.0f, E_EnemyShotPattern::AIMED_SINGLE, 1, 0.0f, 0.18f, 290.0f, .65f };
		case 2: return { "SPELL 2 - SPIRAL BONE", 14.0f, E_EnemyShotPattern::ROTATING_BURST, 14, 0.0f, 0.32f, 185.0f, .65f };
		default: return { "NORMAL - THOROW BONE", 8.0f, E_EnemyShotPattern::AIMED_SPREAD, 3, 0.55f, 1.2f, 190.0f, .65f };
		}
	}
	if (bossType == 2)
	{
		switch (phase)
		{
		case 1: return { "SPELL 1 - SAL RAIN", 11.0f, E_EnemyShotPattern::STRAIGHT_SPREAD, 9, 0.8f, 0.75f, 245.0f, .65f };
		case 2: return { "SPELL 2 - RADIAL", 13.0f, E_EnemyShotPattern::RADIAL_BURST, 20, 0.0f, 1.1f, 165.0f, .65f };
		default: return { "NORMAL - AIMED SALVO", 8.0f, E_EnemyShotPattern::AIMED_SPREAD, 3, 0.45f, 1.0f, 200.0f, .65f };
		}
	}

	//Default
	switch (phase)
	{
	case 1: return { "SPELL 1", 12.0f, E_EnemyShotPattern::AIMED_SPREAD, 5, .65f, 0.9f, 230.0f, .65f };
	case 2: return { "SPELL 2", 12.0f, E_EnemyShotPattern::ROTATING_BURST, 18, .65f, .5f, 150.0f, .65f };
	default: return { "NORMAL", 8.0f, E_EnemyShotPattern::AIMED_SINGLE, 1, .0f, .75f, 210.0f, .65f };
	}
}


inline EnemyConfig MakeFastEnemyConfig()
{
	EnemyConfig config;
	config.hp = 5;
	config.moveSpeed = 160.0f;
	config.stopY = 180.0f;
	config.initialFireDelay = 0.5f;
	config.fireInterval = 0.5f;
	config.bulletSpeed = 340.0f;
	config.shotPattern = E_EnemyShotPattern::AIMED_SINGLE;

	config.spritePath = "rsc/imgs/Enemies/0.png";
	return config;
}

inline EnemyConfig MakeShotGunEnemy()
{
	EnemyConfig config;
	config.hp = 5;
	config.moveSpeed = 160.0f;
	config.stopY = 180.0f;
	config.initialFireDelay = 0.5f;
	config.fireInterval = 1.6f;
	config.bulletSpeed = 220.0f;
	config.shotPattern = E_EnemyShotPattern::AIMED_SPREAD;
	config.bulletCount = 9;
	config.spreadAngle = 0.9f;

	config.spritePath = "rsc/imgs/Enemies/1.png";

	return config;
}

inline EnemyConfig MakeAimedFiveShotEnemy()
{
	EnemyConfig config;
	config.hp = 4;
	config.moveSpeed = 65.0f;
	config.stopY = 140.0f;
	config.fireInterval = 1.3f;
	config.bulletSpeed = 280.0f;
	config.shotPattern = E_EnemyShotPattern::AIMED_SPREAD;
	config.bulletCount = 5;
	config.spreadAngle = 0.4f;
	
	config.spritePath = "rsc/imgs/Enemies/2.png";

	return config;
}

inline EnemyConfig MakeRapidStraightEnemy()
{
	EnemyConfig config;
	config.hp = 2;
	config.moveSpeed = 115.0f;
	config.stopY = 190.0f;
	config.initialFireDelay = 0.3f;
	config.fireInterval = 0.7f;
	config.bulletSpeed = 390.0f;
	config.shotPattern = E_EnemyShotPattern::STRAIGHT_RAPID;

	config.spritePath = "rsc/imgs/Enemies/3.png";

	return config;
}

inline EnemyConfig MakeStraightShotgunEnemy()
{
	EnemyConfig config;
	config.hp = 4;
	config.moveSpeed = 65.0f;
	config.stopY = 140.0f;
	config.fireInterval = 1.4f;
	config.bulletSpeed = 280.0f;
	config.shotPattern = E_EnemyShotPattern::STRAIGHT_SPREAD;
	config.bulletCount = 5;
	config.spreadAngle = 0.55f;

	config.spritePath = "rsc/imgs/Enemies/4.png";

	return config;
}

inline EnemyConfig MakeRadialShotgunEnemy()
{
	EnemyConfig config;
	config.hp = 6;
	config.moveSpeed = 40.0f;
	config.stopY = 165.0f;
	config.initialFireDelay = 1.2f;
	config.fireInterval = 2.0f;
	config.bulletSpeed = 210.0f;
	config.shotPattern = E_EnemyShotPattern::RADIAL_BURST;
	config.bulletCount = 12;
	config.bulletRadius = 6.0f;

	config.spritePath = "rsc/imgs/Enemies/5.png";

	return config;
}


//Bosses
inline EnemyConfig MakeStageBoss()
{
	EnemyConfig config;
	config.hp = 1000;
	config.radius = 32.0f;
	config.moveSpeed = 45.0f;
	config.stopY = 100.0f;
	config.initialFireDelay = 1.0f;
	config.fireInterval = 0.9f;
	config.bulletSpeed = 210.0f;
	config.bulletRadius = 7.0f;
	config.bulletCount = 3;
	config.shotPattern = E_EnemyShotPattern::AIMED_SPREAD;
	config.cycleBossPatterns = true;
	config.bossType = 0;

	vector<string> paths{};
	for (int i = 1; i < 50; ++i)
		paths.push_back("rsc/imgs/Enemies/Boss/(" + std::to_string(i) + ").png");
	config.animationPaths = paths;

	return config;
}

inline EnemyConfig MakeStageBoss2()
{
	EnemyConfig config = MakeStageBoss();
	config.bossType = 1;
	config.hp = 1250;
	config.moveSpeed = 35.0f;
	config.bulletRadius = 6.0f;
	config.animationPaths = { "rsc/imgs/Enemies/Boss1/0.png" };
	config.bulletTexturePath = "rsc/imgs/Bullets/BoneShot.png";
	return config;

}

inline EnemyConfig MakeStageBoss3()
{
	EnemyConfig config = MakeStageBoss();
	config.bossType = 2;
	config.hp = 850;
	config.moveSpeed = 60.0f;
	config.bulletRadius = 5.0f;
	config.animationPaths = { "rsc/imgs/Enemies/Boss2/0.png" };
	return config;
}

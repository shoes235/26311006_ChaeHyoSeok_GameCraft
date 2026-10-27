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

inline EnemyConfig ScaleEnemyForDifficulty(EnemyConfig config, int difficulty)
{
	const float hpScale[] = { 0.75f, 1.0f, 1.25f };
	const float moveSpeedScale[] = { 0.9f, 1.0f, 1.15f };
	const float fireIntervalScale[] = { 1.25f, 1.0f, 0.8f };
	const float bulletSpeedScale[] = { 0.8f, 1.0f, 1.2f };
	if (difficulty < 0 || difficulty > 2) difficulty = 0;

	config.hp = (config.hp > 0) ? static_cast<int>(std::ceil(config.hp * hpScale[difficulty])) : 1;
	config.moveSpeed *= moveSpeedScale[difficulty];
	config.initialFireDelay *= fireIntervalScale[difficulty];
	config.fireInterval *= fireIntervalScale[difficulty];
	config.bulletSpeed *= bulletSpeedScale[difficulty];
	config.difficultyFireIntervalScale = fireIntervalScale[difficulty];
	config.difficultyBulletSpeedScale = bulletSpeedScale[difficulty];
	return config;
}

inline BossPatternData GetStageBossPatternData(int bossType, int phase)
{
	if (bossType == 4)
	{
		switch (phase)
		{
		case 0: return { "SPELL 1 - BONE THROW", 12.0f, E_EnemyShotPattern::RADIAL_BURST, 16, 0.0f, 0.8f, 180.0f, 0.9f };
		case 1: return { "SPELL 2 - GASTER BLAST", 12.0f, E_EnemyShotPattern::STRAIGHT_SPREAD, 13, 1.3f, 0.65f, 240.0f, 0.8f };
		case 2: return { "SPELL 3 - TRIPLE BONE", 10.0f, E_EnemyShotPattern::RADIAL_BURST, 24, 0.0f, 0.65f, 210.0f, 0.8f };
		case 3: return { "SPELL 4 - CROSSING GASTER BLAST", 12.0f, E_EnemyShotPattern::STRAIGHT_SPREAD, 17, 1.8f, 0.5f, 275.0f, 0.7f };
		case 4: return { "SPELL 5 - RADIAL BONE", 10.0f, E_EnemyShotPattern::RADIAL_BURST, 30, 0.0f, 0.55f, 235.0f, 0.7f };
		default: return { "SPELL 6 - BONE THROW", 10.0f, E_EnemyShotPattern::STRAIGHT_SPREAD, 21, 2.2f, 0.38f, 300.0f, 0.7f };
		}
	}
	if (bossType == 5)
	{
		switch (phase)
		{
		case 0: return { "SPELL 1 - HUNTER'S LATTICE", 12.0f, E_EnemyShotPattern::AIMED_SPREAD, 7, 0.7f, 0.8f, 215.0f, 0.9f };
		case 1: return { "SPELL 2 - ROTATING CAGE", 12.0f, E_EnemyShotPattern::ROTATING_BURST, 16, 0.0f, 0.42f, 165.0f, 0.8f };
		case 2: return { "SPELL 3 - HUNTER'S LATTICE", 10.0f, E_EnemyShotPattern::AIMED_SPREAD, 11, 1.0f, 0.65f, 240.0f, 0.8f };
		case 3: return { "SPELL 4 - ROTATING CAGE", 12.0f, E_EnemyShotPattern::ROTATING_BURST, 22, 0.0f, 0.34f, 190.0f, 0.7f };
		case 4: return { "SPELL 5 - LAST HUNT", 10.0f, E_EnemyShotPattern::AIMED_SPREAD, 15, 1.25f, 0.5f, 270.0f, 0.7f };
		default: return { "SPELL 6 - CLOSING CAGE", 10.0f, E_EnemyShotPattern::ROTATING_BURST, 28, 0.0f, 0.27f, 215.0f, 0.7f };
		}
	}
	if (bossType == 3)
	{
		switch (phase)
		{
		case 0: return { "SPELL 1 - SEEKING GAP", 12.0f, E_EnemyShotPattern::AIMED_GAP_RING, 18, 0.0f, 1.25f, 170.0f, 1.0f };
		case 1: return { "SPELL 2 - WAVING BLADES", 12.0f, E_EnemyShotPattern::WAVING_SPREAD, 11, 1.8f, 0.55f, 235.0f, 0.8f };
		case 2: return { "SPELL 3 - SEEKING GAP", 10.0f, E_EnemyShotPattern::AIMED_GAP_RING, 24, 0.0f, 0.9f, 190.0f, 0.8f };
		case 3: return { "SPELL 4 - LAST HORIZON", 12.0f, E_EnemyShotPattern::WAVING_SPREAD, 15, 2.2f, 0.4f, 270.0f, 0.7f };
		case 4: return { "SPELL 5 - COLLAPSING GAP", 12.0f, E_EnemyShotPattern::AIMED_GAP_RING, 30, 0.0f, 0.7f, 215.0f, 0.7f };
		default: return { "SPELL 6 - FINAL WAVE", 10.0f, E_EnemyShotPattern::WAVING_SPREAD, 19, 2.6f, 0.32f, 300.0f, 0.7f };
		}
	}
	if (bossType == 1)
	{
		switch (phase)
		{
		case 0: return { "SPELL 1 - GASTER BLAST", 10.0f, E_EnemyShotPattern::AIMED_SINGLE, 1, 0.0f, 0.18f, 290.0f, .65f };
		case 1: return { "SPELL 2 - SPIRAL BONE", 14.0f, E_EnemyShotPattern::ROTATING_BURST, 14, 0.0f, 0.32f, 185.0f, .65f };
		case 2: return { "SPELL 3 - THOROW BONE", 8.0f, E_EnemyShotPattern::AIMED_SPREAD, 3, 0.55f, 1.2f, 190.0f, .65f };
		case 3: return { "SPELL 4 - GASTER BLAST", 10.0f, E_EnemyShotPattern::AIMED_SPREAD, 5, 0.7f, 0.75f, 230.0f, .65f };
		case 4: return { "SPELL 5 - SPIRAL BONE", 14.0f, E_EnemyShotPattern::ROTATING_BURST, 20, 0.0f, 0.25f, 200.0f, .65f };
		default: return { "SPELL 6 - BONE RAIN", 8.0f, E_EnemyShotPattern::RADIAL_BURST, 24, 0.0f, 0.9f, 175.0f, .65f };
		}
	}
	if (bossType == 2)
	{
		switch (phase)
		{
		case 0: return { "SPELL 1 - SAL RAIN", 11.0f, E_EnemyShotPattern::STRAIGHT_SPREAD, 9, 0.8f, 0.75f, 245.0f, .65f };
		case 1: return { "SPELL 2 - RADIAL", 13.0f, E_EnemyShotPattern::RADIAL_BURST, 20, 0.0f, 1.1f, 165.0f, .65f };
		case 2: return { "SPELL 3 - AIMED SALVO", 8.0f, E_EnemyShotPattern::AIMED_SPREAD, 3, 0.45f, 1.0f, 200.0f, .65f };
		case 3: return { "SPELL 4 - SAL RAIN", 11.0f, E_EnemyShotPattern::STRAIGHT_SPREAD, 13, 1.0f, 0.6f, 270.0f, .65f };
		case 4: return { "SPELL 5 - RADIAL", 13.0f, E_EnemyShotPattern::RADIAL_BURST, 28, 0.0f, 0.8f, 190.0f, .65f };
		default: return { "SPELL 6 - AIMED SALVO", 8.0f, E_EnemyShotPattern::AIMED_SPREAD, 5, 0.65f, 0.75f, 230.0f, .65f };
		}
	}

	//Default
	switch (phase)
	{
	case 0: return { "SPELL 1", 12.0f, E_EnemyShotPattern::AIMED_SPREAD, 5, .65f, 0.9f, 230.0f, .65f };
	case 1: return { "SPELL 2", 12.0f, E_EnemyShotPattern::ROTATING_BURST, 18, .65f, .5f, 150.0f, .65f };
	case 2: return { "SPELL 3", 8.0f, E_EnemyShotPattern::AIMED_SINGLE, 1, .0f, .75f, 210.0f, .65f };
	case 3: return { "SPELL 4", 12.0f, E_EnemyShotPattern::STRAIGHT_SPREAD, 9, .8f, .7f, 240.0f, .65f };
	case 4: return { "SPELL 5", 12.0f, E_EnemyShotPattern::ROTATING_BURST, 24, .0f, .35f, 175.0f, .65f };
	default: return { "SPELL 6", 8.0f, E_EnemyShotPattern::RADIAL_BURST, 24, .0f, .9f, 180.0f, .65f };
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
	config.hp = 650;
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
	config.hp = 800;
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
	config.hp = 500;
	config.moveSpeed = 60.0f;
	config.bulletRadius = 5.0f;
	config.animationPaths = { "rsc/imgs/Enemies/Boss2/0.png" };
	return config;
}

inline EnemyConfig MakeFinalBoss()
{
	EnemyConfig config = MakeStageBoss3();
	config.hp = 1200;
	config.moveSpeed = 45.0f;
	config.bossType = 3;
	config.animationPaths = { "rsc/imgs/Enemies/Boss3/0.png" };
	config.bulletTexturePath = "rsc/imgs/Bullets/EnemyShot.png";

	return config;
}

inline EnemyConfig MakeFinalBoss2()
{
	EnemyConfig config = MakeStageBoss();
	config.hp = 1450;
	config.radius = 36.0f;
	config.moveSpeed = 32.0f;
	config.bossType = 4;
	config.animationPaths = { "rsc/imgs/Enemies/Boss4/0.png" };
	config.bulletTexturePath = "rsc/imgs/Bullets/BoneShot.png";
	return config;
}

inline EnemyConfig MakeFinalBoss3()
{
	EnemyConfig config = MakeStageBoss2();
	config.hp = 1650;
	config.moveSpeed = 28.0f;
	config.bossType = 5;

	vector<string> paths{};
	for (int i = 1; i < 265; ++i)
		paths.push_back("rsc/imgs/Enemies/Boss5/(" + std::to_string(i) + ").png");
	config.animationPaths = paths;

	config.bulletTexturePath = "rsc/imgs/Bullets/EnemyShot.png";

	return config;
}

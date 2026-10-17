#pragma once
#include "Enemy.h"
#include "Randomizer.h"
#include <cmath>

// 적 팩토리에서 spritePath는 단일 이미지, animationPaths는 반복 애니메이션 프레임
// 예: config.animationPaths = { "rsc/imgs/Enemies/Shotgun/0.png", "rsc/imgs/Enemies/Shotgun/1.png" };
// 애니메이션 속도는 config.animationFrameDuration로 조정합니다.


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

// 스테이지가 오를 때마다 적 최대 HP를 12%씩 올립니다.
inline EnemyConfig ScaleEnemyForStage(EnemyConfig config, int stage)
{
	if (stage < 1) stage = 1;
	config.hp = static_cast<int>(std::ceil(config.hp * (1.0f + 0.12f * (stage - 1))));
	return config;
}

inline BossPatternData GetStageBossPatternData(int phase)
{
	Randomizer _rander;

	switch (phase)
	{
	case 1: return { "SPELL 1", 12.0f, E_EnemyShotPattern::AIMED_SPREAD, 5, .65f, 0.9f, 230.0f, .65f };
	case 2:return { "SPELL 2", 12.0f, E_EnemyShotPattern::ROTATING_BURST, 18, .65f, .5f, 150.0f, .65f };
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

	vector<string> paths{};
	for (int i = 0; i < 50; ++i)
		paths.push_back("rsc/imgs/Enemies/Boss/(" + std::to_string(i) + ").png");


	config.animationPaths = paths;

	return config;
}

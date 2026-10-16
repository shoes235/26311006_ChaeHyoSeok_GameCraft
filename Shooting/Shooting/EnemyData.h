#pragma once
#include "Enemy.h"


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
	return config;
}

inline EnemyConfig MakeRapidStraightEnemy()
{
	EnemyConfig config;
	config.hp = 2;
	config.moveSpeed = 115.0f;
	config.stopY = 190.0f;
	config.initialFireDelay = 0.3f;
	config.fireInterval = 0.16f;
	config.bulletSpeed = 390.0f;
	config.shotPattern = E_EnemyShotPattern::STRAIGHT_RAPID;
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
	return config;
}


//Bosses


inline EnemyConfig MakeStageBoss()
{
	EnemyConfig config;
	config.hp = 180;
	config.radius = 32.0f;
	config.moveSpeed = 45.0f;
	config.stopY = 100.0f;
	config.initialFireDelay = 1.0f;
	config.fireInterval = 0.65f;
	config.bulletSpeed = 210.0f;
	config.bulletRadius = 7.0f;
	config.bulletCount = 16;
	config.shotPattern = E_EnemyShotPattern::RADIAL_BURST;
	return config;
}

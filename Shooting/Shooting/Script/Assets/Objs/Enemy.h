#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include <string>
#include <vector>

enum class E_EnemyShotPattern
{
	AIMED_SINGLE,
	AIMED_SPREAD,
	STRAIGHT_RAPID,
	STRAIGHT_SPREAD,
	RADIAL_BURST,
	ROTATING_BURST
};

struct EnemyConfig
{
	int hp;
	float radius;
	float moveSpeed;
	float stopY;
	float initialFireDelay;
	float fireInterval;
	float bulletSpeed;
	float bulletRadius;
	int bulletCount;
	float spreadAngle;
	E_EnemyShotPattern shotPattern;
	bool cycleBossPatterns;
	int bossType;
	string spritePath;
	vector<std::string> animationPaths;
	float animationFrameDuration;
	string bulletTexturePath;
	float difficultyFireIntervalScale;
	float difficultyBulletSpeedScale;

	EnemyConfig()
		: hp(3), radius(18.0f), moveSpeed(80.0f), stopY(130.0f),
		initialFireDelay(0.8f), fireInterval(1.0f), bulletSpeed(260.0f),
		bulletRadius(5.0f), bulletCount(5), spreadAngle(0.5f),
		shotPattern(E_EnemyShotPattern::AIMED_SINGLE), cycleBossPatterns(false), bossType(0),
		spritePath("rsc/imgs/Enemies/RiceSpirit.png"),
		animationFrameDuration(0.1f),
		bulletTexturePath("rsc/imgs/Bullets/EnemyShot.png"),
		difficultyFireIntervalScale(1.0f), difficultyBulletSpeedScale(1.0f)
	{
	}
};

class Enemy : public GameObject
{
private:
	EnemyConfig _config;
	int _hp;
	int _maxHp;
	float _fireCooldown;
	float _bossPhaseTimer = 0.0f;
	int _bossPhase = 0;
	BulletPool _bullets;
	Texture _bulletTexture;
	void FirePattern(VEC2 playerPos);
	void AddBullet(float angle);
	void SetBossPattern();
	float _bossPatternAngle{};

public:
	Enemy(VEC2 pos, const EnemyConfig& config = EnemyConfig());
	void Reset(VEC2 pos, const EnemyConfig& config);
	bool UsesAssets(const EnemyConfig& config) const;
	void Update(float delta, VEC2 playerPos);
	void Render() override;
	void TakeDamage(int damage = 1);
	bool IsAlive() const { return _hp > 0; }
	int GetHP() const { return _hp; }
	int GetMaxHP() const { return _maxHp; }
	const char* GetBossPatternName() const;
	std::vector<Bullet*>& GetBullets() { return _bullets.GetBullets(); }
};

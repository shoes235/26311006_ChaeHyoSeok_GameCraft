#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include <vector>

class Player : public GameObject
{
private:
    //move
    float _speedCur;
    const float DEF_SPEED;
    const float LOW_SPEED;
    bool _isFocused;

    VEC2 _areaMin;
    VEC2 _areaMax;
    VEC2 _moveDirection;


    //hurt ¾ÆÆÄ

    float _grazeRadius;
    Collider _grazeCollider;

    bool _isInvincible;
    float _invincibleTimer;

    //power
    float _shotCooldown{};
    float _rapidFireTimer{};
    float _shotgunTimer{};
    int _powerLevel{};
    const int MAX_POWER{ 20 };
    float _powerDecayTimer{};
    float _grazePower{};
    const float POWER_DECAY_INTERVAL{120.0f};

    //Æø
    int _bombs{ 3 };
    const int MAX_BOMB{ 8 };

    BulletPool _shots;

    Texture _hurtBoxTex{"rsc/imgs/Player/HurtBox.png",_pos};
    Texture _shotTex{"rsc/imgs/Bullets/PlayerShot.png",_pos};

public:
    Player();
    virtual ~Player();
    void Update(float delta) override;
    void Render() override;
    void Move(VEC2 dir, float dt);
    void SetFocus(bool focused);
    void Shoot();

    bool UseBomb();
    void AddBomb() { ++_bombs; }

    void AddRapidFire(float duration = 8.0f) { _rapidFireTimer = duration; }
    void AddShotgun(float duration = 10.0f) { _shotgunTimer = duration; }
    void AddPower();
    
    int GetMaxBomb() const { return MAX_BOMB; }
    int GetBombs() const { return _bombs; }
    int GetShotDamage() const { return 1 + _powerLevel; }
    int GetPowerLevel() const { return _powerLevel; }
    int GetMaxPower() const { return MAX_POWER; }
   
    float GetRapidFireTime() const { return _rapidFireTimer; }
    float GetShotgunTime() const { return _shotgunTimer; }
    float GetPowerDecayTime() const { return _powerDecayTimer; }
    float GetGrazePower() const { return _grazePower; }
    
    bool OnHit();
    void OnGraze();
   
    float GetCoreRadius() const { return _collider.Radius; }
    float GetGrazeRadius() const { return _grazeRadius; }
    const Collider& GetGrazeCollider() const { return _grazeCollider; }
    
    VEC2 GetCorePos() const { return VEC2(_pos.x + 28.0f, _pos.y + 28.0f); }
    std::vector<Bullet>& GetShots() { return _shots.GetBullets(); }
};

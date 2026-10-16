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
    float _hurtBoxRadius;
    float _grazeRadius;

    bool _isInvincible;
    float _invincibleTimer;

    //power
    float _shotCooldown = 0.0f;
    float _rapidFireTimer = 0.0f;
    float _powerTimer = 0.0f;

    //Æø
    int _bombs = 3;
    const int MAX_BOMB = 10;

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
    void AddPower(float duration = 8.0f) { _powerTimer = duration; }
    
    int GetMaxBomb() const { return MAX_BOMB; }
    int GetBombs() const { return _bombs; }
    int GetShotDamage() const { return _powerTimer > 0.0f ? 2 : 1; }
   
    float GetRapidFireTime() const { return _rapidFireTimer; }
    float GetPowerTime() const { return _powerTimer; }
    
    bool OnHit();
    void OnGraze();
   
    float GetCoreRadius() const { return _hurtBoxRadius; }
    float GetGrazeRadius() const { return _grazeRadius; }
    
    VEC2 GetCorePos() const { return VEC2(_pos.x + 28.0f, _pos.y + 28.0f); }
    std::vector<Bullet>& GetShots() { return _shots.GetBullets(); }
};

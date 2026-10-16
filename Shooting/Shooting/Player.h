#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include <vector>

class Player : public GameObject
{
private:
    float _speedCur;
    const float DEF_SPEED;
    const float LOW_SPEED;
    bool _isFocused;
    VEC2 _areaMin;
    VEC2 _areaMax;
    VEC2 _moveDirection;
    float _hurtBoxRadius;
    float _grazeRadius;
    bool _isInvincible;
    float _invincibleTimer;
    float _shotCooldown = 0.0f;
    std::vector<Bullet> _shots;

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
    void UseBomb();
    void OnHit();
    void OnGraze();
    float GetCoreRadius() const { return _hurtBoxRadius; }
    float GetGrazeRadius() const { return _grazeRadius; }
};

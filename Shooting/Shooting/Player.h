#pragma once
#include "GameObject.h"

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

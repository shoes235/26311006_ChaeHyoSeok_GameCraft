#include "Particle.h"

Particle::Particle(VEC2 pos, const std::vector<std::string>& frames,
	float frameDuration, float scale)
	: GameObject(pos), _lifeTime(frameDuration * frames.size()), _maxLifeTime(_lifeTime)
{
	auto clip = std::make_unique<AnimationClip>("Particle", frameDuration, false);

	for (const std::string& path : frames)
	{
		auto texture = std::make_unique<Texture>(path, pos);
		texture->SetScale(VEC2(scale, scale));
		clip->AddTexture(std::move(texture));
	}
	_anim->AddClip("Particle", std::move(clip));
	_anim->ChangeClip("Particle");
}

void Particle::Update(float delta)
{
	GameObject::Update(delta);
	_lifeTime -= delta;
}

void Particle::Reset(VEC2 pos)
{
	_pos = pos;
	_lifeTime = _maxLifeTime;
	_anim->Restart();
}

#include "Particle.h"

Particle::Particle(VEC2 pos, const std::vector<std::string>& frames,
	float frameDuration)
	: GameObject(pos), _lifeTime(frameDuration * frames.size())
{
	auto clip = std::make_unique<AnimationClip>("Particle", frameDuration, false);

	for (const std::string& path : frames)
	{
		auto texture = std::make_unique<Texture>(path, pos);
		texture->Load();
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

#pragma once

#include "GameObject.h"
#include "Randomizer.h"
#include <vector>
#include <string>
#include <unordered_map>

enum class MonitorScreenType
{
	HIT,
	IDLE,
	BOSS_CLEAR,
	USE_BOM,
	ITEM_GET
};

class MonitorScreen : public GameObject
{
private:
	static const char* GetClipName(MonitorScreenType type);
	void AddClip(const std::string& name, const std::vector<std::string>& frames,
		float frameDuration, bool loop);
	std::vector<std::string> _idleClips;
	std::unordered_map<std::string, float> _clipDurations;
	Randomizer _randomizer;
	float _timeRemaining = 0.0f;

public:
	using GameObject::GameObject;

	void LoadAnimations();
	void AddAnimation(MonitorScreenType type,
		const std::vector<std::string>& frames,
		float frameDuration = 0.1f,
		bool loop = false);
	void Play(MonitorScreenType type);
	void Update(float delta) override;
};

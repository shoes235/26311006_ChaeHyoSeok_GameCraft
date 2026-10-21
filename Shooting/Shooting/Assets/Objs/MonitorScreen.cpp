#include "MonitorScreen.h"
#include "AnimationClip.h"
#include <io.h>

const char* MonitorScreen::GetClipName(MonitorScreenType type)
{
	switch (type)
	{
	case MonitorScreenType::HIT: return "Hit";
	case MonitorScreenType::IDLE: return "Idle";
	case MonitorScreenType::BOSS_CLEAR: return "BossClear";
	case MonitorScreenType::USE_BOM: return "UseBom";
	case MonitorScreenType::ITEM_GET: return "ItemGet";
	default: return "Idle";
	}
}

void MonitorScreen::AddAnimation(MonitorScreenType type,
	const std::vector<std::string>& frames,
	float frameDuration,
	bool loop)
{
	const std::string name = type == MonitorScreenType::IDLE ? "Idle_0" : GetClipName(type);
	AddClip(name, frames, frameDuration, loop);
	if (type == MonitorScreenType::IDLE && !frames.empty() && _idleClips.empty())
		_idleClips.push_back(name);
}

void MonitorScreen::AddClip(const std::string& name,
	const std::vector<std::string>& frames, float frameDuration, bool loop)
{
	if (frames.empty()) return;

	auto clip = std::make_unique<AnimationClip>(name, frameDuration, loop);
	for (const std::string& path : frames)
	{
		auto texture = std::make_unique<Texture>(path, _pos);
		texture->Load();
		clip->AddTexture(std::move(texture));
	}
	_anim->AddClip(name, std::move(clip));
	_clipDurations[name] = frameDuration * frames.size();
}

void MonitorScreen::LoadAnimations()
{
	_idleClips.clear();
	auto loadFolder = [this](const std::string& folder, const std::string& clipName,
		float frameDuration, bool loop)
	{
		std::vector<std::string> frames;
		for (int frame = 1;; ++frame)
		{
			const std::string path = "rsc/imgs/UI/Screen/" + folder + "/ (" +
				std::to_string(frame) + ").png";
			if (_access(path.c_str(), 0) != 0) break;
			frames.push_back(path);
		}
		AddClip(clipName, frames, frameDuration, loop);
		if (clipName.find("Idle_") == 0 && !frames.empty())
			_idleClips.push_back(clipName);
	};

	loadFolder("Hit", "Hit", 0.08f, false);
	loadFolder("Idle", "Idle_0", 0.1f, true);
	loadFolder("Idle2", "Idle_1", 0.1f, true);
	loadFolder("Death", "BossClear", 0.08f, false);
	loadFolder("UseBom", "UseBom", 0.08f, false);
	loadFolder("ItemGet", "ItemGet", 0.08f, false);
	Play(MonitorScreenType::IDLE);
}

void MonitorScreen::Play(MonitorScreenType type)
{
	std::string clipName = GetClipName(type);
	if (type == MonitorScreenType::IDLE && !_idleClips.empty())
		clipName = _idleClips[_randomizer.Rand(0, static_cast<int>(_idleClips.size()) - 1)];

	_anim->ChangeClip(clipName);
	_anim->Restart();
	_timeRemaining = type == MonitorScreenType::IDLE ? 0.0f : _clipDurations[clipName];
}

void MonitorScreen::Update(float delta)
{
	GameObject::Update(delta);
	if (_timeRemaining <= 0.0f) return;
	_timeRemaining -= delta;
	if (_timeRemaining <= 0.0f) Play(MonitorScreenType::IDLE);
}

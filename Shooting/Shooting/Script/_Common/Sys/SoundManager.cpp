#include "SoundManager.h"

SoundManager g_SoundManager;

SoundManager::~SoundManager()
{
	Clear();
}

void SoundManager::Clear()
{
	StopBGM();
	for (auto& pair : _SFXDict)
		g2_SoundRelease(pair.second);
	_SFXDict.clear();
	for (auto& pair : _SFXPools)
		for (int sound : pair.second)
			g2_SoundRelease(sound);
	_SFXPools.clear();
	_SFXNext.clear();

	for (auto& pair : _BGMDict)
		g2_SoundRelease(pair.second);
	_BGMDict.clear();
}





bool SoundManager::AddSFX(const string& key, const string& path, std::size_t count)
{
	if (_SFXDict.count(key) || _SFXPools.count(key) || count == 0)
		return false;

	if (count == 1)
	{
		int sound = g2_SoundLoad(path.c_str());
		if (sound == 0)
			return false;
		_SFXDict[key] = sound;
		return true;
	}

	auto& sounds = _SFXPools[key];
	for (std::size_t i = 0; i < count; ++i)
	{
		int sound = g2_SoundLoad(path.c_str());
		if (sound == 0)
		{
			for (int loaded : sounds) g2_SoundRelease(loaded);
			_SFXPools.erase(key);
			return false;
		}
		sounds.push_back(sound);
	}
	_SFXNext[key] = 0;
	return true;
}

void SoundManager::PlaySFX(const string& key)
{
	auto i = _SFXDict.find(key);
	if (i != _SFXDict.end())
	{
		g2_SoundPlay(i->second);
		return;
	}
	auto pool = _SFXPools.find(key);
	if (pool == _SFXPools.end() || pool->second.empty()) return;

	auto& next = _SFXNext[key];
	g2_SoundReset(pool->second[next]);
	g2_SoundPlay(pool->second[next]);
	next = (next + 1) % pool->second.size();
}

void SoundManager::StopSFX(const string& key)
{
	auto i = _SFXDict.find(key);
	if (i != _SFXDict.end())
	{
		g2_SoundStop(i->second);
		g2_SoundReset(i->second);
	}
	auto pool = _SFXPools.find(key);
	if (pool != _SFXPools.end()) for (int sound : pool->second) {
		g2_SoundStop(sound);
		g2_SoundReset(sound);
	}
}

void SoundManager::RemoveSFX(const string& key)
{
	auto i = _SFXDict.find(key);
	if (i != _SFXDict.end()) {
		g2_SoundRelease(i->second);
		_SFXDict.erase(i);
	}
	auto pool = _SFXPools.find(key);
	if (pool != _SFXPools.end()) {
		for (int sound : pool->second) g2_SoundRelease(sound);
		_SFXPools.erase(pool);
	}
	_SFXNext.erase(key);
}





bool SoundManager::AddBGM(const string& key, const string& path)
{
	if (_BGMDict.count(key))
		return false;

	int sound = g2_SoundLoad(path.c_str());
	if (sound == 0)
		return false;
	_BGMDict[key] = sound;
	return true;
}

void SoundManager::PlayBGM(const string& key,bool loop)
{
	auto i = _BGMDict.find(key);
	if (i == _BGMDict.end())
		return;

	if (_curBGM != 0) {
		g2_SoundStop(_curBGM);
		g2_SoundReset(_curBGM);
	}
	_curBGM = i->second;

	g2_SoundReset(_curBGM);
	g2_SoundPlay(_curBGM,loop);
}

bool SoundManager::IsBGMPlaying()
{
	if (_curBGM == 0) return false;
	return g2_SoundIsPlaying(_curBGM);
}


void SoundManager::StopBGM()
{
	if (_curBGM == 0)
		return;

	g2_SoundStop(_curBGM);
	g2_SoundReset(_curBGM);
	_curBGM = 0;
}

void SoundManager::StopAll()
{
	StopBGM();
	for (const auto& pair : _SFXDict)
	{
		g2_SoundStop(pair.second);
		g2_SoundReset(pair.second);
	}
	for (const auto& pair : _SFXPools)
		for (int sound : pair.second) {
			g2_SoundStop(sound);
			g2_SoundReset(sound);
		}
}

void SoundManager::RemoveBGM(const string& key)
{
	auto i = _BGMDict.find(key);
	if (i == _BGMDict.end())
		return;

	if (_curBGM == i->second) {
		g2_SoundStop(_curBGM);
		g2_SoundReset(_curBGM);
		_curBGM = 0;
	}
	g2_SoundRelease(i->second);
	_BGMDict.erase(i);
}

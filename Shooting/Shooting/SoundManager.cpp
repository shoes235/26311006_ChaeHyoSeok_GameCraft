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

	for (auto& pair : _BGMDict)
		g2_SoundRelease(pair.second);
	_BGMDict.clear();
}





bool SoundManager::AddSFX(const string& key, const string& path)
{
	if (_SFXDict.count(key))
		return false;

	int sound = g2_SoundLoad(path.c_str());
	if (sound == 0)
		return false;
	_SFXDict[key] = sound;
	return true;
}

void SoundManager::PlaySFX(const string& key)
{
	auto i = _SFXDict.find(key);
	if (i == _SFXDict.end())
		return;

	//g2_SoundReset(i->second);
	g2_SoundPlay(i->second);
}

void SoundManager::StopSFX(const string& key)
{
	auto i = _SFXDict.find(key);
	if (i == _SFXDict.end())
		return;

	g2_SoundStop(i->second);
	g2_SoundReset(i->second);
}

void SoundManager::RemoveSFX(const string& key)
{
	auto i = _SFXDict.find(key);
	if (i == _SFXDict.end())
		return;

	g2_SoundRelease(i->second);
	_SFXDict.erase(i);
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
	for (const auto& pair : _SFXDict) {
		g2_SoundStop(pair.second);
		g2_SoundReset(pair.second);
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

#pragma once

#include <glc2d.h>
#include <string>
#include <map>

using std::string;

class SoundManager
{
private:
    std::map<string, int> _SFXDict;
    std::map<string, int> _BGMDict;
    int _curBGM{};

public:
    SoundManager() = default;
    ~SoundManager();
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    bool AddSFX(const string& key, const string& path);
    void PlaySFX(const string& key);
    void StopSFX(const string& key);
    void RemoveSFX(const string& key);

    bool AddBGM(const string& key, const string& path);
    void PlayBGM(const string& key, bool loop);
    bool IsBGMPlaying();
    void StopBGM();
    void StopAll();
    void RemoveBGM(const string& key);
    void Clear();
};

extern SoundManager g_SoundManager;

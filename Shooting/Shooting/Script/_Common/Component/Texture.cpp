#include "Texture.h"
#include <algorithm>
#include <cctype>
#include <unordered_map>

namespace
{
    struct CachedTexture { int handle; unsigned int references; };
    std::unordered_map<std::string, CachedTexture>& TextureCache()
    {
        static std::unordered_map<std::string, CachedTexture> cache;
        return cache;
    }
    std::string CacheKey(std::string path)
    {
        std::transform(path.begin(), path.end(), path.begin(), [](unsigned char ch) {
            return ch == '/' ? '\\' : static_cast<char>(std::tolower(ch));
        });
        return path;
    }
}

int Texture::Load()
{
    if (_texture != -1) return 0;
    auto& cache = TextureCache();
    const std::string key = CacheKey(_filePath);
    auto it = cache.find(key);
    if (it != cache.end())
    {
        _texture = it->second.handle;
        ++it->second.references;
    }
    else
    {
        _texture = g2_TextureLoad(_filePath.c_str());
        if (_texture != -1) cache.emplace(key, CachedTexture{ _texture, 1 });
    }

	printf("Texture Load : %s / handle = %d\n",
		_filePath.c_str(),
		_texture);

	return 0;
}

//Life-Cycle 에서 호출
int Texture::Print(const RECT* srcRect)
{
	VEC2 pos(_texturePos);
	VEC2 scale(_scale);
	g2_DrawAlphaOption(2);
	g2_Draw2D(_texture, srcRect, &pos, &scale);
	g2_DrawAlphaOption(0);

	return 0;
}

int Texture::PrintHorizontal(float progress)
{
	if (progress <= 0.0f) return 0;
	if (progress > 1.0f) progress = 1.0f;

	const int width = g2_TextureWidth(_texture);
	const int height = g2_TextureHeight(_texture);
	RECT srcRect{ 0, 0, static_cast<LONG>(width * progress), height };
	return Print(&srcRect);
}
int Texture::Release()
{
	if (_texture == -1) return 0;

    auto& cache = TextureCache();
    auto it = cache.find(CacheKey(_filePath));
    if (it != cache.end() && --it->second.references == 0)
    {
        g2_TextureRelease(it->second.handle);
        cache.erase(it);
    }
	_texture = -1;

	return 0;
}

int Texture::SetPos(VEC2 pos)
{
	_texturePos = pos;
	return 0;
}

int Texture::SetScale(VEC2 scale)
{
	_scale = scale;
	return 0;
}

VEC2 Texture::GetPos()
{
	return _texturePos;
}

std::string Texture::GetFilePath()
{
	return _filePath;
}
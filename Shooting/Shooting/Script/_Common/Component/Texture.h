#pragma once
#include <string>
#include <glc2d.h>
class Texture
{
protected :
	std::string _filePath;

	int _texture = -1;
	VEC2 _texturePos{0,0};
	VEC2 _scale{1.0f, 1.0f};

public :
	Texture(std::string path, VEC2 pos, VEC2 scale = VEC2(1.0f, 1.0f)) : _filePath(path), _texturePos(pos), _scale(scale)
	{
		printf("Texture CREATE : %p / %s\n",
			this,
			_filePath.c_str());
		Load();
	}
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture()
	{
		printf("Texture DELETE : %p\n", this);
		Release();
	}
	int Load();
	int Release();
	int Print(const RECT* srcRect = nullptr);
	int PrintHorizontal(float progress);

	int SetPos(VEC2 pos);
	int SetScale(VEC2 scale);

	VEC2 GetPos();

	std::string GetFilePath();
};


#include "Texture.h"

int Texture::Load()
{
	_texture = g2_TextureLoad(_filePath.c_str());

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

	g2_TextureRelease(_texture);
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
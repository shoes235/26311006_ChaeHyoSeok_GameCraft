#include "ProgressBar.h"

ProgressBar::ProgressBar(VEC2 pos)
	: GameObject(pos)
{
	string prefix = "rsc/imgs/ProgressBar/Bar_";

	auto clip = std::make_unique<AnimationClip>(
		"Progress", 0.1f, false);

	for(int i = 0; i < 7; i++)
	{
		auto tex = std::make_unique<Texture>
			(
				prefix + std::to_string(i) + ".png", pos
			);
		tex->Load();

		clip->AddTexture(std::move(tex));
	}
	_progressClip = clip.get();

	_anim->AddClip("Progress", std::move(clip));
	_anim->ChangeClip("Progress");
}


void ProgressBar::SetTime(float time)
{
	_manualProgress = false;
	int frameCount = _progressClip->GetFrameCount();

	if (frameCount <= 1)
		return;

	float frameDur = time / (frameCount - 1);

	_progressClip->SetFrameDuration(frameDur);

	_anim->Restart();
}

void ProgressBar::SetProgress(float progress)
{
	const int frameCount = _progressClip->GetFrameCount();
	if (frameCount <= 1) return;
	if (progress < 0.0f) progress = 0.0f;
	if (progress > 1.0f) progress = 1.0f;
	_frame = static_cast<int>((1.0f - progress) * (frameCount - 1) + 0.5f);
	_manualProgress = true;
}

void ProgressBar::Render()
{
	if (!_manualProgress)
	{
		GameObject::Render();
		return;
	}
	Texture* texture = _progressClip->GetTexture(_frame);
	if (texture)
	{
		texture->SetPos(_pos);
		texture->Print();
	}
}

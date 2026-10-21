#pragma once
#include "Particle.h"
#include "Randomizer.h"

class BomParticle : public Particle
{

private:
	static std::vector<std::string> GetFrames()
	{
		Randomizer rander;
		int r = rander.Rand(0, 1);

		std::vector<std::string> frames;
		if (r)
		{
			for (int i = 0; i < 7; i++)
				frames.push_back("rsc/imgs/Particle/Bom/" + std::to_string(i) + ".png");
		}
		else
		{
			for (int i = 7; i < 16; i++)
				frames.push_back("rsc/imgs/Particle/Bom/" + std::to_string(i) + ".png");
		}


		return frames;
	}


public:
	BomParticle(VEC2 pos, float scale = 1.0f)
		: Particle
		(
			pos,
			GetFrames(),
			.06f,
			scale
		)
	{ }
};

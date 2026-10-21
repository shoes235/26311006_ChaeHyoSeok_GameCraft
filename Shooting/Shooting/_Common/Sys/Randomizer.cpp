#include "Randomizer.h"
int Randomizer::Rand(int min, int max)
{
	if (min > max)
	{
		printf("INVALID RANDOM RANGE");
		return min;
	}

	_disI.param(std::uniform_int_distribution<int>::param_type(min, max));
	return _disI(_gen);
}

float Randomizer::Rand(float min, float max)
{
	if (min > max)
	{
		printf("INVALID RANDOM RANGE");
		return min;
	}

	_disF.param(std::uniform_real_distribution<float>::param_type(min, max));
	return _disF(_gen);
}
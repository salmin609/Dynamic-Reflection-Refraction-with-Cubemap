#pragma once

#include <ctime>
#include <random>
#include "Affine.h"

class RandomNumber
{
public:
	static RandomNumber* instance;

	RandomNumber();
	float RandomFloat(float min, float max);
	int RandomInt(int max);
	Vector3 RandomVector3(float min, float max);

private:
	std::random_device rd;
};
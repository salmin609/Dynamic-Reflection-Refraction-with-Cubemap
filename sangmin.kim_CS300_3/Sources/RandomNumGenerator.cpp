#include "RandomNumGenerator.h"
RandomNumber* RandomNumber::instance = nullptr;

RandomNumber::RandomNumber()
{
	if (instance != nullptr)
	{
		delete instance;
	}
	instance = this;
}

float RandomNumber::RandomFloat(float min, float max)
{
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<> distr(static_cast<double>(min), static_cast<double>(max));
	return static_cast<float>(distr(eng));
}

int RandomNumber::RandomInt(int max)
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, max);
	return dis(gen);
}

Vector3 RandomNumber::RandomVector3(float min, float max)
{
	return Vector3{ RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max) };
}
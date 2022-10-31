#pragma once
#include <random>
#include <limits>
#include "../VMath/Arithmetic.h"

class Random
{
public:
	Random(int seed = -1);
	~Random();

	template<Arithmetic T>
	T Next(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max());

	Vec2f NextVec2f(float min = 0, float max = 1)
	{
		float x = Next<float>(min, max);
		float y = Next<float>(min, max);
		return Vec2f(x, y);
	}

	Vec3f NextVec3f(float min = 0, float max = 1)
	{
		float x = Next<float>(min, max);
		float y = Next<float>(min, max);
		float z = Next<float>(min, max);
		return Vec3f(x, y, z);
	}

	Vec4f NextVec4f(bool w_is_one, float min = 0, float max = 1)
	{
		float x = Next<float>(min, max);
		float y = Next<float>(min, max);
		float z = Next<float>(min, max);
		float w = (w_is_one) ? 1 : Next<float>(min, max);
		return Vec4f(x, y, z, w);
	}
};

template<Arithmetic T>
inline T Random::Next(T min, T max)
{
	double val = ((double)std::rand()) / RAND_MAX;
	val = min + val * (max - min);
	return (T)val;
}

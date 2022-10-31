#pragma once
#include <math.h>
#include "VecXX.h"

namespace SIMath
{
	const float pi = 3.14159265359;

	inline float Clamp(float val, float min, float max)
	{
		return fmaxf(fminf(val, max), min);
	}

	inline float Rad2Deg(float rad)
	{
		return 360.0 * (rad / (2 * pi));
	}

	inline float Deg2Rad(float deg)
	{
		return (2 * pi) * (deg / 360.0);
	}

	inline float Dir2Angle(Vec2f dir)
	{
		dir = dir.normalized();
		return Rad2Deg(std::atan2f(dir.y, dir.x));
	}

	inline Vec2f Angle2Dir(float angle)
	{
		return Vec2f(std::cosf(Deg2Rad(angle)), std::sinf(Deg2Rad(angle)));
	}

	inline Vec3f Rotate2D(Vec3f v, float angle)
	{
		float rad = Deg2Rad(angle);

		return Vec3f(v.x * std::cosf(rad) + v.y * std::sinf(rad), 
					 v.x * std::sinf(rad) + v.y * std::cosf(rad),
					 v.z);
	}
}
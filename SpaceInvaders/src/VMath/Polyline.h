#pragma once
#include <vector>
#include "VecXX.h"

class Polyline
{
public:
	Polyline();
	Polyline(std::vector<Vec3f> points, bool closed);

	float Length() const
	{
		float length = 0;
		for (auto A = points.begin(), B = points.begin() + 1; B != points.end(); A++, B++)
		{
			length += Vec3f::Distance(*A, *B);
		}
		return length;
	};

	std::vector<Vec3f> points;
};


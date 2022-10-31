#pragma once
#include "../VMath/VecXX.h"

struct AABB
{
	Vec2f min;
	Vec2f max;

	inline bool Contains(Vec2f point)
	{
		return (min.x < point.x && min.y < point.y && point.x < max.x && point.y < max.y);
	}
};

inline bool AABBTest(AABB& a, AABB& b)
{
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

	return true;
}


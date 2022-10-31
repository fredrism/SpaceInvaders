#pragma once
#include "../../ECSCommon.h"
#include "../VMath/VecXX.h"

struct CollisionInfo
{
	Vec3f penetration;
	EntityID first;
	EntityID second;
};
#pragma once
#include <bitset>

const unsigned int MAX_ENTITIES = 2048;
const unsigned int MAX_COMPONENTS = 32;

typedef unsigned int EntityID;
typedef std::bitset<MAX_COMPONENTS> ComponentFlags;

inline bool MatchFlags(ComponentFlags a, ComponentFlags b)
{
	return ((a | b) == a);
}
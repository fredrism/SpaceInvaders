#include "pch.h"
#include "Random.h"

Random::Random(int seed)
{
    if (seed != -1)
    {
        std::srand(seed);
    }
}

Random::~Random()
{
}

#pragma once
#include "Components.h"
#include "../Core/Random/Random.h"


struct EnemySpawner
{
	float timer = 5;
	float maxTimer = 5;
};

class EnemySpawnerSystem : public ECSSystem
{
public:
	Random rng;

	virtual void Update(float dt);
};
#pragma once
#include "Components.h"
#include "Physics/PhysicsSystem.h"
#include "Transform.h"
#include "Projectile.h"
#include "../Core/Random/Random.h"


struct RandomStrafer
{
	Vec3f currentTarget;
	float speed;
	float shootTimer;
};

typedef EntityPrefab<RigidBody, Transform, Projectile, Team> EnemyProjectile;

class RandomStraferSystem : public ECSSystem
{
public:
	RandomStraferSystem()
	{

	}

	std::shared_ptr<EnemyProjectile> projectile;
	Random rng;

	virtual void OnCreate(EntityID entity)
	{
		auto& transform = m_ecs->GetComponent<Transform>(entity);
		auto& strafer = m_ecs->GetComponent<RandomStrafer>(entity);

		transform.position = Vec3f(rng.Next<float>(-1, 1), 2, 0);
		transform.rotation = 180;
		strafer.currentTarget = Vec3f(rng.Next<float>(-0.9, 0.9), rng.Next<float>(-0.9, 0.3), 0);
		strafer.speed = rng.Next<float>(1, 2);
		strafer.shootTimer = rng.Next<float>(1, 3);
	}

	virtual void OnDestroy(EntityID entity)
	{

	}

	virtual void OnMessage(ECSMessage& message)
	{

	}

	virtual void Update(float dt) override;
};
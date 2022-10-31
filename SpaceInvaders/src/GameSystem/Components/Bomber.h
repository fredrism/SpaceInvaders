#pragma once
#include "Components.h"
#include "Physics/PhysicsSystem.h"
#include "Transform.h"
#include "Projectile.h"
#include "../Core/Random/Random.h"
#include "PlayerController.h"


struct Bomber
{
	EntityID target;
	float timer = 2;
	float bombTimer = 0.1;
};

class BomberSystem : public ECSSystem
{
public:
	BomberSystem()
	{
		
	}

	Random rng;

	virtual void OnCreate(EntityID entity)
	{
		PlayerControllerSystem* pc = m_ecs->FindSystem<PlayerControllerSystem>();
		if (pc->GetEntities().size() == 0) return;

		auto& transform = m_ecs->GetComponent<Transform>(entity);
		auto& bomber = m_ecs->GetComponent<Bomber>(entity);

		transform.position = Vec3f(rng.Next<float>(-1, 1), 2, 0);
		transform.rotation = 180;
		
		int tgt = rng.Next<int>(0, 100) % pc->GetEntities().size();
		auto it = pc->GetEntities().begin();
		std::advance(it, tgt);
		bomber.target = *it;
	}

	virtual void OnDestroy(EntityID entity)
	{

	}

	virtual void OnMessage(ECSMessage& message)
	{

	}

	virtual void Update(float dt) override;
};
#pragma once
#include "Components.h"
#include "Transform.h"
#include "../Core/Random/Random.h"

struct BeachBall
{
	char tag;
};

class BeachBallSystem : public ECSSystem
{
public:
	BeachBallSystem()
	{

	}

	virtual void OnCreate(EntityID entity) override
	{
		auto& transform = m_ecs->GetComponent<Transform>(entity);
		auto& rb = m_ecs->GetComponent<RigidBody>(entity);

		transform.position = rng.NextVec2f();
		rb.velocity = rng.NextVec2f(-1, 1);
	}

	virtual void OnMessage(ECSMessage& message) override 
	{
		if (message.type == ECSMessageType::Collision)
		{
			auto info = (CollisionInfo*)message.data;
			//std::cout << "Collision Event: Entity : " << info->first << ", Entity : " << info->second << std::endl;
		}
	};

	Random rng;

	virtual void Update(float dt)
	{
		for (auto& entity : m_entities)
		{
			auto& transform = m_ecs->GetComponent<Transform>(entity);
			auto& rb = m_ecs->GetComponent<RigidBody>(entity);

			if (std::abs(transform.position.x) > 1)
			{
				transform.position.x *= 0.95;
				rb.velocity.x *= -1;

				rb.velocity += rng.NextVec2f(-0.05, 0.05);
			}
			if (std::abs(transform.position.y) > 1)
			{
				transform.position.y *= 0.95;
				rb.velocity.y *= -1;
				rb.velocity += rng.NextVec2f(-0.1, 0.1);
			}
		}
	}
};
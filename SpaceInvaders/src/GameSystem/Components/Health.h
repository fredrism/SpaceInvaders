#pragma once
#include "Components.h"
#include "Explosion.h"

struct Health
{
	int health;
};

struct Team
{
	std::bitset<8> enemy;
	int team;
};

class HealthSystem : public ECSSystem
{
	virtual void OnCreate(EntityID entity)
	{

	}

	virtual void OnDestroy(EntityID entity)
	{

	}

	virtual void OnMessage(ECSMessage& message)
	{

	}

	virtual void Update(float dt)
	{
		for (auto& entity : m_entities)
		{
			auto& health = m_ecs->GetComponent<Health>(entity);
			if (health.health <= 0)
			{
				ECSMessage message;
				message.entity = entity;
				message.type = ECSMessageType::Callback;
				message.callback = [&]() {
					Explosion ex = { 24, 1 };
					m_ecs->AddComponent<Explosion>(entity, ex);
				};

				m_ecs->SendMessage(message);
			}
		}
	}
};
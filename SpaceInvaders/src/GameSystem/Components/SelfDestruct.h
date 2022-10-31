#pragma once
#include "../ECSSystem.h"
#include "../ECS.h"

struct SelfDestruct
{
	float timer = 5;
};

class SelfDestructSystem : public ECSSystem
{
public:

	virtual void Update(float dt)
	{
		for (auto entity : m_entities)
		{
			auto& timer = m_ecs->GetComponent<SelfDestruct>(entity);
			if (timer.timer < 0)
			{
				ECSMessage msg;
				msg.entity = entity;
				msg.type = ECSMessageType::Destroyed;


				m_ecs->SendMessage(msg);
			}
			else
			{
				timer.timer -= dt;
			}
		}
	}
};
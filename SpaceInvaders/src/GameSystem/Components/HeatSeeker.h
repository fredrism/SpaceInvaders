#pragma once
#include "Components.h"
#include "Physics/PhysicsSystem.h"
#include "Health.h"

struct HeatSeeker
{
	float strength;
	Collider collider;
};

class HeatSeekerSystem : public ECSSystem
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
		auto* physics = m_ecs->FindSystem<PhysicsSystem>();
		if (physics == nullptr)
		{
			std::cout << "Failed to find physics system!" << std::endl;
		}

		for (auto e : m_entities)
		{
			auto& rb = m_ecs->GetComponent<RigidBody>(e);
			auto& hs = m_ecs->GetComponent<HeatSeeker>(e);
			auto& tr = m_ecs->GetComponent<Transform>(e);
			auto& team = m_ecs->GetComponent<Team>(e);

			std::vector<EntityID> entities = physics->CheckCollider(hs.collider, tr);
			//SystemManager::Gizmo()->DrawCircle(tr.position, hs.collider.radius, Vec3f(0, 0, 1));
			for (auto other : entities)
			{
				if (other == e) continue;
				Health* otherHealth;

				auto& otherRB = m_ecs->GetComponent<RigidBody>(other);

				if (m_ecs->QueryComponent<Health>(other, &otherHealth))
				{
					auto& otherTeam = m_ecs->GetComponent<Team>(other);

					if (team.team != otherTeam.team)
					{
						rb.force += (otherRB.position - rb.position) * hs.strength;
					}
				}
			}
		}
	}
};
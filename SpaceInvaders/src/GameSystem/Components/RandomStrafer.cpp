#include "pch.h"
#include "RandomStrafer.h"
#include "../Game/Game.h"

void RandomStraferSystem::Update(float dt)
{
	for (auto& entity : m_entities)
	{
		auto& transform = m_ecs->GetComponent<Transform>(entity);
		auto& strafer = m_ecs->GetComponent<RandomStrafer>(entity);

		if (Vec3f::Distance(transform.position, strafer.currentTarget) < 0.01)
		{
			strafer.currentTarget = Vec3f(rng.Next<float>(-0.9, 0.9), rng.Next<float>(0.9, 0.3), 0);
			strafer.speed = rng.Next<float>(1, 2);
		}
		else
		{
			transform.position = Vec3f::Lerp(transform.position, strafer.currentTarget, strafer.speed * dt);
		}

		if (strafer.shootTimer < 0)
		{
			strafer.shootTimer = rng.Next<float>(1, 3);

			ECSMessage msg;
			msg.type = ECSMessageType::Created;
			msg.callback = [&]() {
				auto& transform = m_ecs->GetComponent<Transform>(entity);
				auto& team = m_ecs->GetComponent<Team>(entity);
				RocketProjectile pr = Game::Get()->CreateRocket(transform.position + SIMath::Angle2Dir(transform.rotation + 90) * 0.2, Vec3f(0, -3, 0), team.team);
				EntityID proj = m_ecs->CreateEntity(pr);
			};

			m_ecs->SendMessage(msg);
		}
		strafer.shootTimer -= dt;
	}
}
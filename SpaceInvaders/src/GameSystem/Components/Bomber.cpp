#include "pch.h"
#include "Bomber.h"
#include "../Game/Game.h"

void BomberSystem::Update(float dt)
{
	PlayerControllerSystem* pc = m_ecs->FindSystem<PlayerControllerSystem>();

	for (auto& entity : m_entities)
	{
		auto& bomber = m_ecs->GetComponent<Bomber>(entity);
		auto& transform = m_ecs->GetComponent<Transform>(entity);

		if (pc->GetEntities().contains(bomber.target))
		{
			auto& otherTransform = m_ecs->GetComponent<Transform>(bomber.target);
			transform.position = Vec3f::Lerp(transform.position, otherTransform.position + Vec3f(0, 0.4, 0), 2 * dt);

			if (bomber.bombTimer < 0)
			{
				bomber.bombTimer = 0.1;

				ECSMessage msg;
				msg.type = ECSMessageType::Created;
				msg.callback = [&]() {
					auto& transform = m_ecs->GetComponent<Transform>(entity);
					auto& team = m_ecs->GetComponent<Team>(entity);
					BasicProjectile pr = Game::Get()->CreateBomb(transform.position + SIMath::Angle2Dir(transform.rotation + 90) * 0.17, team.team);
					EntityID proj = m_ecs->CreateEntity(pr);
				};

				m_ecs->SendMessage(msg);
			}

			if (bomber.timer < 0)
			{
				bomber.bombTimer -= dt;
			}
			if (bomber.timer < -1)
			{
				bomber.timer = 2;
			}
			bomber.timer -= dt;
		}
	}
}

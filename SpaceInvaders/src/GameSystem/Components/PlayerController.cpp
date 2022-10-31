#include "pch.h"
#include "PlayerController.h"
#include "../Game/Game.h"

void PlayerControllerSystem::Update(float dt)
{
	const int fireRate = 5;

	//SI_SCOPE_PROFILE("Sprite Update");
	for (auto& entity : m_entities)
	{
		auto& transform = m_ecs->GetComponent<Transform>(entity);
		auto& playerController = m_ecs->GetComponent<PlayerController>(entity);

		if (SystemManager::Input()->GetKey(playerController.leftKey))
		{
			playerController.velocity.x -= 10 * dt;
		}
		if (SystemManager::Input()->GetKey(playerController.rightKey))
		{
			playerController.velocity.x += 10 * dt;
		}
		if (SystemManager::Input()->GetKeyDown(playerController.sfKey))
		{
			ECSMessage msg;
			msg.type = ECSMessageType::Created;
			msg.callback = [&]() {
				auto& transform = m_ecs->GetComponent<Transform>(entity);
				auto& team = m_ecs->GetComponent<Team>(entity);
				RocketProjectile pr = Game::Get()->CreateRocket(transform.position + SIMath::Angle2Dir(transform.rotation + 90) * 0.17, Vec3f(0, 3, 0), team.team);
				EntityID proj = m_ecs->CreateEntity(pr);
			};

			m_ecs->SendMessage(msg);
		}
		if (SystemManager::Input()->GetKey(playerController.pfKey) && playerController.primaryFireTimer < 0)
		{
			ECSMessage msg;
			msg.type = ECSMessageType::Created;
			msg.callback = [&]() {
				auto& transform = m_ecs->GetComponent<Transform>(entity);
				auto& team = m_ecs->GetComponent<Team>(entity);
				BasicProjectile pr = Game::Get()->CreateProjectile(transform.position + SIMath::Angle2Dir(transform.rotation + 90)*0.17, Vec3f(0, 13, 0), team.team);
				EntityID proj = m_ecs->CreateEntity(pr);
			};

			playerController.primaryFireTimer = 1.0 / fireRate;

			m_ecs->SendMessage(msg);
		}

		transform.position += playerController.velocity * dt;

		playerController.primaryFireTimer -= dt;
		playerController.velocity *= 0.995;
	}
}

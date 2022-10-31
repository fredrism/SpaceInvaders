#include "pch.h"
#include "EnemySpawner.h"
#include "../Game/Game.h"

void EnemySpawnerSystem::Update(float dt)
{
	Random rng;
	for (auto& e : m_entities)
	{
		auto& spawner = m_ecs->GetComponent<EnemySpawner>(e);

		if (spawner.timer < 0)
		{
			ECSMessage msg;
			msg.type = ECSMessageType::Created;
			msg.callback = [&]() {
				if ((rng.Next<int>(0, 49) % 2) == 0)
				{
					EntityID proj = m_ecs->CreateEntity(*Game::Get()->strafeEnemy);
				}
				else
				{
					EntityID proj = m_ecs->CreateEntity(*Game::Get()->bomberEnemy);
				}
			};

			spawner.timer = spawner.maxTimer;
			spawner.maxTimer -= 0.1;
			m_ecs->SendMessage(msg);
		}

		spawner.timer -= dt;
	}
}

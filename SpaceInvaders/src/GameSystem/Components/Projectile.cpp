#include "pch.h"
#include "Projectile.h"
#include "../Game/Game.h"

void ProjectileSystem::OnCollision(EntityID first, EntityID second, Vec3f penetration)
{
	//std::cout << " ProjectileCollision : " << first << ", " << second << std::endl;
	auto& projectile = m_ecs->GetComponent<Projectile>(first);
	auto& team = m_ecs->GetComponent<Team>(first);

	Health* health;
	if (m_ecs->QueryComponent<Health>(second, &health))
	{
		auto& otherTeam = m_ecs->GetComponent<Team>(second);

		if (team.enemy.test(otherTeam.team))
		{
			health->health -= projectile.damage;

			ECSMessage msg;
			msg.type = ECSMessageType::Created;
			msg.callback = [=]() {
				EntityID sparks = m_ecs->CreateEntity(*Game::Get()->sparks);
				auto& transform = m_ecs->GetComponent<Transform>(first);
				auto& otherTransform = m_ecs->GetComponent<Transform>(second);

				auto& sparksTransform = m_ecs->GetComponent<Transform>(sparks);
				auto& particles = m_ecs->GetComponent<ParticleEmitter>(sparks);

				particles.color = Game::Get()->teamColors[team.team];
				sparksTransform.rotation = SIMath::Dir2Angle(otherTransform.position - transform.position) - 90;
				sparksTransform.position = transform.position;
			};

			ECSMessage dstmsg;
			dstmsg.entity = first;
			dstmsg.other = second;
			dstmsg.type = ECSMessageType::Destroyed;

			m_ecs->SendMessage(msg);
			m_ecs->SendMessage(dstmsg);
		}
	}
}

void ProjectileSystem::Update(float dt)
{
	for (auto& entity : m_entities)
	{
		auto& rb = m_ecs->GetComponent<RigidBody>(entity);
		auto& transform = m_ecs->GetComponent<Transform>(entity);

		transform.rotation = SIMath::Dir2Angle(rb.velocity) - 90;

	}
}
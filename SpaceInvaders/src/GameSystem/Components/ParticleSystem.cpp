#include "pch.h"
#include "ParticleSystem.h"
#include "../Game/Game.h"

void ParticleSystem::OnCreate(EntityID entity)
{
	Transform& transform = m_ecs->GetComponent<Transform>(entity);
	ParticleEmitter& emitter = m_ecs->GetComponent<ParticleEmitter>(entity);
	emitter.particles = std::make_shared<std::vector<Particle>>();

	for (unsigned int i = 0; i < emitter.numParticles; i++)
	{
		emitter.particles->push_back({ Vec3f(0,0,0), Vec3f(0,0,0), Vec4f(0,0,0,0), 0, 0, rng.Next<float>(0, emitter.lifetime), i });
		if (emitter.loop) emitter.initialized = true;
	}
}

void ParticleSystem::UpdateParticle(Particle& particle, Transform& transform, ParticleEmitter& emitter)
{
	float a = SIMath::Deg2Rad(transform.rotation - 90 + rng.Next<float>(-emitter.angle / 2, emitter.angle / 2));
	float s = rng.Next<float>(1, 3);

	particle.position = transform.position;
	particle.rotation = rng.Next<float>(0, 360);
	particle.color = emitter.color + rng.NextVec3f(-emitter.colorVariation, emitter.colorVariation);
	particle.size = rng.Next(0.01, 0.2);
	particle.velocity.x = std::cosf(a) * s;
	particle.velocity.y = std::sinf(a) * s;
	particle.lifetime = rng.Next<float>(emitter.lifetime / 2, emitter.lifetime);
}

void ParticleSystem::Update(float dt)
{
	for (auto& entity : m_entities)
	{
		ParticleEmitter& emitter = m_ecs->GetComponent<ParticleEmitter>(entity);
		Transform& transform = m_ecs->GetComponent<Transform>(entity);

		std::vector<Matrix4f> matrices;
		std::vector<Vec4f> colors;
		matrices.resize(emitter.numParticles);
		colors.resize(emitter.numParticles);

		std::atomic_bool alive = emitter.loop;

		std::for_each(std::execution::par, emitter.particles->begin(), emitter.particles->end(), [&](auto&& particle) {
			if ((particle.lifetime < 0 && emitter.loop) || !emitter.initialized)
			{
				UpdateParticle(particle, transform, emitter);
			}

			Vec3f rotation(0, 0, particle.rotation);
			Vec3f size(particle.size, particle.size, particle.size);
			matrices[particle.index] = { };
			matrices[particle.index].SetTRS(particle.position, rotation, size);
			colors[particle.index] = particle.color;

			particle.position += particle.velocity * dt;
			particle.lifetime -= dt;
			particle.color = Vec4f::Lerp(particle.color, Vec4f(0, 0, 0, 0), dt);

			alive = alive | (particle.lifetime > 0);
			});

		emitter.initialized = true;
		if (!alive)
		{
			ECSMessage message;
			message.entity = entity;
			message.type = ECSMessageType::Destroyed;
			m_ecs->SendMessage(message);
		}

		SystemManager::Graphics()->DrawInstanced(&rect->geometry, material.get(), &matrices[0], &colors[0], emitter.numParticles);
	}
}
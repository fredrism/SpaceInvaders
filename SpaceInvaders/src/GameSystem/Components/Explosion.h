#pragma once
#include "Components.h"
#include "Physics/PhysicsSystem.h"
#include "../Core/Random/Random.h"
#include "../Resources/TextureLoader.h"
#include "SelfDestruct.h"
#include "Trail.h"

struct Explosion
{
	int numDebris;
	float force;
};


class ExplosionSystem : public ECSSystem
{
public:
	ExplosionSystem()
	{
		textures.push_back(TextureLoader::LoadTexture("resources/sprites/shapes/tri_hollow.png"));
		textures.push_back(TextureLoader::LoadTexture("resources/sprites/shapes/tri_fill.png"));
		textures.push_back(TextureLoader::LoadTexture("resources/sprites/shapes/square_hollow.png"));
		textures.push_back(TextureLoader::LoadTexture("resources/sprites/shapes/square_fill.png"));
		textures.push_back(TextureLoader::LoadTexture("resources/sprites/shapes/circle_hollow.png"));
		textures.push_back(TextureLoader::LoadTexture("resources/sprites/shapes/circle_fill.png"));
	}

	Random rng;
	std::vector<std::shared_ptr<Texture2D>> textures;

	virtual void OnCreate(EntityID entity) override
	{
		auto& explosion = m_ecs->GetComponent<Explosion>(entity);

		for (int i = 0; i < explosion.numDebris; i++)
		{
			ECSMessage msg;
			msg.type = ECSMessageType::Created;
			msg.callback = [=]() {
				auto& transform = m_ecs->GetComponent<Transform>(entity);
				
				unsigned int texture = rng.Next<unsigned int>(0, 100) % textures.size();
				Vec4f color = rng.NextVec4f(false, 0.5, 10);
				Vec2f velocity = Vec2f(rng.Next<float>(-1, 1), rng.Next<float>(-0.2, 1.5));// rng.NextVec2f(-1, 1);

				EntityID debris = m_ecs->CreateEntity();
				RigidBody rb(rng.Next<float>(1, 10), 0.1, CircleCollider(0.05), 0.1, false, true);
				rb.velocity = velocity;
				m_ecs->AddComponent<RigidBody>(debris, rb);
				Sprite s = { textures[texture], color };
				m_ecs->AddComponent<Sprite>(debris, s);
				Transform t = { transform.position + rng.NextVec2f(-0.1,0.1), rng.Next<float>(0, 360), rng.NextVec3f(0.003, 0.01)};
				m_ecs->AddComponent<Transform>(debris, t);
				SelfDestruct sd = { rng.Next<float>(2, 10) };
				m_ecs->AddComponent<SelfDestruct>(debris, sd);

				if (i % 4 == 0)
				{
					Trail trail = { color, 0.01 };
					m_ecs->AddComponent<Trail>(debris, trail);
				}
			};

			m_ecs->SendMessage(msg);
		}

		ECSMessage msg;
		msg.type = ECSMessageType::Destroyed;
		msg.entity = entity;

		m_ecs->SendMessage(msg);
	}

	virtual void Update(float dt) {};
};
#pragma once
#include "Collider.h"
#include "../Transform.h"

#include <math.h>
#include <algorithm>
#include <execution>
#include <array>

const int GROUP_SIZE = 10;

struct RigidBody
{
	RigidBody(float _mass, float _drag, Collider _collider, float _restitution, bool _trigger, bool _gravity = false)
	{
		mass = _mass;
		drag = _drag;
		collider = _collider;
		restitution = _restitution;
		trigger = _trigger;
		gravity = _gravity;
	}

	RigidBody() = default;

	Vec2f position;
	float rotation;

	Vec2f velocity = { 0,0 };
	Vec2f force = { 0,0 };
	Vec2f acc = { 0,0 };
	float mass;
	Collider collider;
	float drag;
	float restitution = 0.1;
	bool gravity;

	bool trigger;
	EntityID owner = 0;
};

class PhysicsSystem : public ECSSystem
{
public:
	struct CollisionInfoWrapper
	{
		CollisionInfo info;
		RigidBody* a;
		RigidBody* b;
	};

	std::vector<CollisionInfoWrapper> collisions;
	std::mutex collisionListMutex;

	virtual void OnCreate(EntityID id) override
	{
		RigidBody& rb = m_ecs->GetComponent<RigidBody>(id);
		Transform& transform = m_ecs->GetComponent<Transform>(id);
		ComputeAABB(rb.collider, transform);
		rb.position = transform.position;
		rb.rotation = transform.rotation;
		rb.owner = id;
	}

	virtual void Update(float dt)
	{
		collisions.clear();

		//Integrator step
		std::for_each(std::execution::par, m_entities.begin(), m_entities.end(), [&](auto&& entity) {
			auto& rb = m_ecs->GetComponent<RigidBody>(entity);
			auto& transform = m_ecs->GetComponent<Transform>(entity);
			
			//DrawCollider(rb.collider, transform);
			if (rb.mass < 0)
			{
				rb.position = transform.position;
				return;
			}

			rb.position = transform.position + rb.velocity * dt + rb.acc * dt * dt;
			rb.rotation = transform.rotation;

			Vec2f newAccel = (rb.force - rb.velocity * rb.velocity.Magnitude() * rb.drag) / rb.mass;

			rb.velocity += (rb.acc + newAccel) * (dt * 0.5);
			rb.force = Vec2f(0, 0);
			rb.acc = newAccel;
			if (rb.gravity) rb.acc.y -= 9.81/4;
			});
		

		{
			//ScopeProfiler profile("PhysicsSystem", 65, "Update()");
			//Collision Detection
			std::for_each(std::execution::par, m_entities.begin(), m_entities.end(), [&](auto&& entity) {
				auto& rb = m_ecs->GetComponent<RigidBody>(entity);
				auto& transform = m_ecs->GetComponent<Transform>(entity);

				auto it = std::find(m_entities.begin(), m_entities.end(), entity);		//prevent testing the same collision twice (a, b) & (b, a)
				while (it != m_entities.end())
				{
					if (entity == *it) { it++; continue; }
					auto& otherRB = m_ecs->GetComponent<RigidBody>(*it);
					auto& otherTransform = m_ecs->GetComponent<Transform>(*it);

					CollisionInfoWrapper info;
					info.a = &rb;
					info.b = &otherRB;

					if (CollisionTest::Intersects(rb.collider, otherRB.collider, transform, otherTransform, info.info))
					{
						info.info.first = entity;
						info.info.second = *it;		

						{
							std::lock_guard lock(collisionListMutex);
							collisions.push_back(info);
		
							ECSMessage msg;
							msg.type = ECSMessageType::Collision;
							msg.data = &collisions.back().info;
							msg.mask = 0;

							m_ecs->SendMessage(msg);
						}
					}

					it++;
				}
				});
		}

		for (auto& collision : collisions)
		{
			ResolveCollision(collision, dt);
		}

		//Update transforms
		std::for_each(std::execution::par, m_entities.begin(), m_entities.end(), [&](auto&& entity) {
			auto& rb = m_ecs->GetComponent<RigidBody>(entity);
			auto& transform = m_ecs->GetComponent<Transform>(entity);
			ComputeAABB(rb.collider, transform);
			if (rb.mass < 0) return;
			transform.position = rb.position;
		});
	}

	std::vector<EntityID> CheckCollider(Collider& collider, Transform& transform)
	{
		std::vector<EntityID> result;
		ComputeAABB(collider, transform);
		std::for_each(std::execution::par, m_entities.begin(), m_entities.end(), [&](auto&& entity) {

			auto& otherRB = m_ecs->GetComponent<RigidBody>(entity);
			auto& otherTransform = m_ecs->GetComponent<Transform>(entity);
			
			CollisionInfoWrapper info;

			if (CollisionTest::Intersects(collider, otherRB.collider, transform, otherTransform, info.info))
			{
				std::lock_guard lock(collisionListMutex);
				result.push_back(entity);
			}
			});

		return result;
	}

	void ResolveCollision(CollisionInfoWrapper& info, float dt)
	{
		if (info.a->trigger || info.b->trigger) return;

		Vec2f relativeVelocity = info.b->velocity - info.a->velocity;
		float projVelocity = Vec2f::Dot(relativeVelocity, info.info.penetration.normalized());

		float ma = info.a->mass;
		float mb = info.b->mass;

		if (ma < 0) ma = 999999999;
		if (mb < 0) mb = 999999999;


		if (projVelocity > 0) return;

		float restitution = std::fminf(info.a->restitution, info.b->restitution);
		float j = -(1 + restitution) * projVelocity;
		j /= (1 / ma) + (1 / mb);

		Vec2f impulse = info.info.penetration * j;
		info.a->velocity -= impulse * (1 / ma);
		info.b->velocity += impulse * (1 / mb);
		
		Vec2f correction = (info.info.penetration*0.8) / ((1 / ma) + (1 / mb) * 0.2);
		info.a->position -=  correction * (1 / ma);
		info.b->position += correction * (1 / mb);
	}
	

	void DrawCollider(Collider& collider, Transform& transform)				//Slow for parallel code!
	{
		switch (collider.shape)
		{
		case ColliderShape::Polygon:
			for (unsigned int i = 0; i < collider.polygon.size() - 1; i++)
			{
				Vec3f A = collider.polygon[i];
				A = transform.matrix.MulPoint(A);

				Vec3f B = collider.polygon[i + 1];
				B = transform.matrix.MulPoint(B);

				SystemManager::Gizmo()->DrawLine(A, B, Vec3f(0, 1, 0));
			}
			break;
		case ColliderShape::Circle:
			SystemManager::Gizmo()->DrawCircle(transform.position, collider.radius, Vec3f(0, 1, 0));
			break;
		default:
			break;
		}

		SystemManager::Gizmo()->DrawBox(collider.boundingBox.min, collider.boundingBox.max, Vec3f(1, 0, 0));
	}
};
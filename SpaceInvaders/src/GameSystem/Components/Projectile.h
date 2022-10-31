#pragma once
#include "Components.h"
#include "Physics/PhysicsSystem.h"
#include "Health.h"
#include "ParticleSystem.h"

struct Projectile
{
	int damage;
};

class ProjectileSystem : public ECSSystem
{
	virtual void OnCollision(EntityID first, EntityID second, Vec3f penetration) override;
	virtual void Update(float dt)  override;
};
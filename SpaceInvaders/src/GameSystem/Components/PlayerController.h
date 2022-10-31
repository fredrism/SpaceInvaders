#pragma once
#include "Components.h"
#include "Transform.h"
#include "SelfDestruct.h"
#include "Projectile.h"
#include "../Prefab/EntityPrefab.h"
#include "../Resources/TextureLoader.h"
#include "../Resources/ShaderLoader.h"
#include "ParticleSystem.h"
#include "HeatSeeker.h"
#include "Trail.h"

struct PlayerController
{
	Vec3f velocity = {0,0,0};
	float primaryFireTimer;
	
	unsigned int leftKey;
	unsigned int rightKey;
	unsigned int pfKey;
	unsigned int sfKey;
};

class PlayerControllerSystem : public ECSSystem
{
public:

	PlayerControllerSystem()
	{

	}

	virtual void OnDestroy(EntityID entity) override
	{
		SystemManager::UI()->SetPanel("defeat");
	}
		 
	virtual void Update(float dt) override;
};
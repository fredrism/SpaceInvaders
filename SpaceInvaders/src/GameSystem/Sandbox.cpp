#include "pch.h"
#include "Sandbox.h"
#include "Game/Game.h"

Sandbox::Sandbox()
{
	ecs.RegisterECSSystem<TransformSystem, Transform>();
	ecs.RegisterECSSystem<SpriteSystem, Transform, Sprite>();
	ecs.RegisterECSSystem<PlayerControllerSystem, Transform, PlayerController>();
	ecs.RegisterECSSystem<PhysicsSystem, Transform, RigidBody>();
	ecs.RegisterECSSystem<SelfDestructSystem, SelfDestruct>();
	ecs.RegisterECSSystem<BeachBallSystem, Transform, RigidBody, BeachBall>();
	ecs.RegisterECSSystem<HealthSystem, Health, Team>();
	ecs.RegisterECSSystem<ProjectileSystem, Projectile, Team>();
	ecs.RegisterECSSystem<ParticleSystem, ParticleEmitter, Transform>();
	ecs.RegisterECSSystem<TextSystem, TextSprite, Transform>();
	ecs.RegisterECSSystem<HeatSeekerSystem, HeatSeeker, RigidBody, Team, Transform>();
	ecs.RegisterECSSystem<TrailSystem, Trail, Transform>();
	ecs.RegisterECSSystem<RandomStraferSystem, RandomStrafer, Transform>();
	ecs.RegisterECSSystem<EnemySpawnerSystem, EnemySpawner>();
	ecs.RegisterECSSystem<ExplosionSystem, Explosion>();
	ecs.RegisterECSSystem<BomberSystem, Bomber, Transform, RigidBody>();

	m_game = std::make_unique<Game>(&ecs);

	SystemManager::UI()->SetPanel("mainMenu");

	ecs.CreateEntity(*m_game->beachBall);
}

void Sandbox::Update(float dt)
{
	if (SystemManager::Input()->GetKeyDown(SDLK_ESCAPE))
	{
		m_game->paused = true;
		SystemManager::UI()->SetPanel("pauseMenu");
	}

	if (!m_game->paused)
	{
		SystemManager::Graphics()->Clear();
		ecs.Update(dt);
	}
}

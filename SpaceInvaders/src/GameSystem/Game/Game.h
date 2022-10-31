#pragma once
#include "../Resources/TextureLoader.h"
#include "../Resources/ShaderLoader.h"
#include "../Core/Random/Random.h"
#include "../Prefab/EntityPrefab.h"
#include "../Components/ALL_COMPONENTS.h"
#include "../ECS.h"
#include "../Resources/ConfigLoader.h"


typedef EntityPrefab<Transform, RigidBody, Sprite, BeachBall, Health, Team> BallPrefab;

typedef EntityPrefab<Transform, Sprite, PlayerController, RigidBody, Health, Team> PlayerShip;
typedef EntityPrefab<Transform, Sprite, RandomStrafer, RigidBody, Health, Team> StrafeEnemy;
typedef EntityPrefab<Transform, Sprite, Bomber, RigidBody, Health, Team> BomberEnemy;

typedef EntityPrefab<Transform, RigidBody, Sprite, SelfDestruct, Projectile, Team> BasicProjectile;
typedef EntityPrefab<Transform, RigidBody, Sprite, SelfDestruct, Projectile, Team, HeatSeeker, Trail> RocketProjectile;

typedef EntityPrefab<Transform, ParticleEmitter> Sparks;

class Game
{
public:
	Game(ECS* ecs);

	void CreateUI(ECS* ecs);

	void Start(ECS* ecs)
	{
		SystemManager::Graphics()->Clear();
		m_ecs->Clear();
		paused = false;
		SystemManager::UI()->SetPanel("");

		for (unsigned int i = 0; i < m_players; i++)
		{
			Vec3f position = Vec3f(-0.5 + 0.1 * i, -0.7 + 0.1 * i, -1);
			PlayerShip p = CreateTriangleShip(position, i);
			EntityID player = ecs->CreateEntity(p);
		}

		EntityID spawner = ecs->CreateEntity();
		EnemySpawner sp{0, 5};
		ecs->AddComponent<EnemySpawner>(spawner, sp);
	}

	BasicProjectile CreateProjectile(Vec3f position, Vec3f velocity, int team);
	RocketProjectile CreateRocket(Vec3f position, Vec3f velocity, int team);
	BasicProjectile CreateBomb(Vec3f position, int team);
	PlayerShip CreateTriangleShip(Vec3f position, int team);

	std::shared_ptr<PlayerShip> playerShip;
	std::shared_ptr<StrafeEnemy> strafeEnemy;
	std::shared_ptr<BomberEnemy> bomberEnemy;
	std::shared_ptr<BallPrefab> beachBall;
	std::shared_ptr<BasicProjectile> basicProjectile;
	std::shared_ptr<BasicProjectile> bombProjectile;
	std::shared_ptr<RocketProjectile> rocketProjectile;
	std::shared_ptr<Sparks> sparks;
	std::shared_ptr<EntityPrefab<Sprite, Transform>> background;

	static Game* Get() { return s_instance; };
	bool paused = false;
	std::vector<Vec4f> teamColors;
	std::vector <std::bitset<8>> enemies;

private:
	inline static Game* s_instance;
	ConfigLoader m_config;
	unsigned int m_players;
	
	ECS* m_ecs;
};
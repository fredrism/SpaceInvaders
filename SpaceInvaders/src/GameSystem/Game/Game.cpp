#include "pch.h"
#include "Game.h"

Game::Game(ECS* ecs) : m_config("resources/config.txt")
{
	s_instance = this;
	m_ecs = ecs;
	paused = false;
	CreateUI(ecs);

	auto unlitTextured = ShaderLoader::Load("resources/shaders/unlit_texture.glsl");
	std::shared_ptr<RectGeometry> rect = std::make_shared<RectGeometry>(Vec2f(0.1, 0.1));

	teamColors.push_back(Vec4f(1, 1, 10, 1));
	teamColors.push_back(Vec4f(1, 10, 1, 1));
	teamColors.push_back(Vec4f(10, 1, 1, 1));
	teamColors.push_back(Vec4f(10, 10, 1, 1));
	enemies.push_back(12);
	enemies.push_back(12);
	enemies.push_back(3);
	enemies.push_back(3);

	//______________________________________________________________________________________________________________________PLAYER SHIP
	
	auto playerTexture = TextureLoader::LoadTexture("resources/sprites/shapes/tri_hollow.png");
	auto bomberTexture = TextureLoader::LoadTexture("resources/sprites/shapes/square_hollow.png");
	
	std::array<Vec2f, 4> collider = { Vec2f(-1, -1), Vec2f(1, -1), Vec2f(0, 1), Vec2f(-1, -1) };
	RigidBody playerRB(-1, 0, PolygonCollider(collider), 0.1, false);
	playerShip = PlayerShip({ Vec3f(0,-0.7, -1), 0, Vec3f(0.1, 0.1, 0.1) }, { playerTexture, teamColors[0]}, {}, playerRB, {100}, {12, 0}).MakeShared();

	//______________________________________________________________________________________________________________________STRAFE ENEMY

	strafeEnemy = StrafeEnemy({ Vec3f(0, 0, -1), 0, Vec3f(0.1, 0.1, 0.1) }, { playerTexture, teamColors[2] }, {}, playerRB, { 10 }, { 3, 2 }).MakeShared();

	//______________________________________________________________________________________________________________________BOMBER ENEMY

	bomberEnemy = BomberEnemy({ Vec3f(0, 0, -1), 0, Vec3f(0.1, 0.1, 0.1) }, { bomberTexture, teamColors[3] }, {}, playerRB, { 10 }, { 3, 3 }).MakeShared();

	//______________________________________________________________________________________________________________________BEACH BALL

	
	auto ballTexture = TextureLoader::LoadTexture("resources/sprites/beach_ball.png");
	RigidBody rbb(1, 0.01, CircleCollider(0.1), 0.1, false);
	beachBall = BallPrefab({ Vec3f(0,0,-0.5), 0, Vec3f(0.2,0.2,0.2) }, rbb, { ballTexture }, {}, { 1000 }, { 1 }).MakeShared();
	

	//______________________________________________________________________________________________________________________BASIC PROJECTILE
	
	auto projectileTexture = TextureLoader::LoadTexture("resources/sprites/shapes/circle_fill.png");
	RigidBody projectileRB(0.1, 0.05, CircleCollider(0.04), 0.1, false);
	basicProjectile = BasicProjectile(
		{ Vec3f(0, 0, 0), 0, Vec3f(0.02, 0.02, 0.02) },
		projectileRB,
		{ projectileTexture },
		{ 5 },
		{ 1 },
		{ 0 }).MakeShared();

	auto bombtexture = TextureLoader::LoadTexture("resources/sprites/shapes/square_dash.png");
	bombProjectile = BasicProjectile(
		{ Vec3f(0, 0, 0), 0, Vec3f(0.02, 0.02, 0.02) },
		projectileRB,
		{ projectileTexture },
		{ 5 },
		{ 1 },
		{ 0 }).MakeShared();



	//______________________________________________________________________________________________________________________ROCKET PROJECTILE

	auto rocketTexture = TextureLoader::LoadTexture("resources/sprites/shapes/tri_fill.png");

	rocketProjectile = RocketProjectile(
		{ Vec3f(0, 0, 0), 0, Vec3f(0.05, 0.05, 0.05) },
		projectileRB,
		{ rocketTexture },
		{ 5 },
		{ 10 },
		{ 0 },
		{ 1, CircleCollider(0.7) },
		{ Vec4f(0.9,0.9,1, 1), 0.01 }).MakeShared();

	//______________________________________________________________________________________________________________________SPARKS
	ParticleEmitter sparksEmitter(75, Vec4f(0.9, 0.9, 0.6, 1), 0.1, false, 20, 1);
	sparks = Sparks({ { Vec3f(0, 0, 0), 0, Vec3f(1, 1, 1) }, sparksEmitter }).MakeShared();
	//______________________________________________________________________________________________________________________BACKDROP
	auto backgroundTexture = TextureLoader::LoadTexture("resources/sprites/background.png");
	background = EntityPrefab<Sprite, Transform>({ backgroundTexture, Vec4f(1,1,1,1) }, { Vec3f(0,0,0), 0, Vec3f(1,1,1) }).MakeShared();
}

void Game::CreateUI(ECS* ecs)
{
	auto mainMenu = std::make_shared<UIPanel>("mainMenu");
	mainMenu->AddButton(UIButton(Vec2f(-0.8, 0.5), Vec2f(0.1, 0.2), "singleplayer", Vec4f(1, 1, 1, 1), [&]()
		{
			m_players = 1;
			Start(m_ecs);
		}));

	mainMenu->AddButton(UIButton(Vec2f(-0.8, 0.25), Vec2f(0.1, 0.2), "multiplayer", Vec4f(1, 1, 1, 1), [&]()
		{
			m_players = 2;
			Start(m_ecs);
		}));

	mainMenu->AddButton(UIButton(Vec2f(-0.8, 0), Vec2f(0.1, 0.2), "quit", Vec4f(1, 1, 1, 1), []() { SystemManager::Quit() = true; }));
	mainMenu->AddLabel(UILabel(Vec2f(0, -0.5), Vec2f(0.1, 0.2), "cpp exam", Vec4f(1, 1, 0, 1)));
	mainMenu->AddLabel(UILabel(Vec2f(-0.8, 0.8), Vec2f(0.07, 0.1), "space invaders from outer space", Vec4f(1, 1, 0, 1)));




	auto pauseMenu = std::make_shared<UIPanel>("pauseMenu");
	pauseMenu->AddButton(UIButton(Vec2f(-0.8, 0.5), Vec2f(0.1, 0.2), "resume", Vec4f(1, 1, 1, 1), [&]()
		{
			SystemManager::UI()->SetPanel("");
			paused = false;
		}));

	pauseMenu->AddButton(UIButton(Vec2f(-0.8, 0), Vec2f(0.1, 0.2), "main menu", Vec4f(1, 1, 1, 1), []() { SystemManager::UI()->SetPanel("mainMenu"); }));



	auto defeatMenu = std::make_shared<UIPanel>("defeat");
	defeatMenu->AddButton(UIButton(Vec2f(-0.8, 0.5), Vec2f(0.1, 0.2), "play again", Vec4f(1, 1, 1, 1), [&]()
		{
			Start(m_ecs);
		}));

	defeatMenu->AddButton(UIButton(Vec2f(-0.8, 0), Vec2f(0.1, 0.2), "main menu", Vec4f(1, 1, 1, 1), []() { SystemManager::UI()->SetPanel("mainMenu"); }));

	auto victoryMenu = std::make_shared<UIPanel>("victory");
	victoryMenu->AddButton(UIButton(Vec2f(-0.8, 0.5), Vec2f(0.1, 0.2), "play again", Vec4f(1, 1, 1, 1), [&]()
		{
			Start(m_ecs);
		}));

	victoryMenu->AddButton(UIButton(Vec2f(-0.8, 0), Vec2f(0.1, 0.2), "main menu", Vec4f(1, 1, 1, 1), []() { SystemManager::UI()->SetPanel("mainMenu"); }));

	SystemManager::UI()->AddPanel(mainMenu);
	SystemManager::UI()->AddPanel(pauseMenu);
	SystemManager::UI()->AddPanel(defeatMenu);
	SystemManager::UI()->AddPanel(victoryMenu);

}

BasicProjectile Game::CreateProjectile(Vec3f position, Vec3f velocity, int team)
{
	BasicProjectile proj = *basicProjectile;
	std::get<Team>(proj.components).team = team;
	std::get<Team>(proj.components).enemy = enemies[team];
	std::get<Transform>(proj.components).position = position;
	std::get<RigidBody>(proj.components).velocity = velocity;
	std::get<Sprite>(proj.components).tint = teamColors[team];
	return proj;
}

RocketProjectile Game::CreateRocket(Vec3f position, Vec3f velocity, int team)
{
	RocketProjectile rocket = *rocketProjectile;
	std::get<Team>(rocket.components).team = team;
	std::get<Team>(rocket.components).enemy = enemies[team];
	std::get<Transform>(rocket.components).position = position;
	std::get<RigidBody>(rocket.components).velocity = velocity;
	std::get<Sprite>(rocket.components).tint = teamColors[team];
	std::get<Trail>(rocket.components).color = teamColors[team];
	return rocket;
}

BasicProjectile Game::CreateBomb(Vec3f position, int team)
{
	BasicProjectile proj = *bombProjectile;
	std::get<Team>(proj.components).team = team;
	std::get<Team>(proj.components).enemy = enemies[team];
	std::get<Transform>(proj.components).position = position;
	std::get<Sprite>(proj.components).tint = teamColors[team];
	std::get<RigidBody>(proj.components).gravity = true;
	return proj;
}

PlayerShip Game::CreateTriangleShip(Vec3f position, int team)
{
	PlayerShip ship = *playerShip;
	std::get<Team>(ship.components).team = team;
	std::get<Transform>(ship.components).position = position;
	std::get<Sprite>(ship.components).tint = teamColors[team];

	std::cout << std::format("player{}-left", team) << std::endl;

	std::get<PlayerController>(ship.components).leftKey = GetKeyCode(m_config.Get<std::string>(std::format("player{}-left", team + 1)));
	std::get<PlayerController>(ship.components).rightKey = GetKeyCode(m_config.Get<std::string>(std::format("player{}-right", team + 1)));
	std::get<PlayerController>(ship.components).pfKey = GetKeyCode(m_config.Get<std::string>(std::format("player{}-primary", team + 1)));
	std::get<PlayerController>(ship.components).sfKey = GetKeyCode(m_config.Get<std::string>(std::format("player{}-secondary", team + 1)));

	return ship;
}

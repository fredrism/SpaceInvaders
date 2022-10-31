#pragma once
#include "ECS.h"
#include "Game/Game.h"
#include "Components/ALL_COMPONENTS.h"

class Sandbox
{
public:
	Sandbox();
	void Update(float dt);

	ECS ecs;
private:
	std::unique_ptr<Game> m_game;
};


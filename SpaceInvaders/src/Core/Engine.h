#pragma once
#include "pch.h"
#include "Core.h"
#include "Window.h"
#include "SDL.h"
#include "glad/glad.h"
#include "Rendering/Renderer.h"
#include "Rendering/Helpers/Gizmos.h"
#include "Input/InputSystem.h"
#include "../GameSystem/SystemManager.h"
#include "../Resources/ShaderLoader.h"
#include "../Resources/TextureLoader.h"

class Engine
{
public:
	Engine(std::string title);
	~Engine();

	void Run();

private:
	std::shared_ptr<Window> m_window;
	std::unique_ptr<SystemManager> m_systemManager;
};


#pragma once
#include <memory>
#include "../Core/Rendering/Renderer.h"
#include "../Core/Rendering/Helpers/Gizmos.h"
#include "../Core/Input/InputSystem.h"
#include "../Core/Threads/ThreadPool.h"
#include "../Core/UI/UISystem.h"
#include "Database.h"


class SystemManager
{
public:
	SystemManager()
	{
		m_renderer = AddSystem<Renderer>();
		m_ui = AddSystem<UISystem>(m_renderer);
		m_input = AddSystem<InputSystem>();
		m_gizmos = AddSystem<Gizmos>(m_renderer);

		s_instance = this;
	}

	template<typename T, typename U> 
	std::shared_ptr<T> AddSystem(U arg)
	{
		auto ptr = std::make_shared<T>(arg);
		m_systems.push_back(ptr.get());
		return ptr;
	}

	template<typename T>
	std::shared_ptr<T> AddSystem()
	{
		auto ptr = std::make_shared<T>();
		m_systems.push_back(ptr.get());
		return ptr;
	}

	~SystemManager()
	{

	}

	void BeginFrame()
	{
		for (auto system : m_systems)
		{
			system->BeginFrame();
		}
	}

	void ProcessEvent(SDL_Event& event)
	{
		for (auto system : m_systems)
		{
			if (system->ProcessEvent(event)) return;
		}
	}

	void EndFrame()
	{
		for (auto system : m_systems)
		{
			system->EndFrame();
		}
	}

	static InputSystem* Input()
	{
		return s_instance->m_input.get();
	}

	static Renderer* Graphics()
	{
		return s_instance->m_renderer.get();
	}

	static UISystem* UI()
	{
		return s_instance->m_ui.get();
	}

	static Gizmos* Gizmo()
	{
		return s_instance->m_gizmos.get();
	}

	static Database* Assets()
	{
		return s_instance->m_database.get();
	}

	static ThreadPool* Threads()
	{
		return s_instance->m_threadPool.get();
	}

	static bool& Quit() { return s_instance->m_quit; }
	

private:
	inline static SystemManager* s_instance;
	std::vector<System*> m_systems;

	std::shared_ptr<Renderer> m_renderer;
	std::shared_ptr<UISystem> m_ui;
	std::shared_ptr<InputSystem> m_input;
	std::shared_ptr<Gizmos> m_gizmos;
	std::shared_ptr<Database> m_database;
	std::shared_ptr<ThreadPool> m_threadPool;
	bool m_quit = false;
};
#include "pch.h"
#include "Engine.h"
#include "../Util/Profiler.h"
#include "../GameSystem/Sandbox.h"
#include "../Util/TickTimer.h"

const int SCREEN_TICKS_PER_FRAME = 1000 / 144;

Engine::Engine(std::string title)
{
	iCHECK_SDL_ERROR(SDL_GL_LoadLibrary(NULL));
	iCHECK_SDL_ERROR(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1));
	iCHECK_SDL_ERROR(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4));
	iCHECK_SDL_ERROR(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5));
	iCHECK_SDL_ERROR(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
	iCHECK_SDL_ERROR(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));

	m_window = std::make_shared<Window>(title, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, 640, 480);

	if (m_window->GetGLContext() == NULL)
	{
		std::cout << "Failed to create OpenGL Context!" << std::endl;
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
			return;
	}

	m_systemManager = std::make_unique<SystemManager>();
	std::cout << "OpenGL version loaded: " << GLVersion.major << "." << GLVersion.minor << std::endl;
}

Engine::~Engine()
{
	TextureLoader::Clear();
	ShaderLoader::Clear();
}

void Engine::Run()
{
	SDL_Event e;
	std::shared_ptr<Sandbox> sandbox = std::make_shared<Sandbox>();

	FrameTimer frameTimer;
	TickTimer capTimer;
	float dt = 0;
	int frameCount = 0;

	while (!m_systemManager->Quit())
	{
		dt = frameTimer.Poll();
		
		m_systemManager->BeginFrame();

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_WINDOWEVENT)
			{
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					m_systemManager->Quit() = true;
				}
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					m_systemManager->Graphics()->SetViewportSize(Vec2i(e.window.data1, e.window.data2));
				}
				
			}
			m_systemManager->ProcessEvent(e);
		}

		sandbox->Update(dt);

		m_systemManager->Graphics()->LateUpdate();

		if (frameCount > 10)
		{
			//std::cout << "FrameRate: " << 1.0 / dt << std::endl;
			frameCount = 0;
		}

		frameCount++;

		m_systemManager->EndFrame();
		m_window->Frame();

		Uint64 ticks = capTimer.GetTicks();
		if (ticks < SCREEN_TICKS_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICKS_PER_FRAME - ticks);
		}
	}
}

#pragma once
#include "pch.h"
#include "SDL.h"

class Window
{
public:
	Window(std::string title, unsigned int flags, unsigned int width, unsigned int height);
	~Window();
	void Frame();

	SDL_Window* GetSDLWindow() { return m_window; }
	SDL_GLContext* GetGLContext() { return &m_ctx; }

private:
	SDL_Window* m_window;
	SDL_GLContext m_ctx;
	unsigned int width, height;
};


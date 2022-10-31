#include "pch.h"
#include "Core.h"
#include "Window.h"

Window::Window(std::string title, unsigned int flags, unsigned int width, unsigned int height)
{
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	if (m_window == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
	}
	
	m_ctx = SDL_GL_CreateContext(m_window);
	if (m_ctx == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
	}

	iCHECK_SDL_ERROR(SDL_GL_SetSwapInterval(1));
}

void Window::Frame()
{
	SDL_GL_SwapWindow(m_window);
}

Window::~Window()
{
	SDL_GL_DeleteContext(m_ctx);
	SDL_DestroyWindow(m_window);
}

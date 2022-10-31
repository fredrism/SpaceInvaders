#include "pch.h"
#include "UISystem.h"

UISystem::UISystem(std::shared_ptr<Renderer> renderer) : m_renderer(renderer)
{
}

UISystem::~UISystem()
{
}

void UISystem::AddPanel(std::shared_ptr<UIPanel> panel)
{
	m_panels[panel->GetName()] = panel;
}

void UISystem::SetPanel(std::string screen)
{
	if (screen == "")
	{
		SDL_ShowCursor(SDL_DISABLE);
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);

	}
	m_currentScreen = screen;
}
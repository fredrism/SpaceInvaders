#pragma once
#include <unordered_map>
#include <memory>
#include "UIPanel.h"
#include "../Rendering/Renderer.h"

class UISystem : public System
{
public:
	UISystem(std::shared_ptr<Renderer> renderer);
	~UISystem();

	void AddPanel(std::shared_ptr<UIPanel> panel);
	void SetPanel(std::string screen);

	virtual void BeginFrame() {};
	
	virtual bool ProcessEvent(SDL_Event& event) 
	{ 
		if (m_currentScreen.size() != 0)
		{
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				m_panels[m_currentScreen]->OnClick(m_renderer->ScreenToViewPoint(Vec2f(event.button.x, event.button.y)));
				return true;
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				m_panels[m_currentScreen]->OnMoved(m_renderer->ScreenToViewPoint(Vec2f(event.button.x, event.button.y)));
			}
		}
		
		return false; 
	};
	
	virtual void EndFrame() 
	{
		if (m_currentScreen.size() != 0)
		{
			m_panels[m_currentScreen]->Draw(m_renderer);
		}
	};
	


public:
	std::unordered_map<std::string, std::shared_ptr<UIPanel>> m_panels;
	std::shared_ptr<Renderer> m_renderer;
	std::string m_currentScreen;
};


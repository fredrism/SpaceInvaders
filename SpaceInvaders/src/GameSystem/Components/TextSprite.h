#pragma once
#include "Components.h"
#include "Transform.h"
#include "../Resources/ConfigLoader.h"

struct TextSprite
{
	const char* text;
	Vec4f color;
	std::shared_ptr<TextGeometry> geometry;
};

class TextSystem : public ECSSystem
{
public:
	TextSystem() : m_config("resources/config.txt")
	{

	}

	virtual void OnCreate(EntityID entity)
	{
		auto& text = m_ecs->GetComponent<TextSprite>(entity);
		text.geometry = std::make_shared<TextGeometry>();
		text.geometry->SetText(text.text);
		text.geometry->SetColor(text.color);

		text.geometry->SetMetrics(m_config.Get<int>("text-symbolx"), m_config.Get<Vec2f>("text-offset"), m_config.Get<Vec2f>("text-size"));
	}

	virtual void Update(float dt)
	{
		if (SystemManager::Input()->GetKeyDown(SDLK_F3))
		{
			m_config.Load("resources/config.txt");

			for (auto& entity : m_entities)
			{
				auto& text = m_ecs->GetComponent<TextSprite>(entity);
				text.geometry->SetMetrics(m_config.Get<int>("text-symbolx"), m_config.Get<Vec2f>("text-offset"), m_config.Get<Vec2f>("text-size"));
			}
		}

		for (auto& entity : m_entities)
		{
			auto& text = m_ecs->GetComponent<TextSprite>(entity);
			auto& transform = m_ecs->GetComponent<Transform>(entity);

			text.geometry->SetText(text.text);
			text.geometry->SetColor(text.color);

			SystemManager::Graphics()->DrawText(text.geometry.get(), &transform.matrix);
		}
	}

private:
	ConfigLoader m_config;
};
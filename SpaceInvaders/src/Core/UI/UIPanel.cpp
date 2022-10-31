#include "pch.h"
#include "UIPanel.h"
#include "../GameSystem/SystemManager.h"

UIPanel::UIPanel(std::string name) : m_name(name)
{
	m_textGeometry = std::make_shared<TextGeometry>();

	ConfigLoader config("resources/config.txt");
	m_textGeometry->SetMetrics(config.Get<int>("text-symbolx"), config.Get<Vec2f>("text-offset"), config.Get<Vec2f>("text-size"));
};

void UIPanel::Draw(std::shared_ptr<Renderer> renderer)
{
	for (auto& label : m_labels)
	{
		m_textGeometry->SetColor(label.color);
		m_textGeometry->SetText(label.text);

		renderer->DrawText(m_textGeometry.get(), label.GetMatrix());
	}
	for (auto& sprite : m_sprites)
	{

	}
	for (auto& button : m_buttons)
	{
		m_textGeometry->SetColor(button.color);
		m_textGeometry->SetText(button.text);
		button.boundingBox = m_textGeometry->ComputeBoundingBox(button.GetMatrix());

		renderer->DrawText(m_textGeometry.get(), button.GetMatrix());
	}
};

void UIPanel::AddButton(UIButton button)
{
	m_buttons.push_back(button);
}
void UIPanel::AddLabel(UILabel label)
{
	m_labels.push_back(label);
}
void UIPanel::AddSprite(UISprite sprite)
{
	m_sprites.push_back(sprite);
}

void UIPanel::OnClick(Vec2f position)
{
	for (auto& button : m_buttons)
	{
		button.OnClick(position);
	}
}

void UIPanel::OnMoved(Vec2f position)
{
	for (auto& button : m_buttons)
	{
		button.OnMove(position);
	}
}
#pragma once
#include "UIElements.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/Geometry/TextGeometry.h"
#include "../Resources/ConfigLoader.h"

class UIPanel
{
public:
	UIPanel(std::string name);

	std::string GetName() const { return m_name; }

	void Draw(std::shared_ptr<Renderer> renderer);

	void AddButton(UIButton button);
	void AddLabel(UILabel label);
	void AddSprite(UISprite sprite);

	void OnClick(Vec2f position);
	void OnMoved(Vec2f position);

private:
	std::vector<UILabel> m_labels;
	std::vector<UISprite> m_sprites;
	std::vector<UIButton> m_buttons;
	std::shared_ptr<TextGeometry> m_textGeometry;

	std::string m_name;
};
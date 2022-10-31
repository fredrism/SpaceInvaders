#pragma once
#include "../Rendering/Material.h"
#include "../GameSystem/Components/Physics/AABB.h"
#include <functional>

class UIElement
{
public:
	UIElement(Vec2f& pos, Vec2f& size)
	{
		Vec3f position = pos;
		Vec3f scale = size;
		Vec3f rotation = { 0,0,0 };
		m_matrix.SetTRS(position, rotation, scale);
	}

	Matrix4f* GetMatrix() { return &m_matrix; }
protected:
	Matrix4f m_matrix;
};

class UILabel : public UIElement
{
public:
	UILabel(Vec2f pos, Vec2f size, std::string _text, Vec4f _color) : UIElement(pos, size), color(_color), text(_text) {}

	std::string text;
	Vec4f color;
};

class UISprite : public UIElement
{
public:
	std::shared_ptr<Texture2D> texture;
};

class UIButton : public UIElement
{
public:
	UIButton(Vec2f _pos, Vec2f _size, std::string _text, Vec4f _color, std::function<void()> _func)
		: text(_text), onClick(_func), color(_color), UIElement(_pos, _size)
	{
		pos = _pos;
		size = _size;
	}

	void OnClick(Vec2f point)
	{
		if (boundingBox.Contains(point)) onClick();
	}

	void OnMove(Vec2f point)
	{
		if (boundingBox.Contains(point))
		{
			if (!hover)
			{
				Vec3f position = pos;
				Vec3f scale = size * 1.1;
				Vec3f rotation = { 0,0,0 };
				m_matrix.SetTRS(position, rotation, scale);
			}

			hover = true;
		}
		else
		{
			if (hover)
			{
				Vec3f position = pos;
				Vec3f scale = size;
				Vec3f rotation = { 0,0,0 };
				m_matrix.SetTRS(position, rotation, scale);
			}
			hover = false;
		}

	}

	std::string text;
	Vec4f color;
	std::function<void()> onClick;
	AABB boundingBox;
	Vec2f pos;
	Vec2f size;

	bool hover = false;
};
#pragma once
#include "../Material.h"
#include "RectGeometry.h"
#include "../GameSystem/Components/Physics/AABB.h"

class Renderer;

class TextGeometry
{
public:
	TextGeometry();

	void SetColor(Vec4f color);
	void SetText(std::string text);

	void SetMetrics(int symbolsX, Vec2f offset, Vec2f size);

	AABB ComputeBoundingBox(Matrix4f* matrix);

private:
	int m_symbolsX = 15;
	Vec2f m_scale;
	Vec2f m_offset;

	std::vector<Vec2f> m_offsets;
	Vec4f m_color;
	RectGeometry m_rect;
	std::shared_ptr<Material> m_textMaterial;

	std::string m_text;

	void GenerateBuffers();
	

	friend class Renderer;
};


#include "pch.h"
#include "TextGeometry.h"
#include "../Resources/ShaderLoader.h"
#include "../Resources/TextureLoader.h"
#include "../GameSystem/Components/Physics/AABB.h"

TextGeometry::TextGeometry() : m_rect(Vec2f(1,1))
{
	auto shader = ShaderLoader::Load("resources/shaders/bitmap_font.glsl");
	auto texture = TextureLoader::LoadTexture("resources/sprites/mono_font.jpg");
	m_textMaterial = std::make_shared<Material>(shader, false);
	m_textMaterial->SetTexture("u_texture", texture);

	GenerateBuffers();
}

void TextGeometry::SetColor(Vec4f color)
{
	m_color = color;
	m_textMaterial->SetProperty<Vec4f>("u_color", color);
}

void TextGeometry::SetText(std::string text)
{
	if (text == m_text) return;
	m_text = text;
	GenerateBuffers();
}

void TextGeometry::SetMetrics(int symbolsX, Vec2f offset, Vec2f size)
{
	int m_symbolsX = symbolsX;
	m_offset = offset;
	m_scale = size;
	GenerateBuffers();
}

AABB TextGeometry::ComputeBoundingBox(Matrix4f* matrix)
{
	Vec3f min(-0.5, -0.5, 0);
	Vec3f max(0.5 + m_text.size(), 0.5, 0);
	return AABB(matrix->MulPoint(min), matrix->MulPoint(max));
}

void TextGeometry::GenerateBuffers()
{
	m_offsets.clear();
	for (unsigned int i = 0; i < m_text.size(); i++)
	{
		char ch = m_text[i] - 'a';

		int xpos = (ch % m_symbolsX);
		int ypos = (ch / m_symbolsX);

		if (m_text[i] == ' ') ypos = -2;

		m_offsets.push_back(Vec2f(m_offset.x + m_scale.x * xpos, m_offset.y - m_scale.y * ypos));
	}
}

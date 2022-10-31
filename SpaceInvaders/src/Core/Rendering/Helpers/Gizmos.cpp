#include "pch.h"
#include "Gizmos.h"

Gizmos::Gizmos(std::shared_ptr<Renderer> renderer) : m_renderer(renderer)
{
	auto shader = ShaderLoader::Load("resources/shaders/unlit_vertexcolor.glsl");
	m_lineMaterial = std::make_shared<Material>(shader);
}

Gizmos::~Gizmos()
{
}

void Gizmos::BeginFrame()
{
	m_lines.clear();
}

void Gizmos::DrawLine(Vec3f start, Vec3f end, Vec3f color)
{
	std::scoped_lock lock(drawMutex);
	m_lines.push_back({start, color});
	m_lines.push_back({ end, color });
}

void Gizmos::DrawCircle(Vec3f center, float radius, Vec3f color)
{
	const unsigned int resolution = 16;

	for (unsigned int i = 0; i < resolution; i++)
	{
		float t0 = (float)i / (resolution);
		float t1 = (float)(i + 1) / (resolution);

		Vec3f a0(std::sinf(t0 * 2 * SIMath::pi), std::cosf(t0 * 2 * SIMath::pi), 0);
		Vec3f a1(std::sinf(t1 * 2 * SIMath::pi), std::cosf(t1 * 2 * SIMath::pi), 0);

		DrawLine(center + a0 * radius, center + a1 * radius, color);
	}
}

void Gizmos::DrawBox(Vec2f size, Matrix4f& matrix, Vec3f color)
{
	Vec3f A(-size.x, -size.y, 0);
	Vec3f B(size.x, -size.y, 0);
	Vec3f C(-size.x, size.y, 0);
	Vec3f D(size.x, size.y, 0);

	A = matrix.MulPoint(A);
	B = matrix.MulPoint(B);
	C = matrix.MulPoint(C);
	D = matrix.MulPoint(D);

	DrawLine(A, B, color);
	DrawLine(B, D, color);
	DrawLine(D, C, color);
	DrawLine(C, A, color);
}

void Gizmos::DrawBox(Vec2f min, Vec2f max, Vec3f color)
{
	Vec3f A(min.x, min.y, 0);
	Vec3f B(max.x, min.y, 0);
	Vec3f C(min.x, max.y, 0);
	Vec3f D(max.x, max.y, 0);

	DrawLine(A, B, color);
	DrawLine(B, D, color);
	DrawLine(D, C, color);
	DrawLine(C, A, color);
}

void Gizmos::EndFrame()
{
	if (m_lines.size() > 0)
	{
		m_geometry.SetVertices(m_lines);
		m_renderer->DrawLines(&m_geometry.geometry, m_lineMaterial.get());
	}
}

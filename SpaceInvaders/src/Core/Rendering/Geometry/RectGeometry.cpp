#include "pch.h"
#include "RectGeometry.h"
#include <array>

RectGeometry::RectGeometry(Vec2f dimensions) : geometry(vertex_pos_uv_layout(), GL_STATIC_DRAW)
{
	auto min = Vec2f(-dimensions.x / 2, -dimensions.y / 2);
	auto max = Vec2f(dimensions.x / 2, dimensions.y / 2);
	GenerateGeometry(min, max);
}

RectGeometry::RectGeometry(Vec2f min, Vec2f max) : geometry(vertex_pos_uv_layout(), GL_STATIC_DRAW)
{
	GenerateGeometry(min, max);
}

RectGeometry::~RectGeometry()
{
}

void RectGeometry::GenerateGeometry(Vec2f& min, Vec2f& max)
{
	std::vector<vertex_pos_uv> vertices;
	std::vector<unsigned int> indices = { 0, 1, 2, 1, 2, 3 };

	vertices.push_back({ Vec3f(min.x, min.y, 0), Vec2f(0, 0) });
	vertices.push_back({ Vec3f(max.x, min.y, 0), Vec2f(1, 0) });
	vertices.push_back({ Vec3f(min.x, max.y, 0), Vec2f(0, 1) });
	vertices.push_back({ Vec3f(max.x, max.y, 0), Vec2f(1, 1) });

	geometry.SetVertices(vertices);
	geometry.SetIndices(indices);
}

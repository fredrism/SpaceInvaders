#pragma once
#include "../VMath/VecXX.h"
#include "VertexLayout.h"

struct vertex_pos_uv
{
	Vec3f position;
	Vec2f uv;
};

struct vertex_pos_col
{
	Vec3f position;
	Vec3f color;
};

inline VertexLayout vertex_pos_uv_layout()
{
	VertexLayout layout;
	layout.AddAttribute(GL_FLOAT, 3);
	layout.AddAttribute(GL_FLOAT, 2);
	return layout;
};

inline VertexLayout vertex_pos_col_layout()
{
	VertexLayout layout;
	layout.AddAttribute(GL_FLOAT, 3);
	layout.AddAttribute(GL_FLOAT, 3);
	return layout;
};
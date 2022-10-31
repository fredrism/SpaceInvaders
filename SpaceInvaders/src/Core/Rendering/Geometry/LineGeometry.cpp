#include "pch.h"
#include "LineGeometry.h"

LineGeometry::LineGeometry() : geometry(vertex_pos_col_layout(), GL_DYNAMIC_DRAW)
{
}

LineGeometry::~LineGeometry()
{
}

void LineGeometry::SetVertices(std::vector<vertex_pos_col> vertices)
{
	std::vector<unsigned int> indices;

	for (int i = 0; i < vertices.size(); i++)
	{
		indices.push_back(i);
	}

	geometry.SetIndices(indices);
	geometry.SetVertices<vertex_pos_col>(vertices);
}

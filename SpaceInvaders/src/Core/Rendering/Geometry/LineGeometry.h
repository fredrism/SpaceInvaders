#pragma once
#include "BufferGeometry.h"
#include "../Buffers/VertexFormat.h"

class LineGeometry
{
public:
	LineGeometry();
	~LineGeometry();

	void SetVertices(std::vector<vertex_pos_col> vertices);
	BufferGeometry geometry;
};


#pragma once
#include "../VMath/VecXX.h"
#include "../Buffers/IndexBuffer.h"
#include "BufferGeometry.h"
#include "../Buffers/VertexFormat.h"

class Renderer;

class RectGeometry
{
public:
	RectGeometry(Vec2f dimensions);
	RectGeometry(Vec2f min, Vec2f max);

	~RectGeometry();

	BufferGeometry geometry;
private:
	void GenerateGeometry(Vec2f& min, Vec2f& max);
};


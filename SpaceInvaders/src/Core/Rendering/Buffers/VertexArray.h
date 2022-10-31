#pragma once
#include "../GL_Debugger.h"
#include "glad/glad.h"
#include <vector>
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "../VMath/VecXX.h"

class VertexArray
{
public:
	VertexArray(VertexLayout layout);
	~VertexArray();

	void Bind();
	void Unbind();

	VertexBuffer buffer;
private:
	unsigned int m_id;
};

#pragma once
#include "../GL_Debugger.h"
#include "glad/glad.h"
#include <vector>
#include "../VMath/VecXX.h"

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Bind();
	void Unbind();

	template <typename T>
	void BufferData(std::vector<T>& vertices, unsigned int usage = GL_STATIC_DRAW)
	{
		Bind();
		GLDEBUG(glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), (const void*)&vertices[0], usage));
		Unbind();
	}

private:
	unsigned int m_id;
};


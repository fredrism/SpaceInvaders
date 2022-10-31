#pragma once
#include "glad/glad.h"
#include "../GL_Debugger.h"
#include <vector>

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void Bind();
	void Unbind();

	void BufferData(std::vector<unsigned int>& indices, unsigned int usage = GL_STATIC_DRAW);

	unsigned int Count() const { return m_count; }

private:
	unsigned int m_id;
	unsigned int m_count;
};


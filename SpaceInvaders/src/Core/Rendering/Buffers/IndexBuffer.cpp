#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
	GLDEBUG(glGenBuffers(1, &m_id));
	m_count = 0;
}

IndexBuffer::~IndexBuffer()
{
	GLDEBUG(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Bind()
{
	GLDEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::Unbind()
{
	GLDEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::BufferData(std::vector<unsigned int>& indices, unsigned int usage)
{
	m_count = indices.size();

	Bind();
	GLDEBUG(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), (const void*)&indices[0], usage));
	Bind();
}

#include "pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	GLDEBUG(glGenBuffers(1, &m_id));
}

VertexBuffer::~VertexBuffer()
{
	GLDEBUG(glDeleteBuffers(1, &m_id));
}

void VertexBuffer::Bind()
{
	GLDEBUG(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::Unbind()
{
	GLDEBUG(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

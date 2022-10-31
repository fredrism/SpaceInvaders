#include "pch.h"
#include "VertexArray.h"

VertexArray::VertexArray(VertexLayout layout) : buffer()
{
	GLDEBUG(glGenVertexArrays(1, &m_id));
	buffer.Bind();
	Bind();
	for (int i = 0; i < layout.Length(); i++)
	{
		VertexAttribute attrib = layout.GetAttribute(i);

		GLDEBUG(glEnableVertexAttribArray(i));
		GLDEBUG(glVertexAttribPointer(i, attrib.width, attrib.type, GL_FALSE, layout.Stride(), (const void*)attrib.offset));
	}
	Unbind();
	buffer.Unbind();
}

VertexArray::~VertexArray()
{
	GLDEBUG(glClear(GL_COLOR_BUFFER_BIT));
	GLDEBUG(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::Bind()
{
	GLDEBUG(glBindVertexArray(m_id));
}

void VertexArray::Unbind()
{
	GLDEBUG(glBindVertexArray(0));
}

#pragma once
#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/VertexArray.h"

class Renderer;

class BufferGeometry
{
public:
	BufferGeometry(VertexLayout layout, unsigned int usage);
	~BufferGeometry();

	template<typename T>
	void SetVertices(std::vector<T>& vertices)
	{
		m_va.buffer.BufferData(vertices, m_usage);
	};

	void SetIndices(std::vector<unsigned int>& indices);

	void Bind();
	void Unbind();

private:
	IndexBuffer m_ib;
	VertexArray m_va;
	unsigned int m_usage;

	friend class Renderer;
};


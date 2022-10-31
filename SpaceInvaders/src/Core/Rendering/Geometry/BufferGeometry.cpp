#include "pch.h"
#include "BufferGeometry.h"

BufferGeometry::BufferGeometry(VertexLayout layout, unsigned int usage) : m_va(layout), m_usage(usage)
{

}

BufferGeometry::~BufferGeometry()
{
}

void BufferGeometry::SetIndices(std::vector<unsigned int>& indices)
{
	m_ib.BufferData(indices, m_usage);
}

void BufferGeometry::Bind()
{
	m_va.Bind();
	m_ib.Bind();
}

void BufferGeometry::Unbind()
{
	m_ib.Unbind();
	m_va.Unbind();
}

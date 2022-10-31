#pragma once
#include <vector>
#include "glad/glad.h"
#include "../VMath/VecXX.h"
#include "../VMath/Matrix4f.h"


static constexpr size_t GLSizeOf(unsigned int t)
{
	switch (t)
	{
	case GL_INT:
		return sizeof(int);
	case GL_FLOAT:
		return sizeof(float);
	case GL_FLOAT_VEC2:
		return sizeof(Vec2f);
	case GL_FLOAT_VEC3:
		return sizeof(Vec3f);
	case GL_FLOAT_VEC4:
		return sizeof(Vec4f);
	case GL_FLOAT_MAT4:
		return sizeof(Matrix4f);
	}

	return 0;
}

struct VertexAttribute
{
	unsigned int type;
	unsigned int width;
	size_t offset;
	size_t size;
};

struct VertexLayout
{
public:
	VertexLayout() : m_stride(0) {}

	void AddAttribute(unsigned int type, unsigned int num)
	{
		m_attributes.push_back({
			type, num, m_stride, GLSizeOf(type)
		});

		m_stride += GLSizeOf(type)*num;
	};

	VertexAttribute GetAttribute(unsigned int i)
	{
		return m_attributes[i];
	};

	size_t Length() const { return m_attributes.size(); };
	size_t Stride() const { return m_stride; };


private:
	size_t m_stride;
	std::vector<VertexAttribute> m_attributes;
};
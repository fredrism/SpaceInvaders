#pragma once
#include "../FrameBuffer.h"
#include "glad/glad.h"
#include <vector>
#include "../Shader.h"
#include "../Resources/ShaderLoader.h"
#include "../Geometry/BufferGeometry.h"


class Bloom
{
public:
	Bloom(unsigned int width, unsigned int height);

	void Apply(FrameBuffer* fb);

	~Bloom()
	{

	}

	void Resize(unsigned int width, unsigned int height)
	{
		m_frameBuffers[0]->Resize(width, height);
		m_frameBuffers[1]->Resize(width, height);
	}

private:
	unsigned int m_strength = 10;
	unsigned int m_directionLocation;
	unsigned int m_textureLocation;
	std::shared_ptr<Shader> m_blurShader;
	std::shared_ptr<Shader> m_addShader;
	std::vector<std::unique_ptr<FrameBuffer>> m_frameBuffers;
};
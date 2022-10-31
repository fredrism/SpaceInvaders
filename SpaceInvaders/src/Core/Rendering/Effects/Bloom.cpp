#include "pch.h"
#include "Bloom.h"
#include "../GameSystem/SystemManager.h"

Bloom::Bloom(unsigned int width, unsigned int height)
{
	m_frameBuffers.push_back(std::make_unique<FrameBuffer>(GL_RGBA, GL_RGBA16F, width, height));
	m_frameBuffers.push_back(std::make_unique<FrameBuffer>(GL_RGBA, GL_RGBA16F, width, height));

	m_blurShader = ShaderLoader::Load("resources/shaders/gaussian_blur.glsl");
	m_addShader = ShaderLoader::Load("resources/shaders/linear_add.glsl");

	m_directionLocation = glGetUniformLocation(m_blurShader->m_id, "u_horizontal");
	m_textureLocation = glGetUniformLocation(m_blurShader->m_id, "image");
}

void Bloom::Apply(FrameBuffer* fb)
{
	m_blurShader->Bind();
	GLDEBUG(glActiveTexture(GL_TEXTURE0));
	GLDEBUG(glUniform1i(m_textureLocation, 0));

	unsigned int i = 0;
	for (i; i < m_strength; i++)
	{
		auto& fba = m_frameBuffers[i % 2];
		auto& fbb = m_frameBuffers[(i + 1) % 2];
	
		unsigned int texture = (i == 0) ? fb->GetColorBuffer() : fbb->m_colorBuffer;
		GLDEBUG(glBindFramebuffer(GL_FRAMEBUFFER, fba->m_fbo));
		GLDEBUG(glUniform1i(m_directionLocation, i % 2));
		GLDEBUG(glBindTexture(GL_TEXTURE_2D, texture));
		SystemManager::Graphics()->Blit();
	}

	unsigned int a = glGetUniformLocation(m_addShader->m_id, "a"); 
	unsigned int b = glGetUniformLocation(m_addShader->m_id, "b");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_addShader->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fb->GetColorBuffer());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_frameBuffers[(i + 1) % 2]->GetColorBuffer());
	glUniform1i(a, 0);
	glUniform1i(b, 1);
	SystemManager::Graphics()->Blit();

}
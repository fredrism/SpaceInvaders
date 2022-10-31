#pragma once
#include "glad/glad.h"
#include "GL_Debugger.h"

class FrameBuffer
{
public:
	FrameBuffer(unsigned int format, unsigned int internalFormat, unsigned int width, unsigned int height)
	{
		GLDEBUG(glGenFramebuffers(1, &m_fbo));
		GLDEBUG(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
		GLDEBUG(glGenTextures(1, &m_colorBuffer));
		GLDEBUG(glBindTexture(GL_TEXTURE_2D, m_colorBuffer));
		GLDEBUG(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, nullptr));

		GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLDEBUG(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Hello???" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_format = format;
		m_internalFormat = internalFormat;
	}

	~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	void Resize(unsigned int width, unsigned int height)
	{
		GLDEBUG(glBindTexture(GL_TEXTURE_2D, m_colorBuffer));
		GLDEBUG(glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, GL_FLOAT, nullptr));
	}

	void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	unsigned int GetColorBuffer() const { return m_colorBuffer; }
	unsigned int m_fbo;
	unsigned int m_colorBuffer;
	unsigned int m_format;
	unsigned int m_internalFormat;
private:
	
	
};
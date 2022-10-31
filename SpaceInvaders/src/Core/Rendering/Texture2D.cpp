#include "pch.h"
#include "Texture2D.h"

unsigned int TextureFormat(unsigned int components)
{
	switch (components)
	{
	case 1:
		return GL_R;
	case 2:
		return GL_RG;
	case 3:
		return GL_RGB;
	case 4:
		return GL_RGBA;
	default:
		break;
	}

	return 0;
}

Texture2D::Texture2D(unsigned int width, unsigned int height, unsigned int components, unsigned char* pixels)
{
	GLDEBUG(glGenTextures(1, &m_id));
	GLDEBUG(glBindTexture(GL_TEXTURE_2D, m_id));

	GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	unsigned int fmt = TextureFormat(components);
	GLDEBUG(glTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, pixels));
	GLDEBUG(glGenerateMipmap(GL_TEXTURE_2D));

	m_width = width;
	m_height = height;
}

Texture2D::~Texture2D()
{
	GLDEBUG(glDeleteTextures(1, &m_id));
}

void Texture2D::Bind(unsigned int slot)
{
	GLDEBUG(glActiveTexture(GL_TEXTURE0 + slot));
	GLDEBUG(glBindTexture(GL_TEXTURE_2D, m_id));
}

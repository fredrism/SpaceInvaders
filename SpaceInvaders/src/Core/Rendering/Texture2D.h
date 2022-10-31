#pragma once
#include "GL_Debugger.h"
#include "glad/glad.h"

class Texture2D
{
public:
	Texture2D(unsigned int width, unsigned int height, unsigned int components, unsigned char* pixels);
	~Texture2D();

	void Bind(unsigned int slot);
	unsigned int GetWidth() const { return m_width; }
	unsigned int GetHeight() const { return m_height; }

private:
	unsigned int m_id;
	unsigned int m_width;
	unsigned int m_height;
};


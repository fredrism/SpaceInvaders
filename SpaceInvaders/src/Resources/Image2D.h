#pragma once
#include "../VMath/VecXX.h"

class Image2D
{
public:
	Image2D(unsigned int width, unsigned int height)
	{

	}

	~Image2D()
	{

	}

	void SetPixels(unsigned char* pixelData, size_t size)
	{
		m_pixels.resize(size / 4);
		for (size_t i = 0; i < size; i += 4)
		{
			m_pixels[i].r = ((float)pixelData[i + 0]) / std::numeric_limits<unsigned char>::max();
			m_pixels[i].g = ((float)pixelData[i + 1]) / std::numeric_limits<unsigned char>::max();
			m_pixels[i].b = ((float)pixelData[i + 2]) / std::numeric_limits<unsigned char>::max();
			m_pixels[i].a = ((float)pixelData[i + 3]) / std::numeric_limits<unsigned char>::max();
		}
	}

private:
	std::vector<Vec4f> m_pixels;
};


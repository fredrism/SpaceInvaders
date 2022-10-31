#include "pch.h"
#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureLoader::~TextureLoader()
{
}

std::shared_ptr<Image2D> TextureLoader::LoadImage(std::string path)
{
	int w, h, n;

	unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &n, 4);
	auto image = std::make_shared<Image2D>(w, h);
	image->SetPixels(pixels, w * h * 4);
	stbi_image_free(pixels);
	return image;
}

std::shared_ptr<Texture2D> TextureLoader::LoadTexture(std::string path)
{
	if (m_textureCache.contains(path)) return m_textureCache[path];
	int w, h, n;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &n, 0);
	if (pixels == nullptr) std::cout << "Failed to load texture: " << path << "!" << std::endl;
	auto texture = std::make_shared<Texture2D>(w, h, n, pixels);
	stbi_image_free(pixels);

	m_textureCache[path] = texture;

	return texture;
}

TextureLoader::TextureLoader()
{
}

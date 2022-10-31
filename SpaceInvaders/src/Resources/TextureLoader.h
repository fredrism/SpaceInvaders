#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../Core/Rendering/Texture2D.h"
#include "Image2D.h"

class TextureLoader
{
public:
	~TextureLoader();

	static void Clear() { m_textureCache.clear(); }
	static std::shared_ptr<Texture2D> LoadTexture(std::string path);
	static std::shared_ptr<Image2D> LoadImage(std::string path);
private:
	TextureLoader();
	inline static std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textureCache;
};


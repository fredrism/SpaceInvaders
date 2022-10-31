#pragma once
#include <memory>
#include "Shader.h"
#include "Texture2D.h"
#include "../Util/MultiMap.h"

class Material
{
public:
	Material(std::shared_ptr<Shader> shader, bool transparent = false);
	~Material();

	template<typename T>
	void SetProperty(std::string name, T value);

	void SetTexture(std::string name, std::shared_ptr<Texture2D> texture)
	{
		m_textures[name] = { m_textureSlotCounter++, texture };
	}

	std::shared_ptr<Shader> GetShader() { return m_shader; }
	bool BlendMode() { return m_transparent; }

	void Use();

private:
	std::shared_ptr<Shader> m_shader;
	MultiMap<std::string> m_uniforms;
	std::unordered_map<std::string, std::pair<unsigned int, std::shared_ptr<Texture2D>>> m_textures;
	unsigned int m_textureSlotCounter;
	bool m_transparent;
};

template<typename T>
void Material::SetProperty(std::string name, T value)
{
	m_uniforms.Set<T>(name, value);
}
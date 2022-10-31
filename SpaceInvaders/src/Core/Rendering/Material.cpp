#include "pch.h"
#include "Material.h"

Material::Material(std::shared_ptr<Shader> shader, bool transparent) : m_shader(shader), m_textureSlotCounter(0), m_transparent(transparent)
{
	std::cout << "Material Created : " << shader->m_id << std::endl;
}

Material::~Material()
{
	std::cout << "Material Destroyed : " << m_shader->m_id << std::endl;
}

void Material::Use()
{
	m_shader->Bind();
	m_shader->SetUniforms(m_uniforms);

	for (auto& texture : m_textures)
	{
		int textureSlot = texture.second.first;

		texture.second.second->Bind(textureSlot);
		m_shader->SetTexture(texture.first, textureSlot);
	}
}

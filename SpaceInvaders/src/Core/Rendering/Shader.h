#pragma once
#include "GL_Debugger.h"
#include <vector>
#include <string>
#include <glad/glad.h>
#include "../Util/MultiMap.h"

class ShaderLoader;

struct ShaderUniform
{
	unsigned int type;
	int location;
};

class Shader
{
public:
	Shader();
	~Shader();
	void Bind();
	void Unbind();
	void SetUniforms(MultiMap<std::string>& map);

	void SetTexture(std::string name, unsigned int slot);

	int m_id;
private:
	std::unordered_map<std::string, ShaderUniform> m_uniforms;
	

	friend class ShaderLoader;
};

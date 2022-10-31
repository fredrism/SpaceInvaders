#include "pch.h"
#include "Shader.h"

Shader::~Shader()
{
	Unbind();
	GLDEBUG(glDeleteProgram(m_id));
}

void Shader::Bind()
{
	GLDEBUG(glUseProgram(m_id));
}

void Shader::Unbind()
{
	GLDEBUG(glUseProgram(0));
}

void Shader::SetUniforms(MultiMap<std::string>& map)
{
	for (auto& u : m_uniforms)
	{
		switch (u.second.type)
		{
		case GL_INT:
			GLDEBUG(glUniform1iv(u.second.location, 1, map.Get<int>(u.first)));
			break;
		case GL_FLOAT:
			GLDEBUG(glUniform1fv(u.second.location, 1, map.Get<float>(u.first)));
			break;
		case GL_FLOAT_VEC2:
			GLDEBUG(glUniform2fv(u.second.location, 1, map.Get<float>(u.first)));
			break;
		case GL_FLOAT_VEC3:
			GLDEBUG(glUniform3fv(u.second.location, 1, map.Get<float>(u.first)));
			break;
		case GL_FLOAT_VEC4:
			GLDEBUG(glUniform4fv(u.second.location, 1, map.Get<float>(u.first)));
			break;
		case GL_FLOAT_MAT4:
			GLDEBUG(glUniformMatrix4fv(u.second.location, 1, false, map.Get<float>(u.first)));
			break;
		}
	}
}

void Shader::SetTexture(std::string name, unsigned int slot)
{
	GLDEBUG(glUniform1i(m_uniforms[name].location, slot));
}

Shader::Shader()
{
}

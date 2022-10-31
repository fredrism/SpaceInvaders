#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <memory>
#include "../Core/Rendering/Shader.h"
#include "glad/glad.h"
#include <unordered_map>

struct ShaderProgramSource
{
	std::string source;
	unsigned int type;
};

class ShaderLoader
{
public:
	~ShaderLoader() {};

	static void Clear() { m_shaderCache.clear(); }
	static std::shared_ptr<Shader> Load(std::string path);

private:
	ShaderLoader() {};
	static unsigned int CompileShader(ShaderProgramSource& shaderSource);
	static void ParseShader(const std::string& source, std::vector<ShaderProgramSource>& destination, std::unordered_map<std::string, ShaderUniform>& uniforms);
	inline static std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaderCache;
};


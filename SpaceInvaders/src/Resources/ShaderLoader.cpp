#include "pch.h"
#include "ShaderLoader.h"

std::shared_ptr<Shader> ShaderLoader::Load(std::string path)
{
    if (m_shaderCache.contains(path)) return m_shaderCache[path];

    std::ifstream f(path);
    std::stringstream ss;
    ss << f.rdbuf();

    auto shader = std::make_shared<Shader>();
    shader->m_id = glCreateProgram();

    std::vector<ShaderProgramSource> shaders;

    ParseShader(ss.str(), shaders, shader->m_uniforms);

    for (auto& s : shaders)
    {
        int result = CompileShader(s);
        glAttachShader(shader->m_id, result);
        glDeleteShader(result);
    }

    glLinkProgram(shader->m_id);

    int result;
    glGetProgramiv(shader->m_id, GL_LINK_STATUS, &result);
    if (result != GL_TRUE)
    {
        std::cout << "Failed to link shader: " << path << std::endl;
        char error[512];
        glGetProgramInfoLog(shader->m_id, 512, NULL, error);
        std::cout << error << std::endl;
    }

    shader->Bind();
    for (auto& u : shader->m_uniforms)
    {
        u.second.location = glGetUniformLocation(shader->m_id, u.first.c_str());
    }
    shader->Unbind();

    m_shaderCache[path] = shader;

    return shader;
}

unsigned int ShaderLoader::CompileShader(ShaderProgramSource& shaderSource)
{
    const char* p_src = shaderSource.source.c_str();
    unsigned int shader = glCreateShader(shaderSource.type);
    glShaderSource(shader, 1, &p_src, NULL);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
    {
        std::cout << "Failed to compile shader: \n" << shaderSource.source << "\n";
        char error[512];
        glGetShaderInfoLog(shader, 512, NULL, error);
        std::cout << error << std::endl;
    }

    return shader;
}

unsigned int GLTypenameToMacro(std::string& name)
{
    if (name == "float") return GL_FLOAT;
    if (name == "vec2") return GL_FLOAT_VEC2;
    if (name == "vec3") return GL_FLOAT_VEC3;
    if (name == "vec4") return GL_FLOAT_VEC4;

    if (name == "mat4") return GL_FLOAT_MAT4;

    if (name == "int") return GL_INT;
    if (name == "sampler2D") return GL_SAMPLER_2D;

    return 0;
}

void ShaderLoader::ParseShader(const std::string& source, std::vector<ShaderProgramSource>& destination, std::unordered_map<std::string, ShaderUniform>& uniforms)
{
    std::regex lineSplit("\\n+");
    std::sregex_token_iterator begin(source.begin(), source.end(), lineSplit, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(begin, end);

    std::string shaderSource;
    unsigned int type = 0;

    for (auto& line : vec)
    {
        if (line == "#vertex")
        {
            if (type != 0)
            {
                destination.push_back({ shaderSource, type });
            }

            type = GL_VERTEX_SHADER;
            shaderSource = "";
        }
        else if (line == "#fragment")
        {
            if (type != 0)
            {
                destination.push_back({ shaderSource, type });
            }

            type = GL_FRAGMENT_SHADER;
            shaderSource = "";
        }
        else if (line == "#geometry")
        {
            if (type != 0)
            {
                destination.push_back({ shaderSource, type });
            }

            type = GL_GEOMETRY_SHADER;
            shaderSource = "";
        }
        else if (line == "#end")
        {
            if (type != 0)
            {
                destination.push_back({ shaderSource, type });
            }
        }
        else
        {
            std::regex token("\\s+");
            std::sregex_token_iterator iter(line.begin(), line.end(), token, -1);
            std::sregex_token_iterator end;
            std::vector<std::string> tokens(iter, end);

            if (tokens.size() > 0 && tokens[0] == "uniform")
            {
                std::string name = tokens[2].substr(0, tokens[2].length() - 1);
                std::string typeName = tokens[1].substr(0, tokens[1].length());
                unsigned int type = GLTypenameToMacro(typeName);
                uniforms[name] = {type, 0};
            }

            shaderSource.append(line);
            shaderSource.append("\n");
        }
    }
}

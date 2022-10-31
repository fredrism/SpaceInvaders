#include "pch.h"
#include "ConfigLoader.h"

ConfigLoader::ConfigLoader(std::string path)
{
    Load(path);
}

void ConfigLoader::Load(std::string path)
{
    m_settings.clear();
    std::ifstream f(path);
    std::stringstream ss;
    ss << f.rdbuf();

    std::string line;
    while (std::getline(ss, line, '\n'))
    {
        size_t split = line.find('=');
        m_settings[line.substr(0, split)] = line.substr(split + 1, line.size());
    }
}

#pragma once
#include <unordered_map>
#include <string>
#include <fstream>
#include "../VMath/VecXX.h"
#include "../VMath/Arithmetic.h"
#include <sstream>

class ConfigLoader
{
public:
	ConfigLoader(std::string path);
	void Load(std::string path);

	template<typename T>
	T Get(std::string key) { assert(false); }

	template<>
	std::string Get(std::string key)
	{
		return m_settings[key];
	}

	template<Arithmetic T>
	T Get(std::string key)
	{
		return std::stof(m_settings[key]);
	}

	template<>
	Vec2f Get(std::string key)
	{
		std::string value = m_settings[key];
		Vec2f result;
		ReadVector((float*)&result, 2, value);
		return result;
	}

	template<>
	Vec3f Get(std::string key)
	{
		std::string value = m_settings[key];
		Vec3f result;
		ReadVector((float*)&result, 3, value);
		return result;
	}

	template<>
	Vec4f Get(std::string key)
	{
		std::string value = m_settings[key];
		Vec4f result;
		ReadVector((float*)&result, 4, value);
		return result;
	}

private:
	void ReadVector(float* data, unsigned int width, std::string& value)
	{
		std::stringstream ss;
		unsigned int i = 0;
		for (auto ch : value)
		{
			if (ch == '(' || ch == ' ') continue;
			if (ch == ',' || ch == ')')
			{
				*data = std::stof(ss.str());
				ss.str("");
				i++;
				data++;
			}
			else
			{
				ss << ch;
			}
		}
	}

	std::unordered_map<std::string, std::string> m_settings;
};


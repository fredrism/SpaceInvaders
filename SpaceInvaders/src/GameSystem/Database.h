#pragma once
#include <string>
#include <unordered_map>
#include <typeinfo>
#include <memory>

class Database
{
public:
	template<typename T> 
	void Set(std::string name, std::shared_ptr<T> asset)
	{
		m_assets[name] = asset;
	}

	template<typename T>
	std::shared_ptr<T> Get(std::string name)
	{
		return static_pointer_cast<T>(m_assets[name]);
	}

private:
	std::unordered_map<std::string, std::shared_ptr<void>> m_assets;
};
#pragma once
#include <unordered_map>

template<typename K>
class MultiMap
{
public:
	MultiMap(size_t size = 0)
	{
		if (size == 0)
		{
			size = 32;
		}
		m_buildOffset = 0;
		m_data = new unsigned char[size];
		m_size = size;
	}

	~MultiMap()
	{
		delete[] m_data;
	}

	bool Contains(K key)
	{
		return m_valid.contains(key) && m_valid[key];
	}

	template <typename T>
	T* Get(K key)
	{
		unsigned int ptr = m_offsets[key];
		return (T*) & m_data[ptr];
	}

	template <typename T>
	void Set(K key, T value)
	{
		if (m_offsets.contains(key))
		{
			unsigned int ptr = m_offsets[key];
			memcpy(m_data + ptr, &value, sizeof(T));
		}
		else
		{
			if (sizeof(unsigned char) * m_buildOffset + sizeof(T) >= m_size)
			{
				Resize(sizeof(T));
			}

			m_offsets[key] = m_buildOffset;
			memcpy(&m_data[m_buildOffset], &value, sizeof(T));
			m_buildOffset += sizeof(T);
		}
	}

	void Erase(K key)
	{
		m_valid[key] = false;
	}

	template<typename T>
	std::vector<T*> GetData() 
	{ 
		std::vector<T*> ptr;
		for (std::pair<K, unsigned int> pair : m_offsets)
		{
			if (m_valid[pair.first]) ptr.push_back((T*)&m_data[pair.second]);
		}

		return ptr;
	}

private:
	void Resize(size_t required)
	{
		size_t newSize = (m_size + required) * 1.5;
		unsigned char* newData = new unsigned char[newSize];
		memcpy(newData, m_data, sizeof(unsigned char) * m_size);
		
		delete[] m_data;
		m_data = newData;
		m_size = newSize;
	}

	std::unordered_map<K, unsigned int> m_offsets;
	std::unordered_map<K, bool> m_valid;
	unsigned char* m_data;
	size_t m_size;
	unsigned int m_buildOffset;
};
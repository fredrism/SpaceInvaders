#pragma once
#include <unordered_map>

template <typename K, typename V>
class bimap
{
public:
	bimap() {};
	~bimap() {};

	bool contains_key(K key)
	{
		return m_kvMap.contains(key);
	}

	bool contains_val(V val)
	{
		return m_vkMap.contains(val);
	}

	void erase_key(K key)
	{
		V val = m_kvMap[key];

		m_kvMap.erase(key);
		m_vkMap.erase(val);
	}

	void erase_val(V val)
	{
		K key = m_kvMap[key];

		m_kvMap.erase(key);
		m_vkMap.erase(val);
	}

	V get_val(K key) { return m_kvMap[key]; }
	K get_key(V val) { return m_vkMap[val]; }

	void set(K key, V val)
	{
		m_kvMap[key] = val;
		m_vkMap[val] = key;
	}

	bool empty()
	{
		return m_kvMap.empty();
	}

private:
	std::unordered_map<K, V> m_kvMap;
	std::unordered_map<V, K> m_vkMap;
};
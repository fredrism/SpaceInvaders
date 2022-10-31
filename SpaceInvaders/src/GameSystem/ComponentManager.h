#pragma once
#include "ComponentArray.h"

class ComponentManager
{
public:
	ComponentManager()
	{
		
	}

	template<typename T>
	void RegisterComponent()
	{
		size_t hash = typeid(T).hash_code();
		m_buffers[hash] = {};
		m_buffers[hash].Initialize<T>(16);

		m_componentTypes[hash] = 1 << m_componentTypeCounter++;
	}

	template<typename T>
	void AddComponent(EntityID entity, T& component)
	{
		size_t hash = typeid(T).hash_code();
		if (!m_componentTypes.contains(hash))
		{
			RegisterComponent<T>();
		}

		GetBuffer<T>().Insert<T>(entity, component);
	}

	void Duplicate(EntityID oldEntity, EntityID newID)
	{
		for (auto& buffer : m_buffers)
		{
			buffer.second.Duplicate(oldEntity, newID);
		}
	}

	template<typename T>
	void RemoveComponent(EntityID entity)
	{
		GetBuffer<T>().Remove<T>(entity);
	}

	template<typename T>
	T& GetComponent(EntityID entity)
	{
		size_t hash = typeid(T).hash_code();
		T& data = m_buffers[hash].GetComponent<T>(entity);
		return data;
	}

	template<typename T>
	ComponentArray& GetBuffer()
	{
		return m_buffers[typeid(T).hash_code()];
	}

	void EntityDestroyed(EntityID entity)
	{
		for (auto& buffer : m_buffers)
		{
			buffer.second.Remove(entity);
		}
	}

	template<typename T>
	ComponentFlags GetFlag()
	{
		size_t hash = typeid(T).hash_code();
		if (!m_componentTypes.contains(hash))
		{
			RegisterComponent<T>();
		}
		return m_componentTypes[hash];
	}

	template<typename T> 
	ComponentArray::iterator<T> Begin()
	{
		size_t hash = typeid(T).hash_code();
		return m_buffers[hash].Begin<T>();
	}

	template<typename T> 
	ComponentArray::iterator<T> End()
	{
		size_t hash = typeid(T).hash_code();
		return m_buffers[hash].End<T>();
	}



private:
	std::unordered_map<size_t, ComponentArray> m_buffers;
	std::unordered_map<size_t, ComponentFlags> m_componentTypes;

	unsigned int m_componentTypeCounter = 0;
};
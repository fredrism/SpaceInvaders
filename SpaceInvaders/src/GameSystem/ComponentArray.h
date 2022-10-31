#pragma once
#include <vector>
#include <typeinfo>
#include <functional>
#include <unordered_map>
#include "ECSCommon.h"
#include <assert.h>

typedef unsigned char byte;

struct ComponentInfo
{
	size_t size;
	size_t hash;
	std::string name;
};

class ComponentArray
{
public:
	template<typename T>
	struct iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		iterator(pointer ptr) : m_ptr(ptr) {}

		reference operator*() const { return *m_ptr; }
		pointer operator->() { return m_ptr; }

		iterator& operator++() { m_ptr++; return *this; }
		iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
		iterator operator+(int val) { iterator tmp = *this; tmp.m_ptr += val; return tmp; }

		friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; };

	private:
		pointer m_ptr;
	};

	~ComponentArray()
	{
		std::cout << "Component Array Destructor : " << m_info.name << std::endl;
		for (size_t offset = 0; offset < m_currentSize; offset += m_info.size)
		{
			m_destructor(&m_data[offset]);
		}

		delete[] m_data;
	}

	template<typename T>
	void Initialize(unsigned int startingSize = 1)
	{
		m_info.hash = typeid(T).hash_code();
		m_info.name = typeid(T).name();
		m_info.size = sizeof(T);

		m_currentSize = 0;
		m_size = m_info.size * startingSize;
		m_data = new byte[m_size * startingSize];

		m_destructor = [](unsigned char* elem) {
			T* ptr = reinterpret_cast<T*>(elem);
			ptr->~T();
			int i = 0; };
	
		/*
		m_constructor = [](unsigned int* elem) {
			new (elem)T();
		};*/

		std::memset(m_data, 0, m_size);
	}

	void Resize(size_t newSize)
	{
		std::cout << m_info.name << " Resized" << std::endl;
		byte* newData = new byte[newSize];
		std::memset(newData, 0, newSize);
		std::memcpy(newData, m_data, m_size);
		
		delete[] m_data;
		m_data = newData;
		m_size = newSize;
	}

	template<typename T>
	void Insert(EntityID entity, T& data)
	{
		size_t index = m_currentSize;
		size_t newSize = m_currentSize + m_info.size;

		if (newSize >= m_size) Resize(m_size * 4);

		m_entityMap[entity] = index;
		m_offsetMap[index] = entity;

		T* ptr = reinterpret_cast<T*>(&m_data[index]);
		std::memset(ptr, 0, m_info.size);					//Ensure shared_ptr is copied correctly
		
		*ptr = data;

		m_currentSize = newSize;
	}

	void Duplicate(EntityID oldEntity, EntityID newID)
	{
		if(!m_entityMap.contains(oldEntity)) return;

		size_t index = m_currentSize;
		size_t newSize = m_currentSize + m_info.size;

		if (newSize >= m_size) Resize(m_size * 4);

		m_entityMap[newID] = index;
		m_offsetMap[index] = newID;

		size_t oldIndex = m_entityMap[oldEntity];
		std::copy(&m_data + oldIndex, &m_data + oldIndex + m_info.size, &m_data + index);

		m_currentSize = newSize;
	}

	void Remove(EntityID entity)
	{
		if (!m_entityMap.contains(entity)) return;

		size_t removedIndex = m_entityMap[entity];
		size_t lastIndex = m_currentSize - m_info.size;
		m_currentSize -= m_info.size;

		if (removedIndex == lastIndex)
		{
			m_destructor(&m_data[removedIndex]);
			m_entityMap.erase(entity);
			m_offsetMap.erase(lastIndex);
			
			return;
		}

		EntityID lastEntity = m_offsetMap[lastIndex];

		m_destructor(&m_data[removedIndex]);
		std::memcpy(m_data + removedIndex, m_data + lastIndex, m_info.size);

		m_entityMap.erase(entity);
		m_offsetMap.erase(lastIndex);

		m_offsetMap[removedIndex] = lastEntity;
		m_entityMap[lastEntity] = removedIndex;
	}

	template<typename T>
	T& GetComponent(EntityID entity)
	{
		assert(m_entityMap.contains(entity) && "Entity does not exist in component array!");
		size_t index = m_entityMap[entity];
		T* object = reinterpret_cast<T*>(m_data + index);
		return *object;
	}

	template<typename T>
	iterator<T> Begin() const
	{
		return iterator(reinterpret_cast<T*>(m_data));
	}

	template<typename T>
	iterator<T> End() const
	{
		return iterator(reinterpret_cast<T*>(m_data + m_currentSize));
	}

	unsigned int Length() const { return m_entityMap.size(); }


private:
	ComponentInfo m_info;
	size_t m_currentSize;
	size_t m_size;
	std::function<void(unsigned char*)> m_destructor;
	std::function<void(unsigned char*)> m_constructor;
	byte* m_data;

	std::unordered_map<EntityID, size_t> m_entityMap;
	std::unordered_map<size_t, EntityID> m_offsetMap;
};
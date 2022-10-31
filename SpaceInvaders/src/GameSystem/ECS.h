#pragma once
#include <memory>

#include "ComponentManager.h"
#include "ECSSystem.h"
#include "ECSMessage.h"
#include "Components/Physics/CollisionInfo.h"
#include "Prefab/EntityPrefab.h"
#include <queue>
#include <mutex>
#include <stack>

class ECS
{
public:
	ECS()
	{
		for (EntityID id = 0; id < MAX_ENTITIES; id++)
		{
			m_availableIDs.push(MAX_ENTITIES - id - 1);
		}
	}

	EntityID CreateEntity()
	{
		EntityID entity = m_availableIDs.top();
		m_availableIDs.pop();
		m_entityMap[entity] = 0;

		return entity;
	}

	template<typename... Ts>
	EntityID CreateEntity(EntityPrefab<Ts...>& prefab)
	{
		EntityID entity = CreateEntity();
		((AddComponent<Ts>(entity, std::get<Ts>(prefab.components))), ...);

		std::cout << "Entity Created : " << entity << std::endl;
		return entity;
	}

	EntityID Duplicate(EntityID entity)
	{
		EntityID newEntity = CreateEntity();
		m_componentManager.Duplicate(entity, newEntity);
	}

	void DestroyEntity(EntityID entity)
	{
		m_entityMap.erase(entity);
		m_componentManager.EntityDestroyed(entity);
		m_availableIDs.push(entity);

		for (auto ECSSystem : m_ECSSystems)
		{
			ECSSystem->EntityDestroyed(entity);
		}

		std::cout << "Entity Destroyed : " << entity << std::endl;
	}

	template<typename T>
	void AddComponent(EntityID entity, T& component)
	{
		m_componentManager.AddComponent<T>(entity, component);
		ComponentFlags components = m_entityMap[entity] | m_componentManager.GetFlag<T>();
		m_entityMap[entity] = components;

		for (auto ECSSystem : m_ECSSystems)
		{
			ECSSystem->EntityChanged(entity, components);
		}
	}

	template<typename T>
	void RemoveComponent(EntityID entity)
	{
		m_componentManager.RemoveComponent<T>(entity);

		ComponentFlags components = m_entityMap[entity] & ~m_componentManager.GetFlag<T>();
		m_entityMap[entity] = components;

		for (auto ECSSystem : m_ECSSystems)
		{
			ECSSystem->EntityChanged(entity);
		}
	}

	void SendMessage(ECSMessage& message)
	{
		//std::lock_guard lock(m_messageLock);
		if (m_messageQueue.size() >= MAX_MESSAGES)
		{
			std::cout << "Message Cap Reached!" << std::endl;
			return;
		}
		m_messageQueue.push(message);
	}

	template<typename T>
	T& GetComponent(EntityID entity)
	{
		return m_componentManager.GetComponent<T>(entity);
	}

	template<typename T>
	bool QueryComponent(EntityID entity, T** component)
	{
		if (entity == 1)
		{
			int i = 0;
		}
		if (MatchFlags(m_entityMap[entity], m_componentManager.GetFlag<T>()))
		{
			*component = &m_componentManager.GetComponent<T>(entity);
			return true;
		}

		return false;
	}

	void Update(float dt)
	{
		for (auto ECSSystem : m_ECSSystems)
		{
			ECSSystem->Update(dt);
		}

		std::scoped_lock lock(m_messageLock);
		while (!m_messageQueue.empty())
		{
			HandleMessage(m_messageQueue.front());
			m_messageQueue.pop();
		}
	}

	template<typename S, typename... Ts>
	void RegisterECSSystem()
	{
		std::shared_ptr<ECSSystem> system = std::make_shared<S>();

		for (auto& entity : m_entityMap)
		{
			system->EntityChanged(entity.first, entity.second);
		}

		ComponentFlags flags;
		((flags |= m_componentManager.GetFlag<Ts>()), ...);
		
		system->SetComponentFlags(flags);
		system->SetECS(this);

		m_ECSSystems.push_back(system);
		m_SystemFlags[typeid(S).hash_code()] = flags;
	}

	template<typename T>
	ComponentArray::iterator<T> ComponentArrayBegin()
	{
		return m_componentManager.Begin<T>();
	}

	template<typename T>
	ComponentArray::iterator<T> ComponentArrayEnd()
	{
		return m_componentManager.End<T>();
	}

	template<typename T>
	unsigned int ComponentArrayLength() const
	{
		return m_componentManager.GetBuffer<T>().second.Length();
	}

	template<typename T>
	ComponentFlags GetSystemFlags() const
	{
		return m_SystemFlags[typeid(T).hash_code];
	}

	template<typename T>
	ComponentFlags GetComponentFlag() const
	{
		return m_componentManager.GetFlag<T>();
	}

	void Clear()
	{
		for (auto entity : m_entityMap)
		{
			ECSMessage msg;
			msg.type = ECSMessageType::Destroyed;
			msg.entity = entity.first;
			SendMessage(msg);
		}

		while (!m_messageQueue.empty())
		{
			HandleMessage(m_messageQueue.front());
			m_messageQueue.pop();
		}
	}
	
	template<typename T>
	T* FindSystem()
	{
		for (auto& system : m_ECSSystems)
		{
			if (typeid(*system.get()) == typeid(T))
			{
				return (T*)system.get();
			}
		}

		return nullptr;
	}

private:
	void HandleMessage(ECSMessage& message)
	{
		if (message.type == ECSMessageType::Callback || message.type == ECSMessageType::Created)
		{
			message.callback();
		}
		else if (message.type == ECSMessageType::Destroyed)
		{
			DestroyEntity(message.entity);
		}
		else if (message.type == ECSMessageType::Collision)
		{
			auto info = (CollisionInfo*)message.data;
			//std::cout << info->first << " : " << info->second << std::endl;
			for (auto& system : m_ECSSystems)
			{
				if ((system->GetComponentFlags() | m_entityMap[info->first]) == m_entityMap[info->first])
				{
					system->OnCollision(info->first, info->second, info->penetration);
				}
				if ((system->GetComponentFlags() | m_entityMap[info->second]) == m_entityMap[info->second])
				{
					system->OnCollision(info->second, info->first, info->penetration);
				}
			}
		}
		else 
		{
			for (auto& system : m_ECSSystems)
			{
				if (MatchFlags(system->GetComponentFlags(), message.mask) || message.mask == 0)
				{
					system->MessageProc(message);
				}
			}
		}
	}
	const unsigned int MAX_MESSAGES = 5000;

	std::vector<std::shared_ptr<ECSSystem>> m_ECSSystems;
	std::unordered_map<size_t, ComponentFlags> m_SystemFlags;
	std::mutex m_messageLock;

	std::unordered_map<EntityID, ComponentFlags> m_entityMap;
	std::queue<ECSMessage> m_messageQueue;
	std::stack<EntityID> m_availableIDs;
	ComponentManager m_componentManager;
};
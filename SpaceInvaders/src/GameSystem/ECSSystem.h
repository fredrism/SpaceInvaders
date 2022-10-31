#pragma once
#include "ECSCommon.h"
#include "ECSMessage.h"
#include <set>

class ECS;

class ECSSystem
{
public:
	ECSSystem() : m_ecs(nullptr) {}
	~ECSSystem()
	{
		for (auto entity : m_entities)
		{
			OnDestroy(entity);
		}
	}

	virtual void Update(float dt) = 0;

	virtual void OnCreate(EntityID entity) {};
	virtual void OnDestroy(EntityID entity) {};
	virtual void OnMessage(ECSMessage& message) {};
	virtual void OnCollision(EntityID first, EntityID second, Vec3f penetration) {};

	void MessageProc(ECSMessage& message) 
	{
		switch (message.type)
		{
		default:
			OnMessage(message);
			break;
		}
	};

	void EntityDestroyed(EntityID entity)
	{
		if (m_entities.contains(entity))
		{
			OnDestroy(entity);
			m_entities.erase(entity);
		}
		
	}

	void EntityChanged(EntityID entity, ComponentFlags flags)
	{
		if ((flags & m_components) == m_components && !m_entities.contains(entity))
		{
			m_entities.insert(entity);
			OnCreate(entity);
		}
		else if ((flags & m_components) != m_components && m_entities.contains(entity))
		{
			m_entities.erase(entity);
			OnDestroy(entity);
		}
	}

	void SetComponentFlags(ComponentFlags& flags)
	{
		m_components = flags;
	}

	ComponentFlags GetComponentFlags() const { return m_components; }

	void SetECS(ECS* ecs)
	{
		m_ecs = ecs;
	}

	const std::set<EntityID>& GetEntities() { return m_entities; }

protected:
	ComponentFlags m_components;
	ECS* m_ecs;
	std::set<EntityID> m_entities;
};
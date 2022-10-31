#pragma once
#include "Components.h"


struct Transform
{
	Vec3f position;
	float rotation;
	Vec3f scale;
	Matrix4f matrix;
	Matrix4f invmatrix;
};

struct Sprite
{
	std::shared_ptr<Texture2D> texture = nullptr;
	Vec4f tint = Vec4f(1,1,1,1);
};

class TransformSystem : public ECSSystem
{
public:
	virtual void Update(float dt)
	{
		//SI_SCOPE_PROFILE("Transform Update");
		auto begin = m_ecs->ComponentArrayBegin<Transform>();
		auto end = m_ecs->ComponentArrayEnd<Transform>();
	


		for(auto it = begin; it != end; it++)
		{
			auto& transform = *it;

			Vec3f rotation(0, 0, SIMath::Deg2Rad(transform.rotation));
			transform.matrix.SetTRS(transform.position, rotation, transform.scale);
			transform.invmatrix = transform.matrix;
			transform.invmatrix.Invert();
		}
	}
};

class SpriteSystem : public ECSSystem
{
	virtual void Update(float dt)
	{
		//SI_SCOPE_PROFILE("Sprite Update");
		for (auto& entity : m_entities)
		{
			auto& transform = m_ecs->GetComponent<Transform>(entity);
			auto& sprite = m_ecs->GetComponent<Sprite>(entity);
			
			SystemManager::Graphics()->Submit({ sprite.texture, &sprite.tint, &transform.matrix, transform.position.z});
		}
		
	}
};
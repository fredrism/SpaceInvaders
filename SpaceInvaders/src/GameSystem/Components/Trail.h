#pragma once
#include "Components.h"
#include "Transform.h"

struct Trail
{
	Vec4f color;
	float width;
	std::array<Vec3f, 8> points;
};


class TrailSystem : public ECSSystem
{
public:
	std::shared_ptr<Material> trailMaterial;
	std::shared_ptr<LineGeometry> lineGeometry;
	std::vector<vertex_pos_col> vertices;

	TrailSystem()
	{
		auto texture = TextureLoader::LoadTexture("resources/sprites/trail.png");
		auto shader = ShaderLoader::Load("resources/shaders/thick_line.glsl");
		trailMaterial = std::make_shared<Material>(shader, false);
		trailMaterial->SetTexture("u_texture", texture);
		lineGeometry = std::make_shared<LineGeometry>();
	}

	virtual void OnCreate(EntityID entity)
	{
		auto& trail = m_ecs->GetComponent<Trail>(entity);
		auto& transform = m_ecs->GetComponent<Transform>(entity);

		trail.points[0] = Vec3f(-10, 0, 0);
	}

	virtual void OnDestroy(EntityID entity)
	{

	}

	virtual void OnMessage(ECSMessage& message)
	{

	}

	virtual void Update(float dt)
	{
		
		for (auto entity : m_entities)
		{
			vertices.clear();
			auto& trail = m_ecs->GetComponent<Trail>(entity);
			auto& transform = m_ecs->GetComponent<Transform>(entity);

			//SystemManager::Gizmo()->DrawLine(trail.start, trail.end, Vec3f(1, 0, 0));
			if (trail.points[0].x < -5)
			{
				for (int i = 0; i < 8; i++)
				{
					trail.points[i] = transform.position;
				}
			}

			for (int i = 6; i >= 0; i--)
			{
				Vec3f AB = trail.points[i + 1] - trail.points[i];

				float dist = AB.Magnitude();
				if (dist > 0.1)
				{
					trail.points[i] = trail.points[i] + AB.normalized() * (dist - 0.1);
				}
			}

			trail.points[7] = transform.position;

			vertices.push_back({ trail.points[0], Vec3f(0,0, 0) });
			for (int i = 1; i < 7; i++)
			{
				vertices.push_back({ trail.points[i], Vec3f(0,(float)i / 7.0, 0)});
				vertices.push_back({ trail.points[i], Vec3f(0,(float)i / 7.0, 0) });
			}
			vertices.push_back({ trail.points[7], Vec3f(0,1, 0) });

			trailMaterial->SetProperty<Vec4f>("u_color", trail.color);
			trailMaterial->SetProperty<float>("u_width", trail.width);

			if (vertices.size() > 0)
			{
				lineGeometry->SetVertices(vertices);
				SystemManager::Graphics()->DrawThickline(lineGeometry.get(), trailMaterial.get());
			}
		}
	}
};
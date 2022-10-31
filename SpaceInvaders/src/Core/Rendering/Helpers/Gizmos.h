#pragma once
#include "../VMath/VecXX.h"
#include "../Resources/ShaderLoader.h"
#include "../Material.h"
#include "../Geometry/LineGeometry.h"
#include "../Renderer.h"
#include "../../System.h"
#include "../VMath/SIMath.h"
#include <mutex>

class Gizmos : public System
{
public:
	Gizmos(std::shared_ptr<Renderer> renderer);
	~Gizmos();

	virtual void BeginFrame() override;
	virtual void EndFrame() override;
	void DrawLine(Vec3f start, Vec3f end, Vec3f color);
	void DrawCircle(Vec3f center, float radius, Vec3f color);
	void DrawBox(Vec2f size, Matrix4f& matrix, Vec3f color);
	void DrawBox(Vec2f min, Vec2f max, Vec3f color);
private:
	std::vector<vertex_pos_col> m_lines;
	std::shared_ptr<Renderer> m_renderer;
	std::shared_ptr<Material> m_lineMaterial;
	LineGeometry m_geometry;
	std::mutex drawMutex;
};


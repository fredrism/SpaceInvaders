#pragma once
#include "../Transform.h"
#include "CollisionInfo.h"
#include "../../SystemManager.h"
#include "AABB.h"

enum ColliderShape
{
	Circle, Polygon
};

struct Collider
{
	AABB boundingBox;

	float radius;					//we have to store parameters for every collider type 
	std::vector<Vec2f> polygon;

	ColliderShape shape;
};

template<unsigned int i>
inline Collider PolygonCollider(std::array<Vec2f, i> points)
{
	return { { Vec3f(0,0,0), Vec3f(0,0,0) }, 0, std::vector<Vec2f>(points.begin(), points.end()), ColliderShape::Polygon};
}

inline Collider CircleCollider(float radius)
{
	return { { Vec3f(0,0,0), Vec3f(0,0,0) }, radius, { Vec2f(0,0)}, ColliderShape::Circle};
}

inline void ComputeAABB(Collider& collider, Transform& transform)
{
	AABB& aabb = collider.boundingBox;

	if (collider.shape == ColliderShape::Polygon)
	{
		aabb.min.x = 999999999;
		aabb.min.y = 999999999;
		aabb.max.x = -999999999;
		aabb.max.y = -999999999;

		for (Vec2f& point : collider.polygon)
		{
			Vec3f p = point;
			p = transform.matrix.MulPoint(p);

			aabb.min.x = std::fminf(p.x, aabb.min.x);
			aabb.min.y = std::fminf(p.y, aabb.min.y);
			aabb.max.x = std::fmaxf(p.x, aabb.max.x);
			aabb.max.y = std::fmaxf(p.y, aabb.max.y);
		}
		
	}
	else if (collider.shape == ColliderShape::Circle)
	{
		aabb.min.x = transform.position.x - collider.radius;
		aabb.min.y = transform.position.y - collider.radius;
		aabb.max.x = transform.position.x + collider.radius;
		aabb.max.y = transform.position.y + collider.radius;
	}
}

namespace CollisionTest
{
	inline bool CircleCircleTest(Collider& c_a, Collider& c_b, Transform& t_a, Transform& t_b, CollisionInfo& info)
	{
		info.penetration = ((Vec2f)t_b.position - (Vec2f)t_a.position).normalized();
		float depth = (c_a.radius + c_b.radius) - Vec2f::Distance((Vec2f)t_a.position, (Vec2f)t_b.position);
		info.penetration *= depth;
		return depth > 0;
	}

	inline bool SideOfLine(Vec2f start, Vec2f end, Vec2f point)
	{
		Vec2f AB = end - start;
		Vec2f AC = point - start;
		return Vec2f::Dot(Vec2f(AB.y, AB.x), AC) > 0;
	}

	inline bool LineSegmentIntersect(Vec2f A, Vec2f B, Vec2f C, Vec2f D, Vec2f& result)
	{
		float div = ((D.y - C.y) * (B.x - A.x) - (D.x - C.x) * (B.y - A.y));

		float t = ((D.x - C.x) * (A.y - C.y) - (D.y - C.y) * (A.x, -C.x)) / div;

		result = A + (B - A) * t;
		return (0 <= t && t <= 1);
	}

	inline bool CircleLineIntersect(Vec2f center, float radius, Vec2f A, Vec2f B, Vec2f& result)
	{
		float t = ((center.x - A.x) * (B.x - A.x) + (center.y - A.y) * (B.y - A.y)) / (B - A).SqrMagnitude();
		Vec2f closest = A + (B - A) * t;
		Vec2f dir = closest - center;

		float length = dir.Magnitude();
		result = dir.normalized() * (length - radius);

		return ((length - radius) < 0) && (0 < t &&  t < 1);
	}
	
	inline bool CirclePolyIntersect(Collider& c_a, Collider& c_b, Transform& t_a, Transform& t_b, CollisionInfo& info)
	{
		bool hit = false;
		Vec2f tmp = Vec2f(99999, 99999);
		Vec2f penetration = Vec2f(99999, 99999);

		for (unsigned int i = 0; i < c_b.polygon.size() - 1; i++)
		{
			Vec3f A = c_b.polygon[i];
			A = t_b.matrix.MulPoint(A);

			Vec3f B = c_b.polygon[i + 1];
			B = t_b.matrix.MulPoint(B);

			if (CircleLineIntersect(t_a.position, c_a.radius, A, B, tmp))
			{ 
 				if (tmp.SqrMagnitude() < penetration.SqrMagnitude()) penetration = tmp;
				hit = true;
			}
		}
		if (!hit)
		{
			for (unsigned int i = 0; i < c_b.polygon.size(); i++)
			{
				Vec3f A = c_b.polygon[i];
				A = t_b.matrix.MulPoint(A);

				if (Vec3f::Distance(A, t_a.position) < c_a.radius)
				{
					hit = true;
					penetration = A - t_a.position;
					break;
				}
			}
		}

		info.penetration = penetration;
		return hit;
	}
	

	inline bool Intersects(Collider& c_a, Collider& c_b, Transform& t_a, Transform& t_b, CollisionInfo& info)
	{
		//if (!AABBTest(c_a.boundingBox, c_b.boundingBox)) return false;
		if (c_a.shape == ColliderShape::Circle && c_b.shape == ColliderShape::Circle) return CircleCircleTest(c_a, c_b, t_a, t_b, info);
		if (c_a.shape == ColliderShape::Circle && c_b.shape == ColliderShape::Polygon) return CirclePolyIntersect(c_a, c_b, t_a, t_b, info);
		if (c_a.shape == ColliderShape::Polygon && c_b.shape == ColliderShape::Circle) return CirclePolyIntersect(c_b, c_a, t_b, t_a, info);
		if (c_a.shape == ColliderShape::Polygon && c_b.shape == ColliderShape::Polygon) return false;
		return false;
	}
}
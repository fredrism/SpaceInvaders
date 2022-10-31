#pragma once
#include "VecXX.h"
#include <sstream>

class Matrix4f
{
public:
	Matrix4f()
	{
		for (int i = 0; i < 16; i++)
		{
			values[i] = 0;
		}
	};

	~Matrix4f() {};

	void SetTRS(Vec3f& translation, Vec3f& rotation, Vec3f& scale)
	{
		Matrix4f t;

		t.Set(0, 3, translation.x);
		t.Set(1, 3, translation.y);
		t.Set(2, 3, translation.z);

		t.Set(0, 0, 1);
		t.Set(1, 1, 1);
		t.Set(2, 2, 1);
		t.Set(3, 3, 1);

		Matrix4f r;
		float sx = std::sinf(rotation.x);
		float cx = std::cosf(rotation.x);

		float sy = std::sinf(rotation.y);
		float cy = std::cosf(rotation.y);

		float sz = std::sinf(rotation.z);
		float cz = std::cosf(rotation.z);

		r.SetColumn(Vec4f((cy * cz),
			(cx * sz) + (sx * sy * cz),
			(sx * sz) - (cx * sy * cz),
			0), 0);

		r.SetColumn(Vec4f(-(cy * sz),
			(cx * cz) - (sx * sy * sz),
			(sx * cz) + (cx * sy * sz),
			0), 1);

		r.SetColumn(Vec4f((sy),
			-(sx * cy),
			(cx * cy),
			0), 2);

		r.SetColumn(Vec4f(0, 0, 0, 1), 3);

		Matrix4f s;

		s.Set(0, 0, scale.x);
		s.Set(1, 1, scale.y);
		s.Set(2, 2, scale.z);
		s.Set(3, 3, 1);

		Matrix4f result = Multiply(s, r);
		*this = Multiply(result, t);
	};

	void SetRow(Vec4f values, int row)
	{
		Set(row, 0, values.x);
		Set(row, 1, values.y);
		Set(row, 2, values.z);
		Set(row, 3, values.w);
	};

	void SetColumn(Vec4f values, int column)
	{
		Set(0, column, values.x);
		Set(1, column, values.y);
		Set(2, column, values.z);
		Set(3, column, values.w);
	};

	constexpr float Get(int r, int c)
	{
		return values[r + 4 * c];
	};

	void Set(int r, int c, float value)
	{
		values[r + 4 * c] = value;
	};

	static Matrix4f Multiply(Matrix4f& a, Matrix4f& b)
	{
		Matrix4f m;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float sum = 0;

				for (int k = 0; k < 4; k++)
				{
					sum += b.Get(i, k) * a.Get(k, j);
				}

				m.Set(i, j, sum);
			}
		}

		return m;
	};

	void Invert()
	{
		Matrix4f temp;

		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				temp.Set(i, j, Get(j, i));
			}
		}
	};

	Vec4f operator *(Vec3f v)
	{
		Vec4f r(
			v.x * Get(0, 0) + v.y * Get(0, 1) + v.z * Get(0, 2) + 1 * Get(0, 3),
			v.x * Get(1, 0) + v.y * Get(1, 1) + v.z * Get(1, 2) + 1 * Get(1, 3),
			v.x * Get(2, 0) + v.y * Get(2, 1) + v.z * Get(2, 2) + 1 * Get(2, 3),
			v.x * Get(3, 0) + v.y * Get(3, 1) + v.z * Get(3, 2) + 1 * Get(3, 3)
		);

		r.x /= r.w;
		r.y /= r.w;
		r.z /= r.w;
		r.w = 1;

		return r;
	};

	Vec4f MulPoint(Vec3f& v)
	{
		Vec4f r(
			v.x * Get(0, 0) + v.y * Get(0, 1) + v.z * Get(0, 2) + 1 * Get(0, 3),
			v.x * Get(1, 0) + v.y * Get(1, 1) + v.z * Get(1, 2) + 1 * Get(1, 3),
			v.x * Get(2, 0) + v.y * Get(2, 1) + v.z * Get(2, 2) + 1 * Get(2, 3),
			v.x * Get(3, 0) + v.y * Get(3, 1) + v.z * Get(3, 2) + 1 * Get(3, 3)
		);

		r.x /= r.w;
		r.y /= r.w;
		r.z /= r.w;
		r.w = 1;

		return r;
	}

	std::string ToString()
	{
		std::stringstream ss;
		for (int i = 0; i < 4; i++)
		{
			ss << "\n";
			for (int j = 0; j < 4; j++)
			{
				ss << values[4 * i + j] << ", ";
			}
		}

		ss << "\n";

		return ss.str();
	}

private:
	float values[16];
};
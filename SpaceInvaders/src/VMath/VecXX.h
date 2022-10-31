#pragma once
#include <string>
#include <sstream>
#include "VXA.h"

//templated Vector-math class that can generate any length(2-4) glsl-compatible vector with an arithmetic type

template<Arithmetic T, VXX U>
struct VecXX : public U
{
	static constexpr unsigned int width() { return sizeof(U) / sizeof(T); }

	//_________________________________________________________________________________________________________________________________________________________________________________________CONSTRUCTORS
	// Constructors constrained such that they only accept the correct number of arguments (eg. Vec2f will accept 2 floats, but not 3)
	VecXX(T _x, T _y) requires(sizeof(U)/sizeof(T) == 2)
	{
		U::data[0] = _x;
		U::data[1] = _y;
	}

	VecXX(T _x, T _y, T _z) requires(sizeof(U) / sizeof(T) == 3)
	{
		U::data[0] = _x;
		U::data[1] = _y;
		U::data[2] = _z;
	}

	VecXX(T _x, T _y, T _z, T _w) requires(sizeof(U) / sizeof(T) == 4)
	{
		U::data[0] = _x;
		U::data[1] = _y;
		U::data[2] = _z;
		U::data[3] = _w;
	}

	VecXX()
	{
		for (unsigned int i = 0; i < width(); i++)
		{
			U::data[i] = 0;
		}
	}

	VecXX(const VecXX& v)
	{
		memcpy(this, &v, sizeof(U));
	}
	
	VecXX(VecXX&& v) noexcept
	{
		memcpy(this, &v, sizeof(U));
	} 

	~VecXX() { }

	//_________________________________________________________________________________________________________________________________________________________________________________________OPERATORS

	VecXX operator +(const VecXX& v) const
	{
		VecXX result;

		for (unsigned int i = 0; i < width(); i++)
		{
			result.data[i] = U::data[i] + v.data[i];
		}

		return result;
	}

	VecXX operator -(const VecXX& v) const
	{
		VecXX result;

		for (unsigned int i = 0; i < width(); i++)
		{
			result.data[i] = U::data[i] - v.data[i];
		}

		return result;
	}

	VecXX operator *(float v) const
	{
		VecXX result;

		for (unsigned int i = 0; i < width(); i++)
		{
			result.data[i] = U::data[i] * v;
		}

		return result;
	}

	VecXX operator /(float v) const
	{
		VecXX result;

		for (unsigned int i = 0; i < width(); i++)
		{
			result.data[i] = U::data[i] / v;
		}

		return result;
	}
	
	VecXX& operator =(VecXX&& v) noexcept
	{
		memcpy(this, &v, sizeof(U));

		return *this;
	};

	VecXX& operator =(VecXX& v)
	{
		memcpy(this, &v, sizeof(U));

		return *this;
	};

	VecXX& operator *=(float v)
	{
		for (unsigned int i = 0; i < width(); i++)
		{
			U::data[i] *= v;
		}

		return *this;
	}

	VecXX& operator +=(VecXX v)
	{
		for (unsigned int i = 0; i < width(); i++)
		{
			U::data[i] += v.data[i];
		}

		return *this;
	}

	VecXX& operator -=(VecXX v)
	{
		for (unsigned int i = 0; i < width(); i++)
		{
			U::data[i] -= v.data[i];
		}

		return *this;
	}

	//_________________________________________________________________________________________________________________________________________________________________________________________MATH

	float Magnitude() const
	{
		return std::sqrt(SqrMagnitude());
	}

	float SqrMagnitude() const
	{
		return Dot(*this, *this);
	}

	void Normalize()
	{
		float m = Magnitude();

		for (unsigned int i = 0; i < width(); i++)
		{
			U::data[i] /= m;
		}
	}

	VecXX normalized() const
	{
		return *this / Magnitude();
	}

	static float Dot(const VecXX& a, const VecXX& b)
	{
		float f = 0;

		for (unsigned int i = 0; i < width(); i++)
		{
			f += a.data[i] * b.data[i];
		}

		return f;
	}

	static float Distance(const VecXX& a, const VecXX& b)
	{
		VecXX c = a - b;
		return c.Magnitude();
	}

	static float SqrDistance(const VecXX& a, const VecXX& b)
	{
		VecXX c = a - b;
		return c.SqrMagnitude();
	}

	static VecXX Lerp(const VecXX& a, const VecXX& b, float t)
	{
		VecXX v;

		for (unsigned int i = 0; i < width(); i++)
		{
			v.data[i] += (1 - t) * a.data[i] + t * b.data[i];
		}

		return v;
	}

	//_________________________________________________________________________________________________________________________________________________________________________________________CONVERSION

	std::string ToString()
	{
		std::stringstream ss;

		ss << "(";

		for (unsigned int i = 0; i < width(); i++)
		{
			ss << U::data[i];
			if (i != width() - 1)
			{
				ss << ", ";
			}
		}

		ss << ")";
		return ss.str();
	}
	
	template<Arithmetic X, VXX Y>		//Conversion operator between vector-types
	operator VecXX<X, Y>()
	{
		VecXX<X, Y> v;
		unsigned int v_width = sizeof(Y) / sizeof(X);

		int min = std::min(width(), v_width);

		for (unsigned int i = 0; i < min; i++)
		{
			v.data[i] = static_cast<X>(U::data[i]);
		}

		return v;
	}
};

typedef VecXX<float, V2A<float>> Vec2f;
typedef VecXX<float, V3A<float>> Vec3f;
typedef VecXX<float, V4A<float>> Vec4f;

typedef VecXX<double, V2A<double>> Vec2d;
typedef VecXX<double, V3A<double>> Vec3d;
typedef VecXX<double, V4A<double>> Vec4d;

typedef VecXX<unsigned char, V2A<unsigned char>> Vec2uc;
typedef VecXX<unsigned char, V3A<unsigned char>> Vec3uc;
typedef VecXX<unsigned char, V4A<unsigned char>> Vec4uc;

typedef VecXX<int, V2A<int>> Vec2i;
typedef VecXX<int, V3A<int>> Vec3i;
typedef VecXX<int, V4A<int>> Vec4i;
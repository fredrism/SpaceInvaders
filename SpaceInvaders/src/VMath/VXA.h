#pragma once
#include "Arithmetic.h"

template<typename T>
concept VXX = requires
{
	T::data;
};

template<Arithmetic T>
struct V2A
{
	union 
	{
		struct {
			union { T x; T r; T u; };
			union { T y; T g; T v; };
		};
		T data[2];
	};
};

template<Arithmetic T>
struct V3A
{
	union
	{
		struct {
			union { T x; T r; T u; };
			union { T y; T g; T v; };
			union { T z; T b; };
		};
		T data[3];
	};
};

template<Arithmetic T>
struct V4A
{
	union
	{
		struct {
			union { T x; T r; T u; };
			union { T y; T g; T v; };
			union { T z; T b; };
			union { T w; T a; };
		};
		T data[4];
	};
};
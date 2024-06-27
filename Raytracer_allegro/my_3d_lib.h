#pragma once
#include <cmath>


template<class T>
class Color
{
	T r, g, b;
public:
	Color(T red, T green, T blue)
	{
		r = red; g = green; b = blue;
	}
};


template<class T>
class vec2
{
public:
	T x = 0;
	T y = 0;

	vec2()
	{}

	vec2(T X, T Y)
	{
		x = X;
		y = Y;
	}
	vec2(T XY_ASSIGNMENT)
	{
		x = XY_ASSIGNMENT;
		y = XY_ASSIGNMENT;
	}
};

template<class T>
class vec3
{
public:
	T x = 0;
	T y = 0;
	T z = 0;
	vec3() {}
	vec3(T X, T Y,T Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	vec3(T XYZ_ASSIGNMENT)
	{
		x = XYZ_ASSIGNMENT;
		y = XYZ_ASSIGNMENT;
		z = XYZ_ASSIGNMENT;
	}
};

template<class T>
inline vec3<T> get_vector_from_angle(vec2<T> pitch_yaw)
{
	T x, y, z;
	
	x = std::sin(pitch_yaw.y) * std::cos(pitch_yaw.x);
	y = std::sin(pitch_yaw.x);
	z = std::cos(pitch_yaw.y) * std::cos(pitch_yaw.x);

	return vec3<T>(x,y,z);
}

class Camera
{
public:
	vec3<float> POSITION;
	vec2<float> PITCH_YAW;
	vec2<unsigned char> FOV;

	Camera(vec3<float>& position,vec2<float>& pitch_yaw, vec2<unsigned char>& fov_xy)
	{
		POSITION = position;
		PITCH_YAW = pitch_yaw;
		FOV = fov_xy;
	}
};

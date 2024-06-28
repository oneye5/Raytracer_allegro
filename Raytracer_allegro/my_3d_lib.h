#pragma once
#include <cmath>
#include <vector>
#include <algorithm>


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
template<class T>
inline T distnce_between(vec3<T> p1, vec3<T> p2)
{
	return std::sqrtf
	(
		(p1.x - p2.x) * (p1.x - p2.x) +
		(p1.y - p2.y) * (p1.y - p2.y) +
		(p1.z - p2.z) * (p1.z - p2.z) 
	);
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
class Mesh 
{
public:
	Mesh(std::vector<vec3<float>> vertex_buffer_,std::vector<vec2<float>> uv_buffer_,std::vector<vec3<float>> normal_buffer_,std::vector<unsigned int> index_buffer_)
	{
		vertex_buffer = vertex_buffer_;
		uv_buffer = uv_buffer_;
		normal_buffer = normal_buffer_;
		index_buffer = index_buffer_;
	}
	Mesh() {}

	std::vector<vec3<float>> vertex_buffer;
	std::vector<vec2<float>> uv_buffer;
	std::vector<vec3<float>> normal_buffer;
	std::vector<unsigned int> index_buffer; //in the format of [[vi,uv,n],[vi,uv,n],[vi,uv,n]]

	static Mesh get_sample_mesh() 
	{
		std::vector<vec3<float>> vertex_buffer = {
	        vec3<float>(-0.5f, -0.5f, 0.0f), // Bottom-left
	        vec3<float>(0.5f, -0.5f, 0.0f),  // Bottom-right
	        vec3<float>(0.5f, 0.5f, 0.0f),   // Top-right
	        vec3<float>(-0.5f, 0.5f, 0.0f)   // Top-left
	    };
		std::vector<vec2<float>> uv_buffer = {
	        vec2<float>(0.0f, 0.0f), // Bottom-left
	        vec2<float>(1.0f, 0.0f), // Bottom-right
	        vec2<float>(1.0f, 1.0f), // Top-right
	        vec2<float>(0.0f, 1.0f)  // Top-left
	    };
		std::vector<vec3<float>> normal_buffer = {
	        vec3<float>(0.0f, 0.0f, 1.0f), // Bottom-left
	        vec3<float>(0.0f, 0.0f, 1.0f), // Bottom-right
	        vec3<float>(0.0f, 0.0f, 1.0f), // Top-right
	        vec3<float>(0.0f, 0.0f, 1.0f)  // Top-left
	    };
	    // Define indices for two triangles that make up the square
	    // Each group of three values represents vertex_index, uv_index, normal_index
		std::vector<unsigned int> index_buffer = {
	        0, 0, 0,  // First triangle: Bottom-left
	        1, 1, 1,  // First triangle: Bottom-right
	        2, 2, 2,  // First triangle: Top-right
	
	        2, 2, 2,  // Second triangle: Top-righte
	        3, 3, 3,  // Second triangle: Top-left
	        0, 0, 0   // Second triangle: Bottom-left
	    };
	

		return Mesh(vertex_buffer,uv_buffer,normal_buffer,index_buffer);
	}
	void sort_tris_by_depth(const vec3<float>& camera_position)
	{
		std::vector<std::pair<float,std::vector<unsigned int>>> tris;
		tris.reserve(index_buffer.size()/9);
		for (int i = 0; i < index_buffer.size(); i += 9)
		{
			unsigned int v1_index = index_buffer[i];
			unsigned int v2_index = index_buffer[i + 3];
			unsigned int v3_index = index_buffer[i + 6];

			vec3<float>& v1 = vertex_buffer[v1_index];
			vec3<float>& v2 = vertex_buffer[v2_index];
			vec3<float>& v3 = vertex_buffer[v3_index];

			float averge_depth =
				(
					distnce_between(v1, camera_position) +
					distnce_between(v2, camera_position) +
					distnce_between(v3, camera_position) 
				) / 3.0f;

			tris.push_back({ averge_depth, {
				  index_buffer[i], index_buffer[i + 1], index_buffer[i + 2],
				  index_buffer[i + 3], index_buffer[i + 4], index_buffer[i + 5],
				  index_buffer[i + 6], index_buffer[i + 7], index_buffer[i + 8]
			  } });
		}



		std::sort(tris.begin(), tris.end(),
			[](const auto& a, const auto& b) { return a.first > b.first; });

		// Update the index buffer
		index_buffer.clear();
		for (const auto& triangle : tris) {
			index_buffer.insert(index_buffer.end(), triangle.second.begin(), triangle.second.end());
		}
	}

};
class Scene
{
public:
	std::vector<Mesh> geometry;
};

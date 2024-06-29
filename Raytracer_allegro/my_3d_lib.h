#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
/*
	this file contains all smaller helper data structures and functions relating to
	the renderer that dont deserve to have their own file.
*/

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

	static inline vec3<T> subtract(const vec3<T>& v1, const vec3<T>& v2)
	{
		return vec3<T>{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	}
	static inline vec3<T> add(const vec3<T>& v1, const vec3<T>& v2)
	{
		return vec3<T>{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	}
	static inline vec3<T> cross_product(const vec3<T>& v1, const vec3<T>& v2)
	{
		return vec3<T>
		{
			v1.y* v2.z - v1.z * v2.y,
			v1.z* v2.x - v1.x * v2.z,
			v1.x* v2.y - v1.y * v2.x
		};
	}
	static inline T dot_product(const vec3<T>& v1, const vec3<T>& v2)
	{
		return
			v1.x * v2.x +
			v1.y * v2.y +
			v1.x * v2.z;
	}
	static inline T multiply(const vec3<T>& v1, const T& f)
	{
		return vec3<T>
		{
			v1.x* f,
			v1.y* f,
			v1.z* f
		};
	}
};

// gets a direction vector from a pitch and yaw value
template<class T>
inline vec3<T> get_vector_from_angle(vec2<T> pitch_yaw)
{
	T x, y, z;
	
	x = std::sin(pitch_yaw.y) * std::cos(pitch_yaw.x);
	y = std::sin(pitch_yaw.x);
	z = std::cos(pitch_yaw.y) * std::cos(pitch_yaw.x);

	return vec3<T>(x,y,z);
}
// returns the distance between two points
template<class T>
inline T distance_between(vec3<T> p1, vec3<T> p2)
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
					distance_between(v1, camera_position) +
					distance_between(v2, camera_position) +
					distance_between(v3, camera_position) 
				) / 3.0f;

			tris.push_back({ averge_depth, {
				  index_buffer[i], index_buffer[i + 1], index_buffer[i + 2],
				  index_buffer[i + 3], index_buffer[i + 4], index_buffer[i + 5],
				  index_buffer[i + 6], index_buffer[i + 7], index_buffer[i + 8]
			  } });
		}



		std::sort(tris.begin(), tris.end(),[](const auto& a, const auto& b) { return a.first > b.first; });

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

/*
* used for wrapping triangle information, to allow for flexibility in geometry implementation / format
*/
template <class VEC_TYPE, class UV_TYPE, class NORM_TYPE> 
struct triangle_info
{
	vec3<VEC_TYPE>* vertices[3];
	vec2<UV_TYPE>*	uv[3];
	vec3<NORM_TYPE>* normals[3];
};

// used in ray_info, for avoiding un-needed memory allocation however we must be careful to avoid memory leaks
template <class VEC_TYPE>
struct hit_info
{
	VEC_TYPE DISTANCE;
	vec3<VEC_TYPE> HIT_POINT_WORLDSPACE;
	vec2<VEC_TYPE> HIT_POINT_UVSPACE;
};
// returned by ray_intersect_tri()
template <class VEC_TYPE>
class ray_info
{
public:
	bool hit;
	hit_info<VEC_TYPE>* hit_result;

	ray_info(bool t, hit_info<VEC_TYPE>* hitInfo)
	{
		hit = t; 
		hit_result = hitInfo;
	}
	ray_info(bool f)
	{
		if(f == true)
			std::cout << "\nin ray_info(bool f), this constructor should only be called when the result is false, because hit information about the hit must be provided otherwise\n";
		hit = f;
	}
	~ray_info()
	{
		if (hit_result != nullptr)
			delete hit_result;
	}
};

//checks if a ray intersects with a triangle. (uses the moller trumbore algorithm)
template <class VEC_TYPE, class UV_TYPE, class NORM_TYPE>
ray_info<VEC_TYPE> ray_intersect_tri(const vec3<float>& ray_origin, const vec3<float>& ray_dir, const triangle_info<VEC_TYPE, UV_TYPE, NORM_TYPE>& tri)
{
	const float moe = 0.000001;

	vec3<VEC_TYPE> edge_1 = vec3<VEC_TYPE>::subtract(tri.vertices[1], tri.vertices[0]);
	vec3<VEC_TYPE> edge_2 = vec3<VEC_TYPE>::subtract(tri.vertices[2], tri.vertices[0]);

	vec3<VEC_TYPE> dir_edge_cross = vec3<VEC_TYPE>::cross_product(ray_dir, edge_2);
	VEC_TYPE cross_edge_dot = vec3<VEC_TYPE>::dot_product(edge_1, dir_edge_cross);

	if (cross_edge_dot > -moe && cross_edge_dot < moe) //ray parallel to triangle plane, it will never intersect so we can return early
		return ray_info{ false };

	VEC_TYPE inverse_cross_dot = 1.0 / cross_edge_dot;
	vec3<VEC_TYPE> origin_tri_dif = vec3<VEC_TYPE>::subtract(ray_origin, tri.vertices[0]);
	VEC_TYPE pos_dif_cross = vec3<VEC_TYPE>::cross_product(origin_tri_dif, edge_1);


	VEC_TYPE intersect_u = inverse_cross_dot * vec3<VEC_TYPE>::dot_product(origin_tri_dif, dir_edge_cross);
	VEC_TYPE intersect_v = inverse_cross_dot * vec3<VEC_TYPE>::dot_product(ray_dir, pos_dif_cross);
	VEC_TYPE dist_traveled = inverse_cross_dot * vec3<VEC_TYPE>::dot_product(edge_2, pos_dif_cross);

	if (intersect_u < 0.0 || intersect_u > 1.0)//--------------- point lies outside the bounds along the edge of the first fertex to the seccond, so we can return early
		return ray_info{ false };

	if (intersect_v < 0.0 || intersect_u + intersect_v > 1.0)//- point lies outside or past the third edge
		return ray_info{ false };

	if (dist_traveled < moe)//---------------------------------- ray does not travel far enough from origin to count as a valid hit
		return ray_info{ false };

	// we have a confirmed hit, now lets calculate where exactly this happened in world space
	vec3<VEC_TYPE> world_pos = vec3<VEC_TYPE>::add
	(
		vec3<VEC_TYPE>::multiply(ray_dir, dist_traveled),
		ray_origin
	);

	return ray_info
	{
		true,
		new hit_info
		{
			dist_traveled,
			world_pos,
			vec2<VEC_TYPE>
			{
				intersect_u,
				intersect_v
			}
		}
	};
}
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

template<class VERTEX_TYPE>
class vec3
{
public:
	VERTEX_TYPE x = 0;
	VERTEX_TYPE y = 0;
	VERTEX_TYPE z = 0;
	vec3() {}
	vec3(VERTEX_TYPE X, VERTEX_TYPE Y, VERTEX_TYPE Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	vec3(VERTEX_TYPE XYZ_ASSIGNMENT)
	{
		x = XYZ_ASSIGNMENT;
		y = XYZ_ASSIGNMENT;
		z = XYZ_ASSIGNMENT;
	}

	static inline vec3<VERTEX_TYPE> subtract(const vec3<VERTEX_TYPE>& v1, const vec3<VERTEX_TYPE>& v2)
	{
		return vec3<VERTEX_TYPE>{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	}
	static inline vec3<VERTEX_TYPE> add(const vec3<VERTEX_TYPE>& v1, const vec3<VERTEX_TYPE>& v2)
	{
		return vec3<VERTEX_TYPE>{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	}
	static inline vec3<VERTEX_TYPE> cross_product(const vec3<VERTEX_TYPE>& v1, const vec3<VERTEX_TYPE>& v2)
	{
		return vec3<VERTEX_TYPE>
		{
			v1.y* v2.z - v1.z * v2.y,
			v1.z* v2.x - v1.x * v2.z,
			v1.x* v2.y - v1.y * v2.x
		};
	}
	static inline VERTEX_TYPE dot_product(const vec3<VERTEX_TYPE>& v1, const vec3<VERTEX_TYPE>& v2)
	{
		return
			v1.x * v2.x +
			v1.y * v2.y +
			v1.x * v2.z;
	}
	static inline VERTEX_TYPE multiply(const vec3<VERTEX_TYPE>& v1, const VERTEX_TYPE& f)
	{
		return vec3<VERTEX_TYPE>
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

template<class VERTEX_TYPE,class NORMAL_TYPE, class UV_TYPE>
class Mesh 
{
public:
	Mesh(std::vector<vec3<VERTEX_TYPE>> vertex_buffer_,
		 std::vector<vec3<NORMAL_TYPE>> normal_buffer_, 
		 std::vector<vec2<UV_TYPE>> uv_buffer_, 
		 std::vector<unsigned int> index_buffer_)
	{
		vertex_buffer = vertex_buffer_;
		uv_buffer = uv_buffer_;
		normal_buffer = normal_buffer_;
		index_buffer = index_buffer_;
	}
	Mesh() {}

	std::vector<vec3<VERTEX_TYPE>> vertex_buffer;
	std::vector<vec3<NORMAL_TYPE>> normal_buffer;
	std::vector<vec2<UV_TYPE>> uv_buffer;

	std::vector<unsigned int> index_buffer; //in the format of [[vi,n,uv],[vi,n,uv],[vi,n,uv]]

	static Mesh get_sample_mesh()
	{
		std::vector<vec3<VERTEX_TYPE>> vertex_buffer = {
			vec3<VERTEX_TYPE>(-0.5, -0.5, 0.0), // Bottom-left
			vec3<VERTEX_TYPE>(0.5, -0.5, 0.0),  // Bottom-right
			vec3<VERTEX_TYPE>(0.5, 0.5, 0.0),   // Top-right
			vec3<VERTEX_TYPE>(-0.5, 0.5, 0.0)   // Top-left
		};

		std::vector<vec3<NORMAL_TYPE>> normal_buffer = {
			vec3<NORMAL_TYPE>(0.0, 0.0, 1.0), // Bottom-left
			vec3<NORMAL_TYPE>(0.0, 0.0, 1.0), // Bottom-right
			vec3<NORMAL_TYPE>(0.0, 0.0, 1.0), // Top-right
			vec3<NORMAL_TYPE>(0.0, 0.0, 1.0)  // Top-left
		};

		std::vector<vec2<UV_TYPE>> uv_buffer = {
			vec2<UV_TYPE>(0.0, 0.0), // Bottom-left
			vec2<UV_TYPE>(1.0, 0.0), // Bottom-right
			vec2<UV_TYPE>(1.0, 1.0), // Top-right
			vec2<UV_TYPE>(0.0, 1.0)  // Top-left
		};

		std::vector<unsigned int> index_buffer = {
			0, 0, 0, // First triangle: Bottom-left
			1, 1, 1, // First triangle: Bottom-right
			2, 2, 2, // First triangle: Top-right
			2, 2, 2, // Second triangle: Top-right
			3, 3, 3,  // Second triangle: Top-left
			3, 3, 3
		};

		return Mesh{ vertex_buffer, normal_buffer, uv_buffer, index_buffer };
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
template<class VERTEX_TYPE, class NORMAL_TYPE, class UV_TYPE>
class Scene
{
public:
	std::vector<Mesh<VERTEX_TYPE, NORMAL_TYPE, UV_TYPE>> geometry;
};

/*
* used for wrapping triangle information, to allow for flexibility in geometry implementation / format
*/
template <class VERTEX_TYPE, class NORMAL_TYPE, class UV_TYPE>
struct triangle_info
{
	vec3<VERTEX_TYPE>** vertices; // array of size 3 containing pointers to the actual values
	vec3<NORMAL_TYPE>** normals;
	vec2<UV_TYPE>**		uv;
};

// used in ray_info, for avoiding un-needed memory allocation however we must be careful to avoid memory leaks
template <class VERTEX_TYPE>
struct hit_info
{
	VERTEX_TYPE DISTANCE;
	vec3<VERTEX_TYPE> HIT_POINT_WORLDSPACE;
	vec2<VERTEX_TYPE> HIT_POINT_UVSPACE;
};
// returned by ray_intersect_tri()
template <class VERTEX_TYPE>
class ray_info
{
public:
	bool hit;
	hit_info<VERTEX_TYPE>* hit_result;

	ray_info(bool t, hit_info<VERTEX_TYPE>* hitInfo)
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
	ray_info() {}
	~ray_info()
	{
		if (hit_result != nullptr)
			delete hit_result;
	}
};

//checks if a ray intersects with a triangle. (uses the moller trumbore algorithm)
template <class VERTEX_TYPE, class NORMAL_TYPE, class UV_TYPE>
ray_info<VERTEX_TYPE> ray_intersect_tri(const vec3<float>& ray_origin, const vec3<float>& ray_dir, const triangle_info<VERTEX_TYPE, UV_TYPE, NORMAL_TYPE>& tri)
{
	const float moe = 0.000001;

	vec3<VERTEX_TYPE> edge_1 = vec3<VERTEX_TYPE>::subtract(tri.vertices[1], tri.vertices[0]);
	vec3<VERTEX_TYPE> edge_2 = vec3<VERTEX_TYPE>::subtract(tri.vertices[2], tri.vertices[0]);

	vec3<VERTEX_TYPE> dir_edge_cross = vec3<VERTEX_TYPE>::cross_product(ray_dir, edge_2);
	VERTEX_TYPE cross_edge_dot = vec3<VERTEX_TYPE>::dot_product(edge_1, dir_edge_cross);

	if (cross_edge_dot > -moe && cross_edge_dot < moe) //ray parallel to triangle plane, it will never intersect so we can return early
		return ray_info{ false };

	VERTEX_TYPE inverse_cross_dot = 1.0 / cross_edge_dot;
	vec3<VERTEX_TYPE> origin_tri_dif = vec3<VERTEX_TYPE>::subtract(ray_origin, tri.vertices[0]);
	VERTEX_TYPE pos_dif_cross = vec3<VERTEX_TYPE>::cross_product(origin_tri_dif, edge_1);


	VERTEX_TYPE intersect_u = inverse_cross_dot * vec3<VERTEX_TYPE>::dot_product(origin_tri_dif, dir_edge_cross);
	VERTEX_TYPE intersect_v = inverse_cross_dot * vec3<VERTEX_TYPE>::dot_product(ray_dir, pos_dif_cross);
	VERTEX_TYPE dist_traveled = inverse_cross_dot * vec3<VERTEX_TYPE>::dot_product(edge_2, pos_dif_cross);

	if (intersect_u < 0.0 || intersect_u > 1.0)//--------------- point lies outside the bounds along the edge of the first fertex to the seccond, so we can return early
		return ray_info{ false };

	if (intersect_v < 0.0 || intersect_u + intersect_v > 1.0)//- point lies outside or past the third edge
		return ray_info{ false };

	if (dist_traveled < moe)//---------------------------------- ray does not travel far enough from origin to count as a valid hit
		return ray_info{ false };

	// we have a confirmed hit, now lets calculate where exactly this happened in world space
	vec3<VERTEX_TYPE> world_pos = vec3<VERTEX_TYPE>::add
	(
		vec3<VERTEX_TYPE>::multiply(ray_dir, dist_traveled),
		ray_origin
	);

	return ray_info
	{
		true,
		new hit_info
		{
			dist_traveled,
			world_pos,
			vec2<VERTEX_TYPE>
			{
				intersect_u,
				intersect_v
			}
		}
	};
}
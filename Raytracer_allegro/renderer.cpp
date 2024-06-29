#include "renderer.h"
#include <iostream>


void Renderer::render_frame()
{
	//steps through every pixel of the screen then calculates the color for that pixel
	for (unsigned short y = 0; y < SCREEN_H; y++)
	{
		for (unsigned short x = 0; x < SCREEN_W; x++)
		{
			auto angle = calculate_ray_vector(x, y);
			auto color = compute_color(camera->POSITION,angle);
		}
	}
	std::cout << "done render";
}

Color<COLOR_TYPE> Renderer::compute_color(const vec3<VERTEX_TYPE>& position,const vec3<float> dir_vector)
{
	
	Color<COLOR_TYPE> c(1.0f, 2.0f, 3.0f);
	return c;
}

ray_info<VERTEX_TYPE> Renderer::ray_intersect_scene(const vec3<float>& ray_origin, const vec3<float> ray_dir, Mesh<VERTEX_TYPE,NORMAL_TYPE,UV_TYPE>* hit_mesh)
{
	ray_info<VERTEX_TYPE> hit;
	triangle_info<VERTEX_TYPE, UV_TYPE, NORMAL_TYPE> tri;
	for (int i = 0; i < scene.geometry.size(); i++)
	{
		Mesh<VERTEX_TYPE, NORMAL_TYPE, UV_TYPE>& mesh = scene.geometry.at(i);
		for (int j = 0; j < mesh.index_buffer.size(); j += 9)
		{
			//get pointers to the data that make up the tri, for use by the raycaster
			vec3<VERTEX_TYPE>* verticies[] =
			{
				&mesh.vertex_buffer.at(mesh.index_buffer.at(j)),
				&mesh.vertex_buffer.at(mesh.index_buffer.at(j + 3)),
				&mesh.vertex_buffer.at(mesh.index_buffer.at(j + 6))
			};

			vec3<NORMAL_TYPE>* normals[] =
			{
				&mesh.normal_buffer.at(mesh.index_buffer.at(j + 1)),
				&mesh.normal_buffer.at(mesh.index_buffer.at(j + 4)),
				&mesh.normal_buffer.at(mesh.index_buffer.at(j + 7))
			};

			vec2<UV_TYPE>* uvs[] =
			{
				&mesh.uv_buffer.at(mesh.index_buffer.at(j + 2)),
				&mesh.uv_buffer.at(mesh.index_buffer.at(j + 5)),
				&mesh.uv_buffer.at(mesh.index_buffer.at(j + 8))
			};

			triangle_info<VERTEX_TYPE, UV_TYPE, NORMAL_TYPE> tri
			{
				verticies,
				normals,
				uvs
			};


			hit = ray_intersect_tri(ray_origin, ray_dir, tri);
			if (!hit.hit)
				continue;

			//if hit 
			hit_mesh = &mesh;
			goto _break;
			
		}
	}
	_break:
	return hit;
}


vec3<float> Renderer::calculate_ray_vector(const unsigned short &screen_x, const unsigned short &screen_y)
{
	float rad_pitch;
	float rad_yaw;

	//angle between two adjacent pixels, based off fov
	float fov_step_x = (float)camera->FOV.x/ (float)SCREEN_W; 
	float fov_step_y = (float)camera->FOV.y / (float)SCREEN_H;

	//calculate the angle from screenspace coords
	rad_pitch = fov_step_y * ((float)screen_y - ((float)SCREEN_H / 2.0f));
	rad_yaw = fov_step_x * ((float)screen_x - ((float)SCREEN_W / 2.0f));
	
	rad_pitch += camera->PITCH_YAW.x;
	rad_yaw += camera->PITCH_YAW.y;

	return get_vector_from_angle(vec2<float>(rad_pitch, rad_yaw));
}


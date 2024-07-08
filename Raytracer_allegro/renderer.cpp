#include "renderer.h"
#include <iostream>
#include <allegro5/allegro.h>

void Renderer::render_frame()
{
	//steps through every pixel of the screen then calculates the color for that pixel
	vec3<float> t = get_vector_from_angle(camera.PITCH_YAW);
	std::cout << "\ncamera = " << camera.PITCH_YAW.x << " " << camera.PITCH_YAW.y << " " << camera.FOV.x << " " << camera.FOV.y;
	std::cout << "\nstart angle = " << t.x << " " << t.y << " " << t.z;
	for (unsigned short y = 0; y < SCREEN_H; y++)
	{
		for (unsigned short x = 0; x < SCREEN_W; x++)
		{
			auto angle = calculate_ray_vector(x, y);
			auto color = compute_color(camera.POSITION,angle);
			if (!color.r)
				continue;

			
			auto new_color = ALLEGRO_COLOR{ color.r,color.g,color.b};
			al_put_pixel(x, y, new_color);
		}
		//std::cout << y << "\n";
	}
	std::cout << "done render";
}

Color<COLOR_TYPE> Renderer::compute_color(const vec3<VERTEX_TYPE>& position,const vec3<float> dir_vector)
{
	Mesh<VERTEX_TYPE,NORMAL_TYPE,UV_TYPE, COLOR_TYPE> hit_mesh;
	auto info = ray_intersect_scene(position, dir_vector, &hit_mesh);
	Color<COLOR_TYPE> c(0.0f, 0.0f, 0.0f);

	if(info.hit)
		c = Color<COLOR_TYPE>(1,1,1);

	return c;
}

ray_info<VERTEX_TYPE> Renderer::ray_intersect_scene(const vec3<float>& ray_origin, const vec3<float> ray_dir, Mesh<VERTEX_TYPE,NORMAL_TYPE,UV_TYPE, COLOR_TYPE>* hit_mesh)
{
	for (int i = 0; i < scene.geometry.size(); i++)
	{
		Mesh<VERTEX_TYPE, NORMAL_TYPE, UV_TYPE, COLOR_TYPE>& mesh = scene.geometry.at(i);
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


			auto hit = ray_intersect_tri<VERTEX_TYPE,NORMAL_TYPE,UV_TYPE>(ray_origin, ray_dir, tri);
			
			if (!hit.hit)
				continue;

			hit_mesh = &mesh;
			return hit;
		}
	}
}


vec3<float> Renderer::calculate_ray_vector(const unsigned short &screen_x, const unsigned short &screen_y)
{
	float fov_step_x = camera.FOV.x / (float)SCREEN_W; 
	float fov_step_y = camera.FOV.y / (float)SCREEN_H;

	//calculate the angle from screenspace coords
	float offset_adjusted_x = ((float)screen_x - ((float)SCREEN_W / 2.0f));
	float offset_adjusted_y = ((float)screen_y - ((float)SCREEN_H / 2.0f));
	float rad_pitch = fov_step_y * offset_adjusted_y;
	float rad_yaw = fov_step_x * offset_adjusted_x;
	
	rad_pitch += camera.PITCH_YAW.x;
	rad_yaw += camera.PITCH_YAW.y;

	return get_vector_from_angle(vec2<float>(rad_pitch, rad_yaw));
}


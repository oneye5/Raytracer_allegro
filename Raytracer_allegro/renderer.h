#pragma once
#include "my_3d_lib.h"

#ifndef COMPILE_TIME_RENDERER_SETTINGS 
#include "compile_time_render_settings.h" //this defines what types to use. for example #define VERTEX_TYPE float
#endif 

#define DEG_TO_RAD(x) ((x) * (0.0174533))
class Renderer
{
private:
	// calculate the ray direction vector from screenspace co-ords as well as the camera angle
	vec3<float> calculate_ray_vector(const unsigned short& screen_x, const unsigned short& screen_y);
	Scene<VERTEX_TYPE,NORMAL_TYPE,UV_TYPE, COLOR_TYPE> scene;
public:
	unsigned short	SCREEN_W, SCREEN_H;
	Camera camera;

	//calls compute_color() with the appropriate angle and position for each pixel on the screen
	void render_frame();

	// the primary method for calculating a pixels color, does so in a recursive manor 
	Color<COLOR_TYPE> compute_color(const vec3<float>& position, const vec3<float> dir_vector);

	// uses ray_intersect_tri but iterates over all meshses and triangles in the scene until its deemed unlikley or certain that the ray does not intersect with anything
	ray_info<VERTEX_TYPE> ray_intersect_scene(const vec3<float>& ray_origin, const vec3<float> ray_dir, Mesh<VERTEX_TYPE, NORMAL_TYPE, UV_TYPE, COLOR_TYPE>* hit_mesh);




	Renderer(const unsigned short screen_width, const unsigned short screen_height)
	{
		SCREEN_W = screen_width;
		SCREEN_H = screen_height;

		//initialize camera with default values
		vec3<float> pos{0.0f,-0.0f,-2.0f};
		vec2<float> rot(DEG_TO_RAD(0.0f), DEG_TO_RAD(0.0f));
		vec2<float> fov_xy(DEG_TO_RAD(90.0), DEG_TO_RAD(90.0));

		camera = Camera(pos,rot,fov_xy);

		scene.geometry.push_back(Mesh<VERTEX_TYPE,NORMAL_TYPE,UV_TYPE, COLOR_TYPE>::get_sample_mesh());
		scene.geometry.at(0).sort_tris_by_depth(camera.POSITION);
	}
	Renderer()
	{}
	~Renderer()
	{
		
	}
};
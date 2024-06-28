#pragma once
#include "my_3d_lib.h"

#define COLOR_DATA_TYPE float
#define DEG_TO_RAD(x) ((x) * (0.0174533))
class Renderer
{
private:
	vec3<float> calculate_ray_vector(const unsigned short& screen_x, const unsigned short& screen_y);
	Scene scene;
public:
	unsigned short	SCREEN_W, SCREEN_H;
	Camera* camera;

	//calls compute_color() with the appropriate angle and position for each pixel on the screen
	void render_frame();

	//the primary method for calculating a pixels color, does so in a recursive manor 
	Color<COLOR_DATA_TYPE> compute_color(const vec3<float>& position, const vec3<float> dir_vector);



	Renderer(const unsigned short screen_width, const unsigned short screen_height)
	{
		SCREEN_W = screen_width;
		SCREEN_H = screen_height;

		//initialize camera with default values
		vec3<float> pos;
		vec2<float> rot(90.0f,0.0f);
		vec2<unsigned char> fov_xy(DEG_TO_RAD(90), DEG_TO_RAD(90));

		camera = new Camera(pos,rot,fov_xy);

		scene.geometry.push_back(Mesh::get_sample_mesh());
		scene.geometry.at(0).sort_tris_by_depth(camera->POSITION);
	}
	~Renderer()
	{
		delete camera;
	}
};
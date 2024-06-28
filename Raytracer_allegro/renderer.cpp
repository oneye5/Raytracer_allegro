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

Color<COLOR_DATA_TYPE> Renderer::compute_color(const vec3<float>& position,const vec3<float> dir_vector)
{


	Color<COLOR_DATA_TYPE> c(1.0f, 2.0f, 3.0f);
	return c;
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


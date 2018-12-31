#include <iostream>
#include <fstream>
#include <float.h>
#include <random>
#include "material.h"
#include "camera.h"
#include "sphere.h"
#include "hitable_list.h"
#include <SDL.h>

math::vec color(raycaster::ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec)) {
		raycaster::ray scattered;
		math::vec attenuation;

		if (depth < 5 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation*color(scattered, world, depth + 1);
		}
		else
		{
			return math::vec(0.f, 0.f, 0.f);
		}
	}
	else {
		math::vec unit_direction = r.direction();
		unit_direction.make_unit();
		float t = 0.5f * (unit_direction[1] + 1.0f);
		return math::vec(1.0f, 1.0f, 1.0f)*(1.0f - t) + math::vec(0.5f, 0.7f, 1.f) * t;
	}
}

hitable *random_scene(int balls) {
	int n = balls;
	hitable **list = new hitable*[n*n*n];
	//list[0] = new sphere(math::vec(0, -1000, 0), 1000, new lambertian(math::vec(0.5, 0.5, 0.5)));
	int i = 0;
	/*for (int a = -balls/2; a < balls/2; a++) {
			float choose_mat = drand48();
			float x_pos = drand48() * 10 - 5;
			float y_pos = drand48() * 10 - 5;
			math::vec center(x_pos + 0.9*drand48(), 0.2, y_pos + 0.9*drand48());
			if (choose_mat < 0.8) {  // diffuse
				list[i++] = new sphere(center, 1.f * drand48(), new lambertian(math::vec(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
			}
			else if (choose_mat < 0.95) { // metal
				list[i++] = new sphere(center, 1.f * drand48(),
					new metal(math::vec(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*drand48()));
			}
			else {  // glass
				list[i++] = new sphere(center, 2.f * drand48(), new dielectric(1.5));
			}
	}*/

	for (int a = -balls / 2; a < balls / 2; a++) {
		for (int b = -balls / 2; b < balls / 2; b++) {
			for (int c = -balls / 2; c < balls / 2; c++) {
				math::vec center(a, b, c);
				if (b % 3 == 0)
				{
					list[i++] = new sphere(center, 0.4f,
						//new metal(math::vec(0.5f, 0.5f, 0.5f), 0.1));
						//new dielectric(1.5));
						new lambertian(math::vec(0, 0, 0)));
				}
				else if (b % 3 == 1 || b % 3 == -2)
				{
					list[i++] = new sphere(center, 0.4f,
						//new metal(math::vec(0.5f, 0.5f, 0.5f), 0.1));
						new dielectric(1.5));
						//new lambertian(math::vec(0, 0, 0)));
				}
				else
				{
					list[i++] = new sphere(center, 0.4f,
						//new metal(math::vec(0.5f, 0.5f, 0.5f), 0.1));
						//new dielectric(1.5));
						new lambertian(math::vec(1.f, 0, 0)));

				}
			}
		}
	}

	return new hitable_list(list, i);
}

inline void set_pixel(int x, int y, int ns, int nx, int ny, camera cam, hitable* world, unsigned char* r, unsigned char* g, unsigned char* b)
{
	math::vec col(0, 0, 0);
	for (int s = 0; s < ns; s++)
	{
		float u = float(x + drand48() - 0.5f) / float(nx);
		float v = float(y + drand48() - 0.5f) / float(ny);
		raycaster::ray r = cam.get_ray(u, v);
		math::vec p = r.point(2.0f);
		col = col + color(r, world, 0);
	}
	col = col / float(ns);
	col = math::vec(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
	col[0] = int(255.99*col[0]);
	col[1] = int(255.99*col[1]);
	col[2] = int(255.99*col[2]);
	r[x + nx*y] = col[0];
	g[x + nx*y] = col[1];
	b[x + nx*y] = col[2];
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, 800, 600,
		SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	int nx = 800;
	int ny = 600;
	int ns = 200;
	unsigned char* r = new unsigned char[800 * 600];
	unsigned char* g = new unsigned char[800 * 600];
	unsigned char* b = new unsigned char[800 * 600];
	hitable **list;
	hitable *world;
	world = random_scene(6);
	
	unsigned char* pixels = new unsigned char[800 * 600 * 3];

	SDL_Surface *surface_bmp = SDL_CreateRGBSurfaceFrom((void*)pixels,
		800,
		600,
		3 * 8,          // bits per pixel = 24
		800 * 3,  // pitch
		0x0000FF,              // red mask
		0x00FF00,              // green mask
		0xFF0000,              // blue mask
		0);                    // alpha mask (none)

	SDL_Surface *surface_window = SDL_GetWindowSurface(window);


	float x = 13.0f;
	while (true)
	{
		math::vec lookfrom(21, -8, 11);
		math::vec lookat(0, 0, 0);
		float dist_to_focus = (lookfrom - lookat).mag();
		float aperture = 0.1f;
		camera cam(lookfrom, lookat, math::vec(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);
		for (int j = ny - 1; j >= 0; j--) {
			for (int i = 0; i < nx; i++) {
				set_pixel(i, j, ns, nx, ny, cam, world, r, g, b);
			}
		}
		int j = 800 * 600;
		for (int i = 0; i < 800 * 600 * 3; i += 3) {
			pixels[i] = r[j];
			pixels[i + 1] = g[j];
			pixels[i + 2] = b[j];
			j--;
		}
		SDL_BlitSurface(surface_bmp,
			NULL,
			surface_window,
			NULL);
		SDL_UpdateWindowSurface(window);
		//x += 5.f;
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
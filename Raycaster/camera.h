#pragma once
#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#define M_PI 3.14

math::vec random_in_unit_disk() {
	math::vec p;
	do {
		p = math::vec(drand48(), drand48(), 0) * 2 - math::vec(1, 1, 0);
	} while (p.dot(p) >= 1.0);
	return p;
}

class camera {
public:
	camera(math::vec lookfrom, math::vec lookat, math::vec vup, float vfov, float aspect, float aperture, float focus_dist) {
		lens_radius = aperture / 2;
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta/2);
		float half_width = aspect*half_height;
		origin = lookfrom;
		w = lookfrom - lookat;
		w.make_unit();
		u = vup.cross(w);
		u.make_unit();
		v = w.cross(u);
		lower_left_corner = origin - u * half_width * focus_dist - v * half_height * focus_dist - w * focus_dist;
		horizontal = u * 2 * half_width * focus_dist;
		vertical = v * 2 * half_height * focus_dist;
	};
	raycaster::ray get_ray(float s, float t) { 
		math::vec rd = random_in_unit_disk() * lens_radius;
		math::vec offset = u * rd[0] + v * rd[1];
		return raycaster::ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset); 
	};
private:
	math::vec origin;
	math::vec lower_left_corner;
	math::vec horizontal;
	math::vec vertical;
	math::vec u, v, w;
	float lens_radius;
};


#endif

#pragma once
#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class material;

struct hit_record {
	float t;
	math::vec p;
	math::vec normal;
	material *mat_ptr;
};

class hitable {
public:
	virtual bool hit(raycaster::ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif // !HITABLEH


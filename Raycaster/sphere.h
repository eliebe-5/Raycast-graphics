#pragma once
#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {};
	sphere(math::vec cen, float r, material* m) : center(cen), rad(r), mat_ptr(m) {};
	virtual bool hit(raycaster::ray& r, float t_min, float t_max, hit_record& rec) const;
private:
	math::vec center;
	float rad;
	material* mat_ptr;
};

bool sphere::hit(raycaster::ray& r, float t_min, float t_max, hit_record& rec) const {
	math::vec oc = r.origin() - this->center;
	float a = r.direction().dot(r.direction());
	float b = oc.dot(r.direction());
	float c = oc.dot(oc) - this->rad * this->rad;
	float discriminant = b * b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point(rec.t);
			rec.normal = (rec.p - center) / this->rad;
			rec.mat_ptr = this->mat_ptr;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point(rec.t);
			rec.normal = (rec.p - center) / this->rad;
			rec.mat_ptr = this->mat_ptr;
			return true;
		}
	}
	return false;
}

#endif 

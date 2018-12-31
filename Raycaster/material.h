#pragma once
#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"
#include <random>

float drand48() {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.99999f));
}

math::vec random_in_unit_sphere() {
	math::vec p;
	do {
		p = math::vec(drand48(), drand48(), drand48())*2.f - math::vec(1.f, 1.f, 1.f);
	} while (p.mag() * p.mag() >= 1.0);
	return p;
}

bool refract(math::vec& v, math::vec& n, float ni_over_nt, math::vec& refracted) {
	math::vec uv = v;
	uv.make_unit();
	float dt = uv.dot(n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminant > 0) {
		refracted = (uv - n * dt) * ni_over_nt - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

math::vec reflect(math::vec& v, math::vec& n) {
	return v - n * (v.dot(n) * 2);
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1.f - r0)*pow((1.f - cosine), 5);
}

class material {
public:
	virtual bool scatter(raycaster::ray& r_in, hit_record& rec, math::vec& attenuation, raycaster::ray& scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(math::vec a) : albedo(a) {};
	virtual bool scatter(raycaster::ray& r_in, hit_record& rec, math::vec& attenuation, raycaster::ray& scattered) const {
		math::vec target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = raycaster::ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
private:
	math::vec albedo;
};

class metal : public material {
public:
	metal(math::vec a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; };
	virtual bool scatter(raycaster::ray& r_in, hit_record& rec, math::vec& attenuation, raycaster::ray& scattered) const {
		math::vec r_in_temp = r_in.direction();
		r_in_temp.make_unit();
		math::vec reflected = reflect(r_in_temp, rec.normal);
		scattered = raycaster::ray(rec.p, reflected + random_in_unit_sphere()*fuzz);
		attenuation = albedo;
		return (scattered.direction().dot(rec.normal) > 0);
	}
private:
	math::vec albedo;
	float fuzz;
};

class dielectric : public material {
public:
	dielectric(float ri) : ref_idx(ri) {};
	virtual bool scatter(raycaster::ray& r_in, hit_record& rec, math::vec& attenuation, raycaster::ray& scattered) const {
		math::vec outward_normal;
		math::vec r_in_temp = r_in.direction();
		math::vec reflected = reflect(r_in_temp, rec.normal);
		float ni_over_nt;
		attenuation = math::vec(1.f, 1.f, 1.f);
		math::vec refracted;
		float reflect_prob;
		float cosine;
		if (r_in_temp.dot(rec.normal) > 0) {
			outward_normal = math::vec(0.f, 0.f, 0.f) - rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * r_in_temp.dot(rec.normal) / r_in_temp.mag();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.f / ref_idx;
			cosine = -r_in_temp.dot(rec.normal) / r_in_temp.mag();
		}
		if (refract(r_in_temp, outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			reflect_prob = 1.f;
		}
		if (drand48() < reflect_prob) {
			scattered = raycaster::ray(rec.p, reflected);
		}
		else {
			scattered = raycaster::ray(rec.p, refracted);
		}
		return true;
	};

private:
	float ref_idx;
};

#endif // !MATERIALH


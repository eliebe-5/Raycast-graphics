#pragma once
#ifndef RAYH
#define RAYH
#include "math.h"

namespace raycaster
{
	class ray
	{
	public:
		ray() {};
		ray(const math::vec& a, const math::vec& b) { A = a; B = b; }
		math::vec origin() { return A; };
		math::vec direction() { return B; };
		math::vec point(float t) { return A + (B * t); };
	private:
		math::vec A;
		math::vec B;
	};
}

#endif // !RAYH


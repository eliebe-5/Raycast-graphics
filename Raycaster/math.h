#pragma once
#include <math.h>

namespace math {

	class vec
	{
	public:
		vec() {};
		vec(float x, float y, float z) { set(x, y, z); };
		vec(const vec& a) { set(a.internal_data[0], a.internal_data[1], a.internal_data[2]); };
		inline void set(float x, float y, float z) { internal_data[0] = x; internal_data[1] = y; internal_data[2] = z; };
		inline vec add(vec a) { return vec(internal_data[0] + a.internal_data[0], internal_data[1] + a.internal_data[1], internal_data[2] + a.internal_data[2]); };
		inline vec sub(vec a) { return vec(internal_data[0] - a.internal_data[0], internal_data[1] - a.internal_data[1], internal_data[2] - a.internal_data[2]); };
		inline vec mult(const float a) { return vec(internal_data[0] * a, internal_data[1] * a, internal_data[2] * a); };
		inline vec mult(vec a) { return vec(internal_data[0] * a.internal_data[0], internal_data[1] * a.internal_data[1], internal_data[2] * a.internal_data[2]); };
		inline float dot(vec a) { return internal_data[0] * a.internal_data[0] + internal_data[1] * a.internal_data[1] + internal_data[2] * a.internal_data[2]; };
		inline vec cross(vec a)
		{
			return vec(
				internal_data[1] * a.internal_data[2] - internal_data[2] * a.internal_data[1],
				internal_data[2] * a.internal_data[0] - internal_data[0] * a.internal_data[2],
				internal_data[0] * a.internal_data[1] - internal_data[1] * a.internal_data[0]
			);
		};
		inline vec div(const float a) { return vec(internal_data[0] / a, internal_data[1] / a, internal_data[2] / a); };
		inline vec div(vec a) { return vec(internal_data[0] / a.internal_data[0], internal_data[1] / a.internal_data[1], internal_data[2] / a.internal_data[2]); };
		inline float mag() { return sqrt(internal_data[0] * internal_data[0] + internal_data[1] * internal_data[1] + internal_data[2] * internal_data[2]); };
		inline void make_unit() { float k = 1.0f / mag(); set(internal_data[0] * k, internal_data[1] * k, internal_data[2] * k); };

		inline vec operator+(const vec& a) { return add(a); };
		inline vec operator-(const vec& a) { return sub(a); };
		inline vec operator*(const vec& a) { return mult(a); };
		inline vec operator*(float a) { return mult(a); };
		inline vec operator/(float a) { return div(a); }
		inline vec operator/(const vec& a) { return div(a); }
		inline vec& operator=(const vec a) { set(a[0], a[1], a[2]); return *this; };
		inline bool operator==(const vec& a) { return (a[0] == (*this)[0]) && ((a[1] == (*this)[1])) && (a[2] == (*this)[2]); };
		inline bool operator!=(const vec& a) { return !((*this) == a); };
		inline float operator[](int a) const { return internal_data[a]; };
		inline float& operator[](int a) { return internal_data[a]; };

	private:
		float internal_data[3];
	};

}

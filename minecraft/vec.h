#pragma once

#include <stdio.h>    
#include <string.h>
#include <math.h>  
#include <vector>
using namespace std;

/**
* Three dimensional vector functions
*/
namespace vec3  {

	class Triangle
	{
	public:
		std::vector<vector<float>> points;
		float w[3];
		Triangle() {
			this->points = vector<vector<float>>();
			w[0] = 1.0;
			w[1] = 1.0;
			w[2] = 1.0;
		}
	};

	inline bool cmpf(float f0, float f1, float d = 0.005f) { 
		return (fabs((double)f0 - (double)f1) < d);
	}

	inline bool cmp(const float v0[4], const float v1[4]) {
		return (cmpf(v0[0], v1[0]) &&
				cmpf(v0[1], v1[1]) &&
				cmpf(v0[2], v1[2]));
	}

	inline void cpy(const float v[4], float out[4]) {
		out[0] = v[0];
		out[1] = v[1];
		out[2] = v[2];
		out[3] = v[3];
	}

	inline void zero(float out[4]) {
		out[0] = 0.0f;
		out[1] = 0.0f;
		out[2] = 0.0f;
		out[3] = 1.0f;
	}

	inline void inverse(float v[4]) {
		v[0] = -v[0];
		v[1] = -v[1];
		v[2] = -v[2];
	}

	inline void translate(const float v0[4], const float v1[4], float out[4]) {
		out[0] = v0[0] + v1[0];
		out[1] = v0[1] + v1[1];
		out[2] = v0[2] + v1[2];
	}

	inline void inv_translate(const float v0[4], const float v1[4], float out[4]) {
		out[0] = v0[0] - v1[0];
		out[1] = v0[1] - v1[1];
		out[2] = v0[2] - v1[2];
	}

	inline void scale(const float v0[4], const float v1[4], float out[4]) {
		out[0] = v0[0] * v1[0];
		out[1] = v0[1] * v1[1];
		out[2] = v0[2] * v1[2];
	}

	inline void scalar(const float v0[4], float k, float out[4]) {
		out[0] = v0[0] * k;
		out[1] = v0[1] * k;
		out[2] = v0[2] * k;
	}

	inline float dot(const float v0[4], const float v1[4]) {
		return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
	}

	inline float dist(const float v0[4], const float v1[4]) {
		double x = (double)v1[0] - (double)v0[0];
		double y = (double)v1[1] - (double)v0[1];
		double z = (double)v1[2] - (double)v0[2];
		return (float)sqrt(x * x + y * y + z * z);
	}

	inline float length(const float v0[4]) {
		double x = (double)v0[0];
		double y = (double)v0[1];
		double z = (double)v0[2];
		return (float)sqrt(x * x + y * y + z * z);
	}

	inline void normalize(const float v0[4], float out[4]) {
		float len = length(v0);
		out[0] = v0[0] / len;
		out[1] = v0[1] / len;
		out[2] = v0[2] / len;
	}

	inline void set_length(const float v0[4], float k, float out[4]) {
		normalize(v0, out);
		scalar(out, k, out);
	}

	inline void init(float x, float y, float z, float out[4]) {
		out[0] = x;
		out[1] = y;
		out[2] = z;
		out[3] = 1.0f;
	}

	const char* string(std::string name, const float v[4]) {
		std::string str = name + " { ";
		str += "x : " + to_string(v[0]) + " / ";
		str += "y : " + to_string(v[1]) + " / ";
		str += "z : " + to_string(v[2]) + " / ";
		str += "w : " + to_string(v[3]) + " }\n";
		return str.c_str();
	}
}

/**
* Two dimensional vector functions
*/
namespace vec2 {

	inline bool cmpf(float f0, float f1, float d = 0.005f) {
		return (fabs((double)f0 - (double)f1) < d);
	}

	inline bool cmp(const float v0[2], const float v1[2]) {
		return (cmpf(v0[0], v1[0]) &&
				cmpf(v0[1], v1[1]));
	}

	inline void cpy(const float v[2], float out[2]) {
		out[0] = v[0];
		out[1] = v[1];
	}

	inline void zero(float out[2]) {
		out[0] = 0.0f;
		out[1] = 0.0f;
	}

	inline void inverse(float v[2]) {
		v[0] = -v[0];
		v[1] = -v[1];
	}

	inline void translate(const float v0[2], const float v1[2], float out[2]) {
		out[0] = v0[0] + v1[0];
		out[1] = v0[1] + v1[1];
	}

	inline void inv_translate(const float v0[2], const float v1[2], float out[2]) {
		out[0] = v0[0] - v1[0];
		out[1] = v0[1] - v1[1];
	}

	inline void scale(const float v0[2], const float v1[2], float out[2]) {
		out[0] = v0[0] * v1[0];
		out[1] = v0[1] * v1[1];
	}

	inline void scalar(const float v0[2], float k, float out[2]) {
		out[0] = v0[0] * k;
		out[1] = v0[1] * k;
	}

	inline float dot(const float v0[2], const float v1[2]) {
		return v0[0] * v1[0] + v0[1] * v1[1];
	}

	inline float cross(const float v0[2], const float v1[2]) {
		return v0[0] * v1[1] - v0[1] * v1[0];
	}

	inline float dist(const float v0[2], const float v1[2]) {
		double x = (double)v1[0] - (double)v0[0];
		double y = (double)v1[1] - (double)v0[1];
		return (float)sqrt(x * x + y * y);
	}

	inline float length(const float v0[2]) {
		double x = (double)v0[0];
		double y = (double)v0[1];
		return (float)sqrt(x * x + y * y);
	}

	inline void normalize(const float v0[2], float out[2]) {
		float m = length(v0);
		out[0] = v0[0] / m;
		out[1] = v0[1] / m;
	}

	inline void set_length(const float v0[2], float k, float out[2]) {
		normalize(v0, out);
		scalar(out, k, out);
	}

	inline void init(float x, float y, float out[2]) {
		out[0] = x;
		out[1] = y;
	}
}
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include <iterator>
#include <math.h>

#ifdef WINDOWS
#include <windows.h>
#endif

using namespace std;

struct vec2f
{
	/* Init */
	float x = 0.0f, y = 0.0f;
	vec2f() { this->x = 0; this->y = 0; };
	vec2f(float x, float y) { this->x = x; this->y = y; };

	/* Basic Operations */
	bool operator==(const vec2f& p) const { return (this->x == p.x && this->y == p.y); };
	bool operator!=(const vec2f& p) const { return !(this->x == p.x && this->y == p.y); };
	vec2f operator+(const vec2f& p) const { return vec2f(this->x + p.x, this->y + p.y); };
	vec2f operator-(const vec2f& p) const { return vec2f(this->x - p.x, this->y - p.y); };
	vec2f operator*(const vec2f& p) const { return vec2f(this->x * p.x, this->y * p.y); };
	vec2f operator/(const vec2f& p) const { return vec2f(this->x / p.x, this->y / p.y); };
	void operator+=(const vec2f& p) { this->x += p.x; this->y += p.y; };
	void operator-=(const vec2f& p) { this->x -= p.x; this->y -= p.y; };
	void operator*=(const vec2f& p) { this->x *= p.x; this->y *= p.y; };
	void operator/=(const vec2f& p) { this->x /= p.x; this->y /= p.y; };

	/* Dot product and Cross product */
	float operator|(const vec2f& p) const { return this->x * p.x + this->y * p.y; };
	float operator&(const vec2f& p) const { return this->x * p.y - this->y * p.x; };
};

struct vec3f
{
	/* Init */
	float x = 0, y = 0, z = 0, w = 1.0f;
	vec3f() { this->x = 0; this->y = 0; this->z = z; };
	vec3f(float x, float y, float z) { this->x = x; this->y = y; this->z = z; };

	/* Basic Operations */
	bool operator==(const vec3f& p) const { return (this->x == p.x && this->y == p.y && this->z == p.z); };
	bool operator!=(const vec3f& p) const { return !(this->x == p.x && this->y == p.y && this->z == p.z); };
	vec3f operator+(const vec3f& p) const { return vec3f(this->x + p.x, this->y + p.y, this->z + p.z); };
	vec3f operator-(const vec3f& p) const { return vec3f(this->x - p.x, this->y - p.y, this->z - p.z); };
	vec3f operator*(const vec3f& p) const { return vec3f(this->x * p.x, this->y * p.y, this->z * p.z); };
	vec3f operator/(const vec3f& p) const { return vec3f(this->x / p.x, this->y / p.y, this->z / p.z); };
	void operator+=(const vec3f& p) { this->x += p.x; this->y += p.y; this->z += p.z; };
	void operator-=(const vec3f& p) { this->x -= p.x; this->y -= p.y; this->z -= p.z; };
	void operator*=(const vec3f& p) { this->x *= p.x; this->y *= p.y; this->z *= p.z; };
	void operator/=(const vec3f& p) { this->x /= p.x; this->y /= p.y; this->z /= p.z; };

	/* Dot product */
	float operator|(const vec3f& p) const { return this->x * p.x + this->y * p.y + this->z * p.z; };
};

class Math
{
public:
	static float DotProduct(vec2f a, vec2f b);
	static float DotProduct(vec3f a, vec3f b);
	static float Distance(vec2f a, vec2f b);
	static float Distance(vec3f a, vec3f b);
	static float ToRadian(float degree);
	static float ToDegree(float radian);
	static vec3f Normalize(vec3f vec);
	static vec2f Normalize(vec2f vec);
	static bool IsBetweenIncluded(float x, float min, float max);
	static bool IsBetweenExcluded(float x, float min, float max);
private:
	Math();
};


class Mat4x4
{
public:
	vector<vector<float>> m;
	Mat4x4(vector<vector<float>> values) { this->m = values; };
	Mat4x4() { m = { { 0.0f,0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f,0.0f } }; };
	static vec3f MultiplyVector(Mat4x4& m, vec3f& i);
	static Mat4x4 MakeIdentity();
	static Mat4x4 MakeRotationX(float fAngleRad);
	static Mat4x4 MakeRotationY(float fAngleRad);
	static Mat4x4 MakeRotationZ(float fAngleRad);
	static Mat4x4 MakeTranslation(float x, float y, float z);
	static Mat4x4 MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
	static Mat4x4 MultiplyMatrix(Mat4x4& m1, Mat4x4& m2);
	static Mat4x4 QuickInverse(Mat4x4& m);
};

template <class T>
class Triangle
{
public:
	T points[3];
	//T startPos;
	//vec2f textureCoordinates[3];
	//vec2f texStartPos;
	//vec2f texRatio;
	//float lightImpact[3];
	//vec3f faceNormal;
	float w[3];
};

# define M_PI           3.14159265358979323846  /* pi */

float Math::DotProduct(vec2f a, vec2f b) {
	return a.x * b.x + a.y * b.y;
}

float Math::DotProduct(vec3f a, vec3f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Math::Distance(vec2f a, vec2f b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

float Math::Distance(vec3f a, vec3f b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

float Math::ToRadian(float degree) {
	return degree / 180.0f * M_PI;
}

float Math::ToDegree(float radian) {
	return radian * 180.0f / M_PI;
}

vec2f Math::Normalize(vec2f a) {
	float l = Distance(vec2f(0.0, 0.0), a);
	return vec2f(a.x / l, a.y / l);
}

vec3f Math::Normalize(vec3f a) {
	float l = Distance(vec3f(0.0, 0.0, 0.0), a);
	return vec3f(a.x / l, a.y / l, a.z / l);
}

bool Math::IsBetweenIncluded(float x, float min, float max) {
	return (x >= 0 && x <= max);
}

bool Math::IsBetweenExcluded(float x, float min, float max) {
	return (x > 0 && x < max);
}

vec3f Mat4x4::MultiplyVector(Mat4x4& m, vec3f& i)
{
	vec3f v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}

Mat4x4 Mat4x4::MakeIdentity()
{
	Mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

Mat4x4 Mat4x4::MakeRotationX(float fAngle)
{
	float cosv = cosf(fAngle / 180.0 * M_PI);
	float sinv = sinf(fAngle / 180.0 * M_PI);

	Mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosv;
	matrix.m[1][2] = sinv;
	matrix.m[2][1] = -sinv;
	matrix.m[2][2] = cosv;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

Mat4x4 Mat4x4::MakeRotationY(float fAngle)
{
	float cosv = cosf(fAngle / 180.0 * M_PI);
	float sinv = sinf(fAngle / 180.0 * M_PI);

	Mat4x4 matrix;
	matrix.m[0][0] = cosv;
	matrix.m[0][2] = sinv;
	matrix.m[2][0] = -sinv;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosv;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

Mat4x4 Mat4x4::MakeRotationZ(float fAngle)
{
	float cosv = cosf(fAngle / 180.0 * M_PI);
	float sinv = sinf(fAngle / 180.0 * M_PI);

	Mat4x4 matrix;
	matrix.m[0][0] = cosv;
	matrix.m[0][1] = sinv;
	matrix.m[1][0] = -sinv;
	matrix.m[1][1] = cosv;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

Mat4x4 Mat4x4::MakeTranslation(float x, float y, float z)
{
	Mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;
	return matrix;
}

Mat4x4 Mat4x4::MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
	float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * M_PI);
	Mat4x4 matrix;
	matrix.m[0][0] = fAspectRatio * fFovRad;
	matrix.m[1][1] = fFovRad;
	matrix.m[2][2] = fFar / (fFar - fNear);
	matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}

Mat4x4 Mat4x4::MultiplyMatrix(Mat4x4& m1, Mat4x4& m2)
{
	Mat4x4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = (float)m1.m[r][0] * (float)m2.m[0][c] + (float)m1.m[r][1] * (float)m2.m[1][c] + (float)m1.m[r][2] * (float)m2.m[2][c] + (float)m1.m[r][3] * (float)m2.m[3][c];
	return matrix;
}


Mat4x4 Mat4x4::QuickInverse(Mat4x4& m) // Only for Rotation/Translation Matrices
{
	Mat4x4 matrix;
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}
#include "Vector3.h"
#include <math.h>

Vector3::Vector3(float x0, float x1, float x2)
{
	x[0] = x0; x[1] = x1; x[2] = x2;
}

Vector3::Vector3(const Vector3 & v)
{
	x[0] = v.x[0]; x[1] = v.x[1]; x[2] = v.x[2];
}

Vector3 & Vector3::set(float x0, float x1, float x2)
{
	x[0] = x0; x[1] = x1; x[2] = x2; return *this;
}

Vector3::operator float *()
{
	return x;
}

float & Vector3::operator [] (int i)
{
	return x[i];
}

Vector3 & Vector3::operator += (const Vector3 & v)
{
	x[0] += v.x[0]; x[1] += v.x[1]; x[2] += v.x[2]; return *this;
}

Vector3 & Vector3::operator -= (const Vector3 & v)
{
	x[0] -= v.x[0]; x[1] -= v.x[1]; x[2] -= v.x[2]; return *this;
}

Vector3 & Vector3::operator *= (float s)
{
	x[0] *= s; x[1] *= s; x[2] *= s; return *this;
}

Vector3 & Vector3::operator = (const Vector3 & v)
{
	x[0] = v.x[0]; x[1] = v.x[1]; x[2] = v.x[2]; return *this;
}

Vector3 operator + (const Vector3 & a, const Vector3 & b)
{
	Vector3 c(a);
	return c += b;
}

Vector3 operator - (const Vector3 & a, const Vector3 & b)
{
	Vector3 c(a);
	return c -= b;
}

Vector3 operator * (const Vector3 & v, float s)
{
	Vector3 a(v);
	return a *= s;
}

Vector3 operator * (float s, const Vector3 & v)
{
	Vector3 a(v);
	return a *= s;
}

Vector3 Vector3::cross(const Vector3 & v)
{
	return Vector3(
		x[1] * v.x[2] - x[2] * v.x[1],
		x[2] * v.x[0] - x[0] * v.x[2],
		x[0] * v.x[1] - x[1] * v.x[0]);
}

float Vector3::length()
{
	return sqrt(length2());
}

float Vector3::length2()
{
	return x[0]*x[0] + x[1]*x[1] + x[2]*x[2];
}

Vector3 & Vector3::norm(float s)
{
	float f = length();
	if (f == 0.0) return *this;
	s /= f;
	x[0] *= s; x[1] *= s; x[2] *= s;
	return *this;
}

ostream & operator << (ostream & os, const Vector3 & v)
{
	os << "(" << v.x[0] << "," << v.x[1] << "," << v.x[2] << ")";
	return os;
}

float operator * (const Vector3 & a, const Vector3 & b)
{
	return a.x[0]*b.x[0] + a.x[1]*b.x[1] + a.x[2]*b.x[2];
}


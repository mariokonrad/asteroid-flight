#include "Quaternion.h"
#include <math.h>

Quaternion::Quaternion(float x0, float x1, float x2, float x3)
{
	x[0] = x0; x[1] = x1; x[2] = x2; x[3] = x3;
}

Quaternion::Quaternion(const Quaternion & q)
{
	x[0] = q.x[0]; x[1] = q.x[1]; x[2] = q.x[2]; x[3] = q.x[3];
}

Quaternion::Quaternion(float s, const Vector3 & v)
{
	x[0] = s;
	x[1] = static_cast<Vector3>(v)[0];
	x[2] = static_cast<Vector3>(v)[1];
	x[3] = static_cast<Vector3>(v)[2];
}

float Quaternion::s()
{
	return x[0];
}

Vector3 Quaternion::v()
{
	return Vector3(x[1], x[2], x[3]);
}

Quaternion::operator float * ()
{
	return x;
}

float & Quaternion::operator [] (int i)
{
	return x[i];
}

Quaternion & Quaternion::operator += (const Quaternion & q)
{
	x[0] += q.x[0]; x[1] += q.x[1]; x[2] += q.x[2]; x[3] += q.x[3]; return *this;
}

Quaternion & Quaternion::operator -= (const Quaternion & q)
{
	x[0] -= q.x[0]; x[1] -= q.x[1]; x[2] -= q.x[2]; x[3] -= q.x[3]; return *this;
}

Quaternion & Quaternion::operator *= (const Quaternion & b)
{
	Quaternion a(*this);
	x[0] = a.x[0] * b.x[0] - a.x[1] * b.x[1] - a.x[2] * b.x[2] - a.x[3] * b.x[3];
	x[1] = a.x[0] * b.x[1] + b.x[0] * a.x[1] + a.x[2] * b.x[3] - a.x[3] * b.x[2];
	x[2] = a.x[0] * b.x[2] + b.x[0] * a.x[2] - a.x[1] * b.x[3] + a.x[3] * b.x[1];
	x[3] = a.x[0] * b.x[3] + b.x[0] * a.x[3] + a.x[1] * b.x[2] - a.x[2] * b.x[1];
	return *this;
}

Quaternion & Quaternion::operator *= (float s)
{
	x[0] *= s; x[1] *= s; x[2] *= s; x[3] *= s; return *this;
}

float Quaternion::length()
{
	return sqrt(length2());
}

float Quaternion::length2()
{
	return x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3];
}

Quaternion & Quaternion::norm(float s)
{
	float f = length();
	if (f == 0.0) return *this;
	s /= f;
	x[0] *= s; x[1] *= s; x[2] *= s; x[3] *= s;
	return *this;
}

Quaternion Quaternion::inv()
{
	return Quaternion(x[0], -x[1], -x[2], -x[3]);
}

Quaternion operator + (const Quaternion & a, const Quaternion & b)
{
	Quaternion q(a);
	return q += b;
}

Quaternion operator - (const Quaternion & a, const Quaternion & b)
{
	Quaternion q(a);
	return q -= b;
}

Quaternion operator * (float s, const Quaternion & a)
{
	Quaternion q(a);
	return q *= s;
}

Quaternion operator * (const Quaternion & a, float s)
{
	Quaternion q(a);
	return q *= s;
}

Quaternion operator * (const Quaternion & a, const Quaternion & b)
{
	return Quaternion(
		a.x[0] * b.x[0] - a.x[1] * b.x[1] - a.x[2] * b.x[2] - a.x[3] * b.x[3],
		a.x[0] * b.x[1] + b.x[0] * a.x[1] + a.x[2] * b.x[3] - a.x[3] * b.x[2],
		a.x[0] * b.x[2] + b.x[0] * a.x[2] - a.x[1] * b.x[3] + a.x[3] * b.x[1],
		a.x[0] * b.x[3] + b.x[0] * a.x[3] + a.x[1] * b.x[2] - a.x[2] * b.x[1]
		);
}

ostream & operator << (ostream & os, const Quaternion & v)
{
	os << "(" << v.x[0] << "," << v.x[1] << "," << v.x[2] << "," << v.x[3] << ")";
	return os;
}


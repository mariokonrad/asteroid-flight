#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <iostream>

using namespace std;

class Vector3
{
	private:
		float x[3];
	public:
		Vector3(float = 0.0, float = 0.0, float = 0.0);
		Vector3(const Vector3 &);
		Vector3 & set(float, float, float);
		operator float * ();
		float & operator [] (int);
		Vector3 & operator += (const Vector3 &);
		Vector3 & operator -= (const Vector3 &);
		Vector3 & operator *= (float);
		Vector3 & operator = (const Vector3 &);
		Vector3 cross(const Vector3 &);
		float length();
		float length2();
		Vector3 & norm(float = 1.0);
		friend Vector3 operator + (const Vector3 &, const Vector3 &);
		friend Vector3 operator - (const Vector3 &, const Vector3 &);
		friend Vector3 operator * (const Vector3 &, float);
		friend Vector3 operator * (float, const Vector3 &);
		friend float operator * (const Vector3 &, const Vector3 &);
		friend ostream & operator << (ostream &, const Vector3 &);
};

#endif

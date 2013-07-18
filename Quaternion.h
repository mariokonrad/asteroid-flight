#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Vector3.h"
#include <iostream>

using namespace std;

class Quaternion
{
	private:
		float x[4];
	public:
		Quaternion(float = 0.0, float = 0.0, float = 0.0, float = 0.0);
		Quaternion(const Quaternion &);
		Quaternion(float, const Vector3 &);
		float s();
		Vector3 v();
		operator float * ();
		float & operator [] (int);
		Quaternion & operator += (const Quaternion &);
		Quaternion & operator -= (const Quaternion &);
		Quaternion & operator *= (const Quaternion &);
		Quaternion & operator *= (float);
		float length();
		float length2();
		Quaternion & norm(float = 1.0);
		Quaternion inv();
		friend Quaternion operator + (const Quaternion &, const Quaternion &);
		friend Quaternion operator - (const Quaternion &, const Quaternion &);
		friend Quaternion operator * (float, const Quaternion &);
		friend Quaternion operator * (const Quaternion &, float);
		friend Quaternion operator * (const Quaternion &, const Quaternion &);
		friend ostream & operator << (ostream &, const Quaternion &);
};

#endif

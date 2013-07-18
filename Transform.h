#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Vector3.h"

class Transform
{
	public:
		static Vector3 & rot(Vector3 *, const Vector3 &, float);
};

#endif

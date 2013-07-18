#include "Transform.h"
#include "Quaternion.h"
#include <math.h>

Vector3 & Transform::rot(Vector3 * p, const Vector3 & u, float angle)
{
	angle *= (M_PI / 180.0 / 2.0);
	Quaternion q(cos(angle), sin(angle) * u);
	Quaternion t = (q * Quaternion(0.0, *p)) * q.inv();
	t *= (1.0 / t.length2());
	*p = t.v();
	return *p;
}


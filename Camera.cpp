#include "Camera.h"
#include "Transform.h"
#include <GL/glu.h>
#include <math.h>

Camera::Camera()
	: fov(45.0), near(0.1), far(100.0)
{
	dir = Vector3(0.0, 0.0, -1.0);
	rgt = Vector3(1.0, 0.0,  0.0);
	upv = Vector3(0.0, 1.0,  0.0);
}

const Vector3 & Camera::getPos()
{
	return pos;
}

Camera & Camera::operator += (const Vector3 & v)
{
	pos += v; return *this;
}

Camera & Camera::operator -= (const Vector3 & v)
{
	pos -= v; return *this;
}

Camera & Camera::setPos(const Vector3 & v)
{
	pos = v; return *this;
}

void Camera::setNear(float n)
{
	near = n;
}

void Camera::setFar(float f)
{
	far = f;
}

void Camera::perspective(float aspect_ratio)
{
	gluPerspective(fov, aspect_ratio, near, far);
}

void Camera::draw()
{
	Vector3 c = pos + dir;
	gluLookAt(pos[0], pos[1], pos[2], c[0], c[1], c[2], upv[0], upv[1], upv[2]);
}

void Camera::forward(float step)
{
	pos += dir * step;
}

void Camera::backward(float step)
{
	pos -= dir * step;
}

void Camera::left(float step)
{
	pos -= rgt * step;
}

void Camera::right(float step)
{
	pos += rgt * step;
}

void Camera::roll(float angle)
{
	// arround z-axis in local coordinate system
	Transform::rot(&rgt, dir, angle);
	Transform::rot(&upv, dir, angle);
}

void Camera::pitch(float angle)
{
	// arround x-axis in local coordinate system
	Transform::rot(&dir, rgt, angle);
	Transform::rot(&upv, rgt, angle);
}

void Camera::yaw(float angle)
{
	// arround y-axis in local coordinate system
	Transform::rot(&dir, upv, angle);
	Transform::rot(&rgt, upv, angle);
}


bool Camera::boundSphereInFrustum(const Vector3 & p, float r)
{
	Vector3 p1 = dir * ((p * dir) / dir.length2());
	Vector3 d1 = (p1 - pos);

	if (d1 * dir < 0.0) return false;
	
	float l = d1.length();
	float d = l * tan(fov / 2.0);
	return (d >= r && l >= near && l <= far);
}


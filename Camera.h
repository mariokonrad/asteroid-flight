#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector3.h"

class Camera
{
	private:
		Vector3 pos;
		Vector3 dir;
		Vector3 rgt;
		Vector3 upv;
		float fov;
		float near;
		float far;
	public:
		Camera();
		Camera & operator += (const Vector3 &);
		Camera & operator -= (const Vector3 &);
		Camera & setPos(const Vector3 &);
		const Vector3 & getPos();
		void setNear(float);
		void setFar(float);

		void forward(float = 1.0);
		void backward(float = 1.0);
		void left(float = 1.0);
		void right(float = 1.0);
		void roll(float);
		void pitch(float);
		void yaw(float);

		void perspective(float);
		void draw();
		bool boundSphereInFrustum(const Vector3 &, float);
};

#endif

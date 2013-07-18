// objects.cpp

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <unistd.h>
#include "Camera.h"
#include <vector>
#include "ImageLoader.h"
#include <math.h>

GLuint texture[1];

class Object
{
	protected:
		Vector3 pos;
		float bsr;		// bounding sphere radius
	public:
		Vector3 & getPos();
		float getBSR();
		virtual void draw() = 0;
		virtual void update() = 0;
};

Vector3 & Object::getPos()
{
	return pos;
}

float Object::getBSR()
{
	return bsr;
}



class Cube : public Object
{
	private:
		Vector3 point[8];
		int quad[6][4];
		void setQuad(int, int, int, int, int);
		Vector3 u;
		float phi;
	public:
		Cube();
		virtual void draw();
		virtual void update();
};

Cube::Cube()
{
	u.set(1.0, 1.0, 1.0).norm();
	phi = 0.0;
	point[0].set(-0.5, -0.5,  0.5);
	point[1].set( 0.5, -0.5,  0.5);
	point[2].set( 0.5,  0.5,  0.5);
	point[3].set(-0.5,  0.5,  0.5);
	point[4].set(-0.5, -0.5, -0.5);
	point[5].set( 0.5, -0.5, -0.5);
	point[6].set( 0.5,  0.5, -0.5);
	point[7].set(-0.5,  0.5, -0.5);
	setQuad(0, 0, 1, 2, 3);
	setQuad(1, 1, 5, 6, 2);
	setQuad(2, 5, 4, 7, 6);
	setQuad(3, 4, 0, 3, 7);
	setQuad(4, 3, 2, 6, 7);
	setQuad(5, 4, 5, 1, 0);
}

void Cube::setQuad(int i, int a, int b, int c, int d)
{
	quad[i][0] = a;
	quad[i][1] = b;
	quad[i][2] = c;
	quad[i][3] = d;
}

void Cube::draw()
{
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(phi, u[0], u[1], u[2]);
	for (int q = 0; q < 6; ++q)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3fv(point[quad[q][0]]);
			glTexCoord2f(1.0f, 0.0f); glVertex3fv(point[quad[q][1]]);
			glTexCoord2f(1.0f, 1.0f); glVertex3fv(point[quad[q][2]]);
			glTexCoord2f(0.0f, 1.0f); glVertex3fv(point[quad[q][3]]);
		glEnd();
	}
	glPopMatrix();
}

void Cube::update()
{
	phi += 1.0;
	while (phi > 360.0) phi -= 360.0;
	while (phi <   0.0) phi += 360.0;
}




Camera camera;
vector<Object *> objects;


void idle(void)
{
	for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->update();
	}
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	camera.draw();
	
	Object * obj;
	for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		obj = *it;
		if (camera.boundSphereInFrustum(obj->getPos(), obj->getBSR())) obj->draw();
	}

	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	if (!height) height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	camera.perspective((float)width / (float)height);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w': camera.forward(); break;
		case 's': camera.backward(); break;
		case 'a': camera.left(); break;
		case 'd': camera.right(); break;
		case 'q': camera.roll(2.0); break;
		case 'e': camera.roll(-2.0); break;
		case 'r': camera.pitch(2.0); break;
		case 't': camera.pitch(-2.0); break;
		case 'f': camera.yaw(2.0); break;
		case 'g': camera.yaw(-2.0); break;
		case 27: exit(0);
	}
}

void initGL(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_LINES);
	glPolygonMode(GL_BACK, GL_LINES);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// init window
	glutCreateWindow("[FLIGHT]");
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(300, 300);

	// register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	// init graphics
	initGL();
	
	// init camera
	camera += Vector3(0.0, 0.0, 4.0);
	camera.setFar(200.0);

	// init universe
	//objects.push_back(new Cube);
	objects.push_back(new Cube);

	// load texture
	Image * img = ImageLoader::load("data.bmp", ImageLoader::BMP);
	if (!img)
	{
		cerr << "Could not load texture" << endl;
		exit(-1);
	}
	// create texture
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
		0,					// LOD, 0 = base image
		3,					// RGB = 3 components
		img->getWidth(),
		img->getHeight(),
		0,					// border
		GL_RGB,				// format
		GL_UNSIGNED_BYTE,	// type
		img->getData()
		);

	glutMainLoop();
	return 0;
}

//
// EOF
//

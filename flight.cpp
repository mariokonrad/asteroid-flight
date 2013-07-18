// flight.cpp

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <unistd.h>
#include "Camera.h"
#include <vector>
#include "ImageLoader.h"
#include "FPSCounter.h"

GLuint texture[1];

class Object
{
	protected:
		Vector3 pos;
		float bsr; // bounding sphere radius
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

class Asteroid : public Object
{
	public:
		typedef vector<Asteroid *> Vector;
	private:
		enum SHAPE
		{
			SPHERE, TORUS, STONE
		};
		float r;
		SHAPE shape;
		Vector3 u;
		float omega;
		float phi;
	public:
		Asteroid();
		virtual void draw();
		virtual void update();
};

Asteroid::Asteroid()
{
	pos[0] = (float)((rand() % 100) - 50);
	pos[1] = (float)((rand() % 100) - 50);
	pos[2] = (float)((rand() % 100) - 50);
	r = (float)(rand() % 40) / 10.0 + 1.0;
	bsr = r;
	switch (rand() % 3) {
		case 0: shape = SPHERE; break;
		case 1: shape = TORUS; break;
		case 2: shape = STONE; break;
	}
	u[0] = (float)((rand() % 1000) - 500);
	u[1] = (float)((rand() % 1000) - 500);
	u[2] = (float)((rand() % 1000) - 500);
	u.norm();
	omega = (float)((rand() % 300) / 100.0);
	if (rand() % 2) omega = 0.0;
	phi = 0.0;
}

void Asteroid::draw()
{
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(phi, u[0], u[1], u[2]);
	glScalef(r, r, r);
	switch (shape) {
		case SPHERE:
			glutWireSphere(1.0, 6, 6);
			break;
		case TORUS:
			glutWireTorus(0.25, 1.0, 6, 6);
			break;
		case STONE:
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glBegin(GL_QUADS);
				// quad 1
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5,  0.5);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5, -0.5,  0.5);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5,  0.5,  0.5);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5,  0.5,  0.5);
				// quad 2
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5, -0.5,  0.5);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5, -0.5, -0.5);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5,  0.5, -0.5);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5,  0.5,  0.5);
				// quad 3
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5, -0.5, -0.5);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5,  0.5, -0.5);
				glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5,  0.5, -0.5);
				// quad 4
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, -0.5,  0.5);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5,  0.5,  0.5);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5,  0.5, -0.5);
				// quad 5
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5,  0.5,  0.5);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5,  0.5,  0.5);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5,  0.5, -0.5);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5,  0.5, -0.5);
				// quad 6
				glNormal3f(0.0,-1.0, 0.0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5, -0.5, -0.5);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5, -0.5,  0.5);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, -0.5,  0.5);
			glEnd();
			break;
	}
	glPopMatrix();
}

void Asteroid::update()
{
	if (omega != 0.0) {
		phi += omega;
		while (phi > 360.0) phi -= 360.0;
		while (phi <   0.0) phi += 360.0;
	}
}


FPSCounter fps;
Camera camera;
Asteroid::Vector asteroids;
float speed = 0.0;

void idle(void)
{
	camera.forward(speed);
	Asteroid * a;
	for (Asteroid::Vector::iterator it = asteroids.begin(); it != asteroids.end(); ++it) {
		a = *it;
		a->update();
	}
	glutPostRedisplay();
}

void display(void)
{
	fps.tick();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	camera.draw();

	Asteroid * a;
	for (Asteroid::Vector::iterator it = asteroids.begin(); it != asteroids.end(); ++it) {
		a = *it;
		if (camera.boundSphereInFrustum(a->getPos(), a->getBSR())) {
			a->draw();
		}
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
	switch (key) {
		case 'w': speed = 0.1; break; //camera.forward(); break;
		case 's': speed = 0.0; break; //camera.backward(); break;
		case 'a': camera.left(); break;
		case 'd': camera.right(); break;
		case 'q': camera.roll(2.0); break;
		case 'e': camera.roll(-2.0); break;
		case 'r': camera.pitch(2.0); break;
		case 't': camera.pitch(-2.0); break;
		case 'f': camera.yaw(2.0); break;
		case 'g': camera.yaw(-2.0); break;
		case 13: cout << fps.fps() << " FPS" << endl; break;
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
	camera += Vector3(0.0, 0.0, 2.0);
	camera.setFar(200.0);

	// init universe
	for (int i = 0; i < 100; ++i) asteroids.push_back(new Asteroid);

	// load texture
	Image * img = ImageLoader::load("data.bmp", ImageLoader::BMP);
	if (!img) {
		cerr << "Could not load texture: data.bmp" << endl;
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


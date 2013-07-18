#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

GLfloat camera_pos[] = { 35.0, 50.0, 40.0 };

GLfloat ball_pos[] = { 0.0, 0.0, 0.0 };
GLfloat ball_dir[] = { 10.0, 2.0, 2.0 };	// 1 unit per sec.

static const GLfloat bound_vec[][3] =
{
	{ -10.0, -10.0,  10.0 },
	{  10.0, -10.0,  10.0 },
	{  10.0, -10.0, -10.0 },
	{ -10.0, -10.0, -10.0 },
	{ -10.0,  10.0,  10.0 },
	{  10.0,  10.0,  10.0 },
	{  10.0,  10.0, -10.0 },
	{ -10.0,  10.0, -10.0 },
};
static const GLint bound_num_vec = sizeof(bound_vec)/sizeof(GLfloat)/3;
static const GLint bound_surf[][3] =
{
	{ 0, 1, 5 }, { 0, 5, 4 },
	{ 1, 2, 6 }, { 1, 6, 5 },
	{ 2, 3, 7 }, { 2, 7, 6 },
	{ 3, 0, 4 }, { 3, 4, 7 },
	{ 4, 5, 6 }, { 4, 6, 7 },
	{ 2, 0, 1 }, { 2, 3, 0 },
};
static const GLint bound_num_surf = sizeof(bound_surf)/sizeof(GLint)/3;
GLfloat bound_surf_abcd[bound_num_surf][4];


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(
		camera_pos[0], camera_pos[1], camera_pos[2],
		0.0, 0.0,  0.0,
		0.0, 1.0,  0.0);

	// draw boundaries
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glBegin(GL_TRIANGLES);
	for (int s = 0; s < bound_num_surf; ++s)
	{
		glVertex3fv(bound_vec[bound_surf[s][0]]);
		glVertex3fv(bound_vec[bound_surf[s][1]]);
		glVertex3fv(bound_vec[bound_surf[s][2]]);
	}
	glEnd();

	// draw ball
	glTranslatef(ball_pos[0], ball_pos[1], ball_pos[2]);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glutWireSphere(1.0, 8, 8);

	glFlush();
	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27: exit(0);
	}
}

void calc_n(GLfloat n[], const GLfloat a[], const GLfloat b[], const GLfloat c[])
{
	GLfloat ba[3];
	GLfloat ca[3];
	ba[0] = b[0]-a[0]; ba[1] = b[1]-a[1]; ba[2] = b[2]-a[2];
	ca[0] = c[0]-a[0]; ca[1] = c[1]-a[1]; ca[2] = c[2]-a[2];
	n[0] = ba[1]*ca[2]-ba[2]*ca[1];
	n[1] = ba[2]*ca[0]-ba[0]*ca[2];
	n[2] = ba[0]*ca[1]-ba[1]*ca[0];
	GLfloat l = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] /= l;
	n[1] /= l;
	n[2] /= l;
}

void calc_n(GLfloat n[], int surf)
{
	calc_n(n,
		bound_vec[bound_surf[surf][0]],
		bound_vec[bound_surf[surf][1]],
		bound_vec[bound_surf[surf][2]]);
}

void calc_ABCD(GLfloat abcd[], const GLfloat a[], const GLfloat b[], const GLfloat c[])
{
	abcd[0] = a[1]*(b[2]-c[2]) + b[1]*(c[2]-a[2]) + c[1]*(a[2]-b[2]);
	abcd[1] = a[2]*(b[0]-c[0]) + b[2]*(c[0]-a[0]) + c[2]*(a[0]-b[0]);
	abcd[2] = a[0]*(b[1]-c[1]) + b[0]*(c[1]-a[1]) + c[0]*(a[1]-b[1]);
	abcd[3] = -(abcd[0]*a[0] + abcd[1]*b[1] + abcd[2]*a[2]);
}

// spiegelt vektor d an ebene (a,b,c) => vektor f
void calc_mirror(GLfloat f[], int surf, GLfloat d[])
{
	f[0] = d[0]; f[1] = d[1]; f[2] = d[2];
	GLfloat n[3];
	GLfloat * abcd = (GLfloat *)&bound_surf_abcd[surf];
	GLfloat den = abcd[0]*d[0] + abcd[1]*d[1] + abcd[2]*d[2];
	if (den == 0.0) return;
	calc_n(n, bound_vec[bound_surf[surf][0]], bound_vec[bound_surf[surf][1]], bound_vec[bound_surf[surf][2]]);
	GLfloat l = 2.0 * (abcd[0]*d[0] + abcd[1]*d[1] + abcd[2]*d[2] + abcd[3]) / den;
	f[0] = d[0] + n[0] * l;
	f[1] = d[1] + n[1] * l;
	f[2] = d[2] + n[2] * l;
}

// d,e		Gerade: d + u * (e-d)
GLfloat calc_u(int surf, const GLfloat d[], const GLfloat e[])
{
	GLfloat e0d0 = e[0]-d[0];
	GLfloat e1d1 = e[1]-d[1];
	GLfloat e2d2 = e[2]-d[2];

	GLfloat * abcd = (GLfloat *)&(bound_surf_abcd[surf]);
	GLfloat den = abcd[0]*e0d0 + abcd[1]*e1d1 + abcd[2]*e2d2;
	if (den == 0.0) return 0.0;
	return -(abcd[0]*d[0] + abcd[1]*d[1] + abcd[2]*d[2] + abcd[3]) / den;
}

// >0:left, <0:right, 0:on line
GLfloat sideOf(const GLfloat a[], const GLfloat b[], const GLfloat p[])
{
	return ((b[0]-a[0]) * (p[1]-a[1]) - (p[0]-a[0]) * (b[1]-a[1]));
}

/* [###] 2D only
// 0:ouside, else:inside
int inside(int surf, GLfloat p[])
{
	return
		   sideOf(bound_vec[bound_surf[surf][0]], bound_vec[bound_surf[surf][1]], p) >= 0.0
		&& sideOf(bound_vec[bound_surf[surf][1]], bound_vec[bound_surf[surf][2]], p) >= 0.0
		&& sideOf(bound_vec[bound_surf[surf][2]], bound_vec[bound_surf[surf][0]], p) >= 0.0;
}
*/

// 3D
int inside(int surf, GLfloat p[])
{
	// [###]
	return 0;
}

void init_surf(void)
{
	for (int i = 0; i < bound_num_surf; ++i) {
		calc_ABCD(bound_surf_abcd[i],
			bound_vec[bound_surf[i][0]],
			bound_vec[bound_surf[i][0]],
			bound_vec[bound_surf[i][0]]);
	}
}

void * thread_update(void * ptr)
{
	GLfloat d[3];
	GLfloat d1[3];
	GLfloat t[3];
	float dl;
	GLfloat u;
	int s;
	while (1) {
		d[0] = ball_dir[0] * 0.01;
		d[1] = ball_dir[1] * 0.01;
		d[2] = ball_dir[2] * 0.01;
		dl = sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
		for (s = 0; s < bound_num_surf; ++s) {
			float u = calc_u(s, ball_pos, d);
			if (u < 0 && fabs(u) <= dl) {
				t[0] = ball_pos[0] + u * d[0];
				t[1] = ball_pos[1] + u * d[1];
				t[2] = ball_pos[2] + u * d[2];
				if (inside(s, t)) {
					break;
				}
			}
		}
		if (s < bound_num_surf) {
			ball_pos[0] = t[0]; ball_pos[1] = t[1]; ball_pos[2] = t[2];
			u = (dl - u) / dl;
			d[0] *= u;
			d[1] *= u;
			d[2] *= u;
			calc_mirror(d1, s, d);
			d[0] = d1[0];
			d[1] = d1[1];
			d[2] = d1[2];
		}
		ball_pos[0] += d[0];
		ball_pos[1] += d[1];
		ball_pos[2] += d[2];
		usleep(10000);	// 10ms
	}
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	init_surf();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(300, 300);
	glutCreateWindow("[PONG]");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
//	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	pthread_t t_update;
	pthread_create(&t_update, 0, thread_update, (void *)0);

	glutMainLoop();
	pthread_join(t_update, 0);
	return 0;
}


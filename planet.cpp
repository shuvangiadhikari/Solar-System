#include "planet.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#pragma comment(lib, "glew32.lib")


Planet::Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni) {
		radius = _radius;
		distance = _distance;
		orbit = _orbit;
		orbitSpeed = _orbitSpeed;
		axisTilt = _axisTilt;
		axisAni = _axisAni;
}

void Planet::drawSmallOrbit(void) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, distance, 100.0, 100.0);
		glPopMatrix();
}

void Planet::drawMoon(void) {
		GLUquadricObj* quadric;
		quadric = gluNewQuadric();
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(orbit, 0.0, 1.0, 0.0);
		glTranslatef(distance, 0.0, 0.0);
		gluSphere(quadric, radius, 20.0, 20.0);
		glPopMatrix();
}


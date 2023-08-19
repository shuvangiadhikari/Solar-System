#include <iostream>
#include <cstdlib> // for rand() function for random number generator in orbit position
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "imageloader.h"
#include "planet.h"
#pragma comment(lib, "glew32.lib")

using namespace std;



//Sun, Planets and Stars
Planet sun(5.0, 0, 0, 0, 0, 0);				//Sun
Planet mer(1.0, 7, rand() % 360, 4.74, 02.11, 0);		//Mercury
Planet ven(1.5, 11, rand() % 360, 3.50, 177.0, 0);		//Venus
Planet ear(2.0, 16, rand() % 360, 2.98, 23.44, 0);		//Earth
Planet mar(1.2, 21, rand() % 360, 2.41, 25.00, 0);		//Mars
Planet jup(3.5, 30, rand() % 360, 1.31, 03.13, 0);		//Jupiter
Planet sat(3.0, 40, rand() % 360, 0.97, 26.70, 0);		//Saturn
Planet ura(2.5, 49.5, rand() % 360, 0.68, 97.77, 0);	//Uranus
Planet nep(2.3, 53.6, rand() % 360, 0.54, 28.32, 0);	//Neptune
Planet lun(.40, 3, 0, 5.40, 0, 0);			//Luna     (Earth)

int isAnimate = 1;
int bigOrbitActive = 0;
int smallOrbitActive = 0;
int moonsActive = 1;
int changeCamera = 0;
int frameCount = 0;
int labelsActive = 0;
int zoom = 50;
int logoScene = 1;

float lightPos[] = { 0.0, 0.0, -50.0, 1.0 }; // Spotlight position.
static float spotAngle = 180; // Spotlight cone half-angle.
float spotDirection[] = { 1.0, 1.0, 1.0 }; // Spotlight direction.
static float spotExponent = 1.0; // Spotlight exponent = attenuation factor.

GLuint loadTexture(Image* image) {
	//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

GLuint sunTexture, merTexture, venTexture, earTexture, marTexture, jupTexture, satTexture, uraTexture, nepTexture, pluTexture, staTexture, logTexture, kriTexture;

void writeBitmapString(void* font, char* string)
{
	char* c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c); // writes string character by character
}

void setup(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	//TEXUTRING SETUP
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Image* sta = loadBMP("stars.bmp");		staTexture = loadTexture(sta);		delete sta;
	Image* sun = loadBMP("sunmap.bmp");		sunTexture = loadTexture(sun);		delete sun;
	Image* mer = loadBMP("mercury.bmp");	merTexture = loadTexture(mer);		delete mer;
	Image* ven = loadBMP("venus.bmp");		venTexture = loadTexture(ven);		delete ven;
	Image* ear = loadBMP("earthmap.bmp");		earTexture = loadTexture(ear);		delete ear;
	Image* mar = loadBMP("mars.bmp");		marTexture = loadTexture(mar);		delete mar;
	Image* jup = loadBMP("jupitermap.bmp");	jupTexture = loadTexture(jup);		delete jup;
	Image* sat = loadBMP("saturnmap.bmp");		satTexture = loadTexture(sat);		delete sat;
	Image* ura = loadBMP("uranus.bmp");		uraTexture = loadTexture(ura);		delete ura;
	Image* nep = loadBMP("neptunemap.bmp");	nepTexture = loadTexture(nep);		delete nep;
	Image* log = loadBMP("logo.bmp");		logTexture = loadTexture(log);		delete log;

	glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 }; // ambient lighting is set to full black
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 }; // source lighting is set to full white
	//float globAmb[] = { 0.1, 0.1, 0.1, 1.0 }; // how much illumination when there is no light at all
	float globAmb[] = { 0.1, 0.1, 0.1, 1.0 }; // how much illumination when there is no light at all
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // so that lighting model takes viewer's position into account
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);

}

void orbitalTrails(void) {
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mer.distance, 100.0, 100.0);
	glutWireTorus(0.001, ven.distance, 100.0, 100.0);
	glutWireTorus(0.001, ear.distance, 100.0, 100.0);
	glutWireTorus(0.001, mar.distance, 100.0, 100.0);
	glutWireTorus(0.001, jup.distance, 100.0, 100.0);
	glutWireTorus(0.001, sat.distance, 100.0, 100.0);
	glutWireTorus(0.001, ura.distance, 100.0, 100.0);
	glutWireTorus(0.001, nep.distance, 100.0, 100.0);
	glPopMatrix();
}

void asteroidBelt(void) {
	glPushMatrix();
	glColor3ub(100, 100, 100);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.1, mar.distance + 2.0, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 2.2, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 2.4, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 2.6, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 2.8, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 3.0, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 3.2, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 3.4, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 3.6, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 3.8, 100.0, 100.0);
	glutWireTorus(0.1, mar.distance + 4.0, 100.0, 100.0);
	glColor3ub(255, 255, 255);
	glPopMatrix();
}

void kuiperBelt(void) {
	glPushMatrix();
	glColor3ub(100, 100, 100);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, nep.distance + 3.0, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 3.2, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 3.4, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 3.6, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 3.8, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 4.0, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 4.2, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 4.4, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 4.6, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 4.8, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 5.0, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 5.2, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 5.4, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 5.6, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 5.8, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 2 * 3.0, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 2 * 3.2, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 2 * 3.4, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 2 * 3.6, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 2 * 3.8, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 2 * 4.0, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 2 * 4.2, 100.0, 100.0);
	glutWireTorus(0.2, nep.distance + 2 * 4.4, 100.0, 100.0);
	glColor3ub(255, 255, 255);
	glPopMatrix();
}

void drawLogoScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, logTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-100, -100, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(100, -100, -100);
	glTexCoord2f(1.0, 1.0); glVertex3f(100, 100, -100);
	glTexCoord2f(0.0, 1.0); glVertex3f(-100, 100, -100);
	glEnd();

	glutSwapBuffers();
}

float x_zoom = 163;
float y_zoom = zoom;
float z_zoom = 50.0;

void ShreeKrishnaLips(void) {
	glPushMatrix();
	glColor3ub(214, 91, 91);
	glTranslatef(80.0, 10.0, 25.0);
	glRotatef(90.0, 0.0, 1.0, 0.0); // rotate 90 degrees about Y-axis
	glRotatef(-23.4985, 0.0, 1.0, 0.0);
	glRotatef(23.985, 0.0, 0.0, 1.0);
	glScalef(1.0, 0.8, 2.0);

	glutSolidTorus(10.0, 50.0, 50.0, 50.0);
	glColor3ub(255, 255, 255);
	
	glPopMatrix();
}

void ShreeKrishnaTeeth(void) {
	glPushMatrix();
	
	glColor3ub(255, 255, 255);
	glTranslatef(90, 36.0, 3.0);
	glutSolidCube(10.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(89.0, 39.0, 13.0);
	glutSolidCube(10.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(86, 40.0, 23.0);
	glutSolidCube(10.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(85.0, 40.0, 33.0);
	glutSolidCube(10.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(80.0, 39.0, 43.0);
	glutSolidCube(10.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(78.0, 34.0, 51.0);
	glutSolidCube(8.0);
	glPopMatrix();

	//tala ko daat
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(93.0, -5, 1.0);
	glutSolidCube(8.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(92.0, -8, 9.0);
	glutSolidCube(10.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(91.0, -10, 19.0);
	glutSolidCube(10.0);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(87.0, -12, 29.0);
	glutSolidCube(10.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(80.0, -14.5, 39.0);
	glutSolidCube(10.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(70, -18.0, 49.0);
	glutSolidCube(15.0);
	glPopMatrix();
}

void drawScene(void) {
	frameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (changeCamera == 0)gluLookAt(x_zoom, y_zoom, z_zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	cout << x_zoom << ", " << y_zoom << ", " << z_zoom << endl;
	if (bigOrbitActive == 1) orbitalTrails();

	GLUquadric* quadric;
	quadric = gluNewQuadric();

	//Sun
	glPushMatrix();
	glRotatef(sun.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sun.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(-1.2, 7.0, 0.0); // position of text
		glColor3ub(255, 255, 255); // color of text
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Sun");
	}
	glPushMatrix();
	glRotatef(sun.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sun.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sun.radius, 20.0, 20.0); // sun is divided into 20 vertical and 20 horizontal parts for smooth rendering
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Mercury
	glPushMatrix();
	glRotatef(mer.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mer.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Mercury");
	}
	glPushMatrix();
	glRotatef(mer.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mer.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, merTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mer.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Venus
	glPushMatrix();
	glRotatef(ven.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ven.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Venus");
	}
	glPushMatrix();
	glRotatef(ven.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ven.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ven.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Earth, Orbit, Moon
	glPushMatrix();
	glRotatef(ear.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ear.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Earth");
	}
	glPushMatrix();
	glRotatef(ear.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ear.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, earTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ear.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		lun.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		lun.drawMoon();
	}
	glPopMatrix();

	//Mars
	glPushMatrix();
	glRotatef(mar.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mar.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Mars");
	}
	glPushMatrix();
	glRotatef(mar.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mar.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, marTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mar.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	// Asteroid Belt
	asteroidBelt();


	//Jupiter
	glPushMatrix();
	glRotatef(jup.orbit, 0.0, 1.0, 0.0);
	glTranslatef(jup.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Jupiter");
	}
	glPushMatrix();
	glRotatef(jup.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(jup.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, jup.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Saturn
	glPushMatrix();
	glRotatef(sat.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sat.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Saturn");
	}
	glPushMatrix();
	glRotatef(sat.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sat.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, satTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sat.radius, 20.0, 20.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3ub(158, 145, 137);
	glRotatef(-63.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, 4.0, 30.0, 30.0); // for rings of Saturn (radiusOfThickness, radiusOfTorusFromCenter, fineDetails, fineDetails)
	glutWireTorus(0.5, 4.5, 30.0, 30.0);
	glutWireTorus(0.5, 5.0, 30.0, 30.0);
	glutWireTorus(0.4, 3.0, 30.0, 30.0);
	glPopMatrix();
	glPopMatrix();

	glColor3ub(255, 255, 255);		//FIXES SHADING ISSUE

	//Uranus
	glPushMatrix();
	glRotatef(ura.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ura.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Uranus");
	}
	glPushMatrix();
	glRotatef(ura.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ura.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uraTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ura.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Neptune
	glPushMatrix();
	glRotatef(nep.orbit, 0.0, 1.0, 0.0);
	glTranslatef(nep.distance, 0.0, 0.0);
	if (labelsActive == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Neptune");
	}
	glPushMatrix();
	glRotatef(nep.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(nep.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, nepTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, nep.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	ShreeKrishnaLips();
	ShreeKrishnaTeeth();

	kuiperBelt();


	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, staTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-200, -200, -100);
	glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, staTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200, -83, 200);
	glTexCoord2f(8.0, 0.0); glVertex3f(200, -83, 200);
	glTexCoord2f(8.0, 8.0); glVertex3f(200, -83, -200);
	glTexCoord2f(0.0, 8.0); glVertex3f(-200, -83, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glutSwapBuffers();
}

void drawScenesInOrder(void) {
	drawScene();
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void animate(int n) {
	if (isAnimate) {
		mer.orbit += mer.orbitSpeed;
		ven.orbit += ven.orbitSpeed;
		ear.orbit += ear.orbitSpeed;
		mar.orbit += mar.orbitSpeed;
		jup.orbit += jup.orbitSpeed;
		sat.orbit += sat.orbitSpeed;
		ura.orbit += ura.orbitSpeed;
		nep.orbit += nep.orbitSpeed;
		if (mer, ven, ear, mar, jup, sat, ura, nep.orbit > 360.0) {
			mer, ven, ear, mar, jup, sat, ura, nep.orbit -= 360.0;
		}
		mer.axisAni += 0.170648;
		ven.axisAni += 0.041152;
		ear.axisAni += 10.0;
		mar.axisAni += 9.7087;
		jup.axisAni += 24.39024;
		sat.axisAni += 22.222;
		ura.axisAni += 13.8889;
		nep.axisAni += 14.9257;
		
		if (mer, ven, ear, mar, jup, sat, ura, nep.axisAni > 360.0) {
			mer, ven, ear, mar, jup, sat, ura, nep.axisAni -= 360.0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, animate, 1);
	}
}

void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		if (logoScene) logoScene = 0;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);
	glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && zoom < 100) x_zoom++;
	if (direction < 0 && zoom > -75) x_zoom--;
	glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27: exit(0); break;
	case 'p': if (isAnimate) isAnimate = 0; else { isAnimate = 1; animate(1); } break;
	case 'o': if (smallOrbitActive) smallOrbitActive = 0; else smallOrbitActive = 1; glutPostRedisplay(); break;
	case 'O': if (bigOrbitActive) bigOrbitActive = 0; else bigOrbitActive = 1; glutPostRedisplay(); break;
	case 'm': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;
	case 'M': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;
	case 'l': if (labelsActive) labelsActive = 0; else labelsActive = 1; glutPostRedisplay(); break;
	case 'L': if (labelsActive) labelsActive = 0; else labelsActive = 1; glutPostRedisplay(); break;
	case '1': changeCamera = 0; glutPostRedisplay(); break;
	}
}

void intructions(void) {
	cout << "p = Pause/Play" << endl;
	cout << "ESC = Exit" << endl;
	/*cout << "O = show/hide orbits" << endl;*/
	cout << "M/m = show/hide Moons." << endl;
	cout << "L/l to show/hide labels" << endl;
	cout << "Mouse Scroll = change camera" << endl;
	cout << x_zoom << ", " << y_zoom << ", " << z_zoom << endl;
}

int main(int argc, char** argv) {
	intructions();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Solar System Simulation");
	glutDisplayFunc(drawScenesInOrder);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);
	glutMouseWheelFunc(mouseWheel);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();
}
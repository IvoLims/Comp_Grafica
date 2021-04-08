#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(1, timer, 0);
}

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

void drawTree() {
	int r = 50;
	for (int i = 0; i < 100; i++) {
		glPushMatrix();
		double posx = 0, posz = 0;
		while (pow(posx, 2) + pow(posz, 2) < pow(r, 2)) {
			posx = (rand() % (r * 4)) - (r * 2);
			posz = (rand() % (r * 4)) - (r * 2);
		}
		glTranslated(posx, 0, posz);
		glRotatef(-90, 1, 0, 0);
		glColor3ub(102, 62, 36);
		glutSolidCone(0.5, 2, 10, 10);
		glTranslatef(0, 0, 2);
		glColor3ub(0, 255, 0);
		glutSolidCone(2, 4, 10, 10);
		glPopMatrix();
	}
}

void drawIndians() {
	for (int i = 0; i < 16; i++) {
		glPushMatrix();
		glTranslatef(0, 3, 0);
		glRotatef((360 / 16) * i, 0, 1, 0);
		glTranslatef(35, 1, 0);
		glColor3f(0.9, 0, 0);
		glutSolidTeapot(2);
		glPopMatrix();
	}
}

void drawCowBoys() {
	for (int i = 0; i < 16; i++) {
		glPushMatrix();
		glTranslatef(0, 3, 0);
		glRotatef((360 / 16) * i, 0, 1, 0);
		glTranslatef(15, 1, 0);
		glColor3f(0.3, 0, 1);
		glutSolidTeapot(2);
		glPopMatrix();
	}
}

void drawCenter() {
	glColor3ub(255, 0, 255);
	glutSolidTorus(1, 2, 10, 10);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
	glVertex3f(100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, 100.0f);

	glVertex3f(100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, 100.0f);
	glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	// End of frame


	// put code to draw scene in here

	drawTree();
	drawIndians();
	drawCowBoys();
	drawCenter();

	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char** argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	spherical2Cartesian();

	printInfo();

	glutTimerFunc(0, timer, 0);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}

#include <math.h>
#include <ctime>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>#include <math.h>
#include <ctime>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1415926535897932384626433832795

//For Camera

#define GlUT_KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

/* Explorer camera

// angle of rotation for the camera direction
float angle = PI/4;  //alpha
float angle1 = PI/4; //beta
// actual vector representing the camera's direction
float dx = 0.0f;
float dy = 0.0f;
float dz = 0.0f;
// XZ position of the camera
float px = 0.0f;
float py = 0.0f;
float pz = 0.0f;
float radium = 17.0f;
*/

//FPS camera
float angle = PI/4 ;  //alpha
float angle1 = PI/4 ; //beta
// actual vector representing the camera's direction
float dx = 0.0f;
float dy = 0.0f;
float dz = 0.0f;
// XZ position of the camera
float px = 25.0f;
float py = 0.0f;
float pz = 10.0f;
float radium = 17.0f;
float speed = 1.0f;
float rotateSpeed = 0.0008f;

//For FPS
int timebase;
float frame;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
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
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

// Draw Figures

void drawAxis() {
	glBegin(GL_LINES);
	// X axis in Red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glEnd();
}

void drawPlane(float width) {
	width/=2;

	glBegin(GL_TRIANGLE_FAN);

	glColor3f(4.0f, 1.0f, 1.0f);

	glVertex3d(width, 0,width);
	glVertex3d(width, 0, -width);
	glVertex3d(-width, 0,-width);

	glColor3f(3.0f, 1.0f, 0.0f);
	
	glVertex3d(width, 0, width);
	glVertex3d(-width, 0, -width);
	glVertex3d(-width, 0, width);
	
	glColor3f(0.0f, 0.0f, 1.0f);
	
	glVertex3d(width, 0, -width);
	glVertex3d(width, 0, width);
	glVertex3d(-width, 0, -width);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glVertex3d(-width, 0, -width);
	glVertex3d(width, 0, width);
	glVertex3d(-width, 0, width);

	glEnd();
}


void drawCylinder(float radius, float height, int slices) {
	float interval = 2 * PI / slices;
	float next_a, next_h;

	for (float a = 0; a < 2 * PI; a += interval) {
		next_a = a + interval;
		if (next_a > 2 * PI) {
			next_a = 2 * PI;
		}

		glBegin(GL_TRIANGLES);
		//Top
		glColor3f(0.698f, 0.133f, 0.133f);

		glVertex3f(0.0f, height / 2, 0.0f);
		glVertex3f(radius * sin(a), height / 2, radius * cos(a));
		glVertex3f(radius * sin(next_a), height / 2, radius * cos(next_a));

		//Bottom
		glColor3f(0.698f, 0.133f, 0.133f);

		glVertex3f(0.0f, -height / 2, 0.0f);
		glVertex3f(radius * sin(next_a), -height / 2, radius * cos(next_a));
		glVertex3f(radius * sin(a), -height / 2, radius * cos(a));

		for (float h = -height / 2; h < height / 2; h += height) {
			next_h = h + height;
			if (next_h > height / 2) {
				next_h = height / 2;
			}
			//Walls
			glColor3f(1.0f, 0.271f, 0.0f);

			glVertex3f(radius * sin(next_a), next_h, radius * cos(next_a));
			glVertex3f(radius * sin(a), next_h, radius * cos(a));
			glVertex3f(radius * sin(next_a), h, radius * cos(next_a));

			glColor3f((a + 0.05) / (2 * PI), (a + 0.3) / (2 * PI), (h + height / 2) / height);

			glVertex3f(radius * sin(a), next_h, radius * cos(a));
			glVertex3f(radius * sin(a), h, radius * cos(a));
			glVertex3f(radius * sin(next_a), h, radius * cos(next_a));

		}
		glEnd();
	}
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	/*Explorer Mode Camera
	px = radium * cosf(angle1) * sinf(angle);
	py = radium * sinf(angle1);
	pz = radium * cosf(angle) * cosf(angle1);
	gluLookAt(px, py, pz, dx, dy, dz, 0.0f, 1.0f, 0.0f);
	*/

	//FPS Camera
	//float rotx = px + dx; float roty = py + dy; float rotz = pz + dz;
    //gluLookAt(px, py, pz, rotx, roty, rotz, 0.0f, 1.0f, 0.0f);

	gluLookAt(px, py, pz, px+dx, py+dy, pz+dz, 0.0f, 1.0f, 0.0f);
	
	
// put the geometric transformations here


// put drawing instructions here
	drawAxis();
	drawPlane(4);
	drawCylinder(1,3,30);

	//FPS counter
	frame++;
	int final_time = glutGet(GLUT_ELAPSED_TIME);

	if (final_time - timebase > 1000) {
		int fps = frame * 1000.0f / (final_time - timebase);
		char title[(((sizeof fps) * CHAR_BIT) + 2) / 3 + 2];
		sprintf(title,"FPS: %d", fps);
		glutSetWindowTitle(title);
		timebase = final_time;
		frame = 0;
	}

	// End of frame
	glutSwapBuffers();

}


// write function to process keyboard events
/*Explorer Mode Camera
* 
void processSpecialKeys(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_RIGHT: angle = angle + 0.05;
		break;
	case GLUT_KEY_LEFT: angle = angle - 0.05;
		break;
	case GLUT_KEY_UP:
		if (angle1 + 0.05 >= 1.5)
			angle1 = 1.5;
		else
			angle1 = angle1 + 0.05;
		break;
	case GLUT_KEY_DOWN:
		if (angle1 - 0.05 <= -1.5)
			angle1 = -1.5;
		else
			angle1 = angle1 - 0.05;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

*/
//FPS Camera

void move(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT: {
		px -= (dz * speed);
		pz += (dx * speed);
		break;
	}
	case GLUT_KEY_LEFT: {
		px += (dz * speed);
		pz -= (dx * speed);
		break;
	}
	case GLUT_KEY_UP: {
		px += (dx * speed);
		pz += (dz * speed);
		break;
	}
	case GLUT_KEY_DOWN: {
		px -= (dx * speed);
		pz -= (dz * speed);
		break;
	}
	default: {
		break;
	}
}
	glutPostRedisplay();
}

void mouse_motion(int x, int y) {
		float lx = x - 100;
		float ly = y - 100;
		angle += (lx * rotateSpeed);
		angle1 += (ly * rotateSpeed);
		dx = radium * cos(angle1) * sin(angle);
	    dy = radium * sin(angle1);
	    dz = radium * cos(angle) * cos(angle1);
	}



int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG");
	

// Required callback registry 
	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//FPS 
	timebase = glutGet(GLUT_ELAPSED_TIME);

// put here the registration of the keyboard callbacks

	/*Explorer Mode Camera
	glutSpecialFunc(processSpecialKeys);*/

	glutSpecialFunc(move);
	glutMotionFunc(mouse_motion);
	//glutPassiveMotionFunc(mouse_motion);
	
//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}

#else
#include <GL/glut.h>
#endif

#define PI 3.1415926535897932384626433832795

struct Polar {
	double radius;
	double alpha;
	double beta;
};

Polar camPos = { sqrt(100), PI, PI };

double polarX(Polar polar) { return polar.radius * cos(polar.beta) * sin(polar.alpha); }
double polarY(Polar polar) { return polar.radius * sin(polar.beta); }
double polarZ(Polar polar) { return polar.radius * cos(polar.beta) * cos(polar.alpha); }

//For FPS
int timebase;
float frame;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
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
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

// Draw Figures

void drawAxis() {
	glBegin(GL_LINES);
	// X axis in Red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glEnd();
}

void drawPlane(float width) {
	width/=2;

	glBegin(GL_TRIANGLE_FAN);

	glColor3f(4.0f, 1.0f, 1.0f);

	glVertex3d(width, 0,width);
	glVertex3d(width, 0, -width);
	glVertex3d(-width, 0,-width);

	glColor3f(3.0f, 1.0f, 0.0f);
	
	glVertex3d(width, 0, width);
	glVertex3d(-width, 0, -width);
	glVertex3d(-width, 0, width);
	
	glColor3f(0.0f, 0.0f, 1.0f);
	
	glVertex3d(width, 0, -width);
	glVertex3d(width, 0, width);
	glVertex3d(-width, 0, -width);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glVertex3d(-width, 0, -width);
	glVertex3d(width, 0, width);
	glVertex3d(-width, 0, width);

	glEnd();
}


void drawCylinder(float radius, float height, int slices) {
	float interval = 2 * PI / slices;
	float next_a, next_h;

	for (float a = 0; a < 2 * PI; a += interval) {
		next_a = a + interval;
		if (next_a > 2 * PI) {
			next_a = 2 * PI;
		}

		glBegin(GL_TRIANGLES);
		//Top
		glColor3f(0.698f, 0.133f, 0.133f);

		glVertex3f(0.0f, height / 2, 0.0f);
		glVertex3f(radius * sin(a), height / 2, radius * cos(a));
		glVertex3f(radius * sin(next_a), height / 2, radius * cos(next_a));

		//Bottom
		glColor3f(0.698f, 0.133f, 0.133f);

		glVertex3f(0.0f, -height / 2, 0.0f);
		glVertex3f(radius * sin(next_a), -height / 2, radius * cos(next_a));
		glVertex3f(radius * sin(a), -height / 2, radius * cos(a));

		for (float h = -height / 2; h < height / 2; h += height) {
			next_h = h + height;
			if (next_h > height / 2) {
				next_h = height / 2;
			}
			//Walls
			glColor3f(1.0f, 0.271f, 0.0f);

			glVertex3f(radius * sin(next_a), next_h, radius * cos(next_a));
			glVertex3f(radius * sin(a), next_h, radius * cos(a));
			glVertex3f(radius * sin(next_a), h, radius * cos(next_a));

			glColor3f((a + 0.05) / (2 * PI), (a + 0.3) / (2 * PI), (h + height / 2) / height);

			glVertex3f(radius * sin(a), next_h, radius * cos(a));
			glVertex3f(radius * sin(a), h, radius * cos(a));
			glVertex3f(radius * sin(next_a), h, radius * cos(next_a));

		}
		glEnd();
	}
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(polarX(camPos), polarY(camPos), polarZ(camPos), 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

// put the geometric transformations here


// put drawing instructions here
	drawAxis();
	drawPlane(4);
	drawCylinder(1,3,30);

	//FPS counter
	frame++;
	int final_time = glutGet(GLUT_ELAPSED_TIME);

	if (final_time - timebase > 1000) {
		int fps = frame * 1000.0f / (final_time - timebase);
		char title[(((sizeof fps) * CHAR_BIT) + 2) / 3 + 2];
		sprintf(title,"FPS: %d", fps);
		glutSetWindowTitle(title);
		timebase = final_time;
		frame = 0;
	}

	// End of frame
	glutSwapBuffers();

}


// write function to process keyboard events

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		camPos.alpha -= PI / 16;
		break;
	case 'd':
		camPos.alpha += PI / 16;
		break;
	case 'w':
		if (camPos.beta - PI/16 >= -PI/2) {
			camPos.beta -= PI / 16;
		}
		break;
	case 's':
		if (camPos.beta + PI/16 <= PI/2) {
			camPos.beta += PI / 16;
		}
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG");
	

// Required callback registry 
	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//FPS 
	timebase = glutGet(GLUT_ELAPSED_TIME);

// put here the registration of the keyboard callbacks
	glutKeyboardFunc(keyboardFunc);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}


	
// put here the registration of the keyboard callbacks



//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}

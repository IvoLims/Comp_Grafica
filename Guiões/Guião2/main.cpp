#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

#define PI 3.1415926535897932384626433832795

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

// draw figures

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
		glColor3f((a + 0.05) / (2 * PI), (a + 0.3) / (2 * PI), height / 2);

		glVertex3f(0.0f, height / 2, 0.0f);
		glVertex3f(radius * sin(a), height / 2, radius * cos(a));
		glVertex3f(radius * sin(next_a), height / 2, radius * cos(next_a));

		//Bottom
		glColor3f((a + 0.05) / (2 * PI), (a + 0.3) / (2 * PI), 0.0f);

		glVertex3f(0.0f, -height / 2, 0.0f);
		glVertex3f(radius * sin(next_a), -height / 2, radius * cos(next_a));
		glVertex3f(radius * sin(a), -height / 2, radius * cos(a));

		for (float h = -height / 2; h < height / 2; h += height) {
			next_h = h + height;
			if (next_h > height / 2) {
				next_h = height / 2;
			}
			//Walls
			glColor3f((h + height / 2) / height, (a + 0.3) / (2 * PI), (h + height / 2) / height);

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
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

// put the geometric transformations here


// put drawing instructions here
	drawPlane(4);
	drawCylinder(1,3,30);

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events






int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks



//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}

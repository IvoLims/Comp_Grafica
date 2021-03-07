#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

#define GlUT_KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

// Variáveis

float angulo_Y;
float angulo_X;
float anda_X;
float anda_Z;
float scale_All = 1;

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


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
	// Eixos
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

// put the geometric transformations here
	glRotatef(angulo_Y,0,1,0);
	glRotatef(angulo_X, 1, 0, 0);
	glTranslatef(anda_X, 0, anda_Z);
	glScalef(scale_All, scale_All, scale_All);

// put drawing instructions here

	//Formar Triângulos
	glBegin(GL_TRIANGLES);
	
	// Parte de Base 
	// Formato: (x,y,z)
	// 1º Triângulo 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	// 2º Triângulo
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	// Triângulo da Frente
	glColor3f(0.13f, 0.54f, 0.13f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	// Triângulo da Direita
	glColor3f(0.0f, 0.19f, 0.58f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	// Triângulo de Trás
	glColor3f(0.9f, 0.39f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	// Triângulo da Esquerda
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);

	glEnd();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void rodar(unsigned char key, int x, int y) {
	switch (key) {
	case 'a': angulo_Y += 1; break;
	case 'd': angulo_Y -= 1; break;
	case 'w': angulo_X += 1; break;
	case 's': angulo_X -= 1; break;
	default:
		break;
	}
	glutPostRedisplay();
}

void escala(unsigned char key, int x, int y) {
	switch (key) {
	case '+': scale_All += 0.2; break;
	case '-': scale_All -= 0.2; break;
	default:
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	rodar(key, x, y);
	escala(key, x, y);
}


void move(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT: anda_X += 1; break;
	case GLUT_KEY_LEFT: anda_X -= 1; break;
	case GLUT_KEY_UP: anda_Z += 1; break;
	case GLUT_KEY_DOWN: anda_Z -= 1; break;
	default:
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
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

// put here the registration of the keyboard callbacks

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(move);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}

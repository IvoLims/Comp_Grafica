#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


//variàveis utilizadas para a formatação da câmara
float px = 0;
float py = 0;
float pz = 0;

float dx = 0;
float dy = 0;
float dz = 0;

//ângulos utilizados para a formatação da câmara
float alpha = 0;
float beta = M_PI/4;
float radium = 20.0;


//Variaveis para a VBO
int n_pontos;
GLuint buffer[1];

//Carregar a Imagem
unsigned int t, tw, th;
unsigned char *imageData;


void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective
    gluPerspective(45,ratio,1,1000);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


float h(int coluna, int linha){
    return imageData[linha*tw+coluna]/255.0 * 100;
}


void terrainVBO(float altura, float largura){
    float *vertexB=NULL, altura_aux=altura,largura_aux;
    int coluna,linha,i=0;

    altura/=th;
    largura/=tw;

    n_pontos=(tw*2*(th-1))*3;
    vertexB=(float*)malloc(n_pontos*sizeof(float));


    glEnableClientState(GL_VERTEX_ARRAY);

    for (linha=0; linha<th-1; linha++) {
        largura_aux=0;
        for (coluna=0; coluna<tw; coluna++) {

            vertexB[i++]=altura_aux;
            vertexB[i++]=h(coluna,linha);
            vertexB[i++]=largura_aux;

            vertexB[i++]=altura_aux-altura;
            vertexB[i++]=h(coluna,linha+1);
            vertexB[i++]=largura_aux;

            largura_aux+=largura;
        }
        altura_aux-=altura;
    }

    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, n_pontos*sizeof(float), vertexB, GL_STATIC_DRAW);
    free(vertexB);


}

void drawTerrain(){
    int i;
    glBindBuffer(GL_ARRAY_BUFFER,buffer[0]);
    glVertexPointer(3,GL_FLOAT,0,0);
    glPushMatrix();
    glTranslatef(-(tw/2.0), 0, -(th/2.0));

    for (i=0; i<th; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i*tw*2, 2*tw);
    }
    glPopMatrix();

}


void renderScene(void) {

    float pos[4] = {-1.0, 1.0, 1.0, 0.0};

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    px = radium*cosf(beta)*sinf(alpha);
    py = radium*sinf(beta);
    pz = radium*cosf(alpha)*cosf(beta);

    gluLookAt(px + dx, py + dy, pz + dz,
              dx, dy, dz,
              0.0f, 1.0f, 0.0f);
    glColor3f(0, 1, 0);
    drawTerrain();

// End of frame
    glutSwapBuffers();
}


void carregarImagem(){

    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)"terreno.jpg");
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    imageData = ilGetData();
}

void processSpecialKeys(int key_code, int xx, int yy) {
    switch (key_code) {
        case GLUT_KEY_RIGHT: alpha = alpha + 0.05;
            break;
        case GLUT_KEY_LEFT: alpha = alpha - 0.05;
            break;
        case GLUT_KEY_UP:
            if (beta + 0.05 >= 1.5)
                beta = 1.5;
            else
                beta = beta + 0.05;
            break;
        case GLUT_KEY_DOWN:
            if (beta - 0.05 <= -1.5)
                beta = -1.5;
            else
                beta = beta - 0.05;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int axis = 0;
void processKeys(unsigned char c, int xx, int yy) {
    switch (c) {
        case 'd': dx += 0.1;
            break;
        case 'a': dx -= 0.1;
            break;
        case 'w': dy += 0.1;
            break;
        case 's': dy -= 0.1;
            break;
        case 'q': dz -= 0.1;
            break;
        case 'e': dz += 0.1;
            break;
        case 'z':
            if(axis==0) axis=1;
            else axis=0;
            break;

        default:
            break;
    }
    glutPostRedisplay();
}

void Mouse_Func(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (radium - 1 == 0) break;
            else radium --;
            break;
        case GLUT_RIGHT_BUTTON:
            radium ++;
            break;
    }
    glutPostRedisplay();
}


int main(int argc, char **argv) {

    // pÙr inicializaÁ„o aqui
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("CG@DI-UM");


    // Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(Mouse_Func);



    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glPolygonMode(GL_FRONT,GL_POINT);

    glewInit();
    ilInit();

    //Carregar Imagem e construir a VBO
    carregarImagem();
    terrainVBO(th,tw);
    // enter GLUT's main cycle
    glutMainLoop();

    return 0;
}

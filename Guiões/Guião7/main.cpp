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
#include <ctime>

#endif


//variàveis utilizadas para a formatação da câmara
float px = 0;
float py = 0;
float pz = 0;

float dx = 0;
float dy = 0;
float dz = 0;


//Angulo de rotação dos centros
float cir1=0,cir2=0;

//ângulos utilizados para a formatação da câmara
float alpha = 0;
float beta = M_PI/4;
float radium = 20.0;


float alfa2 = 0.0f, beta2 = 0.5f, radius = 100.0f, angle = 0;
float camX, camY, camZ;
float *translacoes;

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

/** arvores e bonecos*/

void arvore(){
    int pos = 0;
    float x,z;
    float g1=0.5;
    float g2=0.6;


        glPushMatrix();

        glColor3f(0.7, 0.3, 0);
        glRotatef(-90,1,0,0);
        glutSolidCone(1,3,20,20);

        glTranslatef(0, 0, 2);
        if ((x=rand()%3) == 0){
            glColor3f(0, g1, 0);
        }
            else{
                glColor3f(0.0, g2, 0.2);
        }

        glutSolidCone(3,5,20,10);
        glPopMatrix();

}

void arvoresPosicao(){

    int x,z;
    int i=100;
    srand(4); //Inicia a sequência aleatória para ser sempre a mesma a sequência a ser gerada
    while (i>0) {
        x=rand()%th/2-1;
        z=rand()%tw/2-1;

        //Caso tenha árvores com coordenadas que coincidem no circulo interior
        if(sqrt(x*x+z*z)<63){
            if(rand()%2==0)
                x=rand()%49+63;
            else
                z=rand()%49+63;

        }

        if (rand()%2==0)
            x=-x;
        if (rand()%2==0)
            z=-z;

        //Fazemos um Push para preservar o ponto de origem
        glPushMatrix();
        glTranslatef(x, h((z+tw/2),256-(x+th/2)), z); //Translate para a posição pretendida
        arvore();
        glPopMatrix();
        i--;

    }
}



void desenhaTeapots(int nTeapots){
    glPushMatrix();

    glRotatef(cir2,0,1,0);
    for(int teapot=0; teapot<nTeapots; teapot++) {
        float alpha2 = teapot * 360/nTeapots;
        glPushMatrix();
        glTranslatef(0,0,0);
        glRotatef(alpha2,0,1,0);
        glTranslatef(30,1.3,0);
        glColor3f(0.9,0,0);
        glutSolidTeapot(2);
        glPopMatrix();
    }
    glPopMatrix();
}

void desenhaTeapotsCentro(int nTeapots){
    glPushMatrix();
    glRotatef(cir1,0,1,0);
    for(int teapot = 0; teapot<nTeapots; teapot++) {
        float alpha2 = teapot * 360/nTeapots;
        glPushMatrix();
        glTranslatef(0,0,0);
        glRotatef(alpha2,0,1,0);
        glTranslatef(15,1.3,0);
        glColor3f(0.3,0,1);
        glutSolidTeapot(2);
        glPopMatrix();
    }
    glPopMatrix();
}

void spherical2Cartesian() {

    camX = radius * cos(beta2) * sin(alfa2);
    camY = radius * sin(beta2);
    camZ = radius * cos(beta2) * cos(alfa2);
}


/** do guião 5*/

void renderScene(void) {

    float pos[4] = {-1.0, 1.0, 1.0, 0.0};

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    px = radium*cosf(beta)*sinf(alpha);
    py = radium*sinf(beta);
    pz = radium*cosf(alpha)*cosf(beta);

    gluLookAt(px + dx, py + dy, pz + dz,
              dx, dy, dz,
              0.0f, 1.0f, 0.0f);
    glColor3f(0.3, 0.8, 0.9);


    drawTerrain();

    arvoresPosicao();
    //Torus Cor-de-Rosa
    glColor3f(0.8, 0, 0.3);
    glTranslatef(15,1,-5);
    glutSolidTorus(2,5,30,30);
    desenhaTeapots(10);
    desenhaTeapotsCentro(10);
//mover cirvulos a cada re-desenho
    cir2++;
    cir1--;
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

void processKeys(unsigned char c, int xx, int yy) {
    switch (c) {
        case 'd': dx -=1;
            break;
        case 'a': dx +=1;
            break;
        case 'w': dy += 1;
            break;
        case 's': dy -= 1;
            break;
        case 'q': dz -= 1;
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
    glutInitWindowSize(1200,800);
    glutCreateWindow("CG@DI-UM");


    // Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(Mouse_Func);


    glewInit();
    ilInit();

    //Carregar Imagem e construir a VBO
    carregarImagem();
    terrainVBO(th,tw);
    spherical2Cartesian();


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glPolygonMode(GL_FRONT,GL_FILL);
    // enter GLUT's main cycle
    glutMainLoop();

    return 0;
}

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

#define SPACEBAR 32

const GLint w = 1024, h = 640;
GLint posX = 500, posY = 30;
GLint shipWidth = 50, shipHeight = 30, shipSpeed = 5;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);    // Especificoes de observacao de cena
    gluOrtho2D(0, w, 0, h);
}

void reshape(GLsizei w, GLsizei h) {
    glutReshapeWindow(1024, 640);   // Nao permite a alteracao de tamanho da janela
}

void drawShip() {
    glBegin(GL_QUADS);
        glVertex2i(posX, posY);
        glVertex2i(posX+shipWidth, posY);
        glVertex2i(posX+shipWidth, posY+shipHeight-15);
        glVertex2i(posX, posY+shipHeight-15);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(posX+5, posY+shipHeight-15);
        glVertex2i(posX+shipWidth-5, posY+shipHeight-15);
        glVertex2i(posX+shipWidth-5, posY+shipHeight-10);
        glVertex2i(posX+5, posY+shipHeight-10);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(posX+(shipWidth/2 - 5), posY+shipHeight-10);
        glVertex2i(posX+(shipWidth/2 + 5), posY+shipHeight-10);
        glVertex2i(posX+(shipWidth/2 + 5), posY+shipHeight-5);
        glVertex2i(posX+(shipWidth/2 - 5), posY+shipHeight-5);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(posX+(shipWidth/2 - 1), posY+shipHeight-5);
        glVertex2i(posX+(shipWidth/2 + 1), posY+shipHeight-5);
        glVertex2i(posX+(shipWidth/2 + 1), posY+shipHeight);
        glVertex2i(posX+(shipWidth/2 - 1), posY+shipHeight);
    glEnd();
    
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawShip();
    
    glutPostRedisplay();    // Chama a funcao DISPLAY apos a atualizacao
    glFlush();
}

void onKeyPress(unsigned char key, int x, int y) {
    if (key == SPACEBAR) {
        cout << "shoot" << endl;
    }
}

void onSpecialKeyPress(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        posX -= shipSpeed;
        if (posX < 0) posX = 0;
    }
    if (key == GLUT_KEY_RIGHT) {
        posX += shipSpeed;
        if (posX > w - shipWidth) posX = w - shipWidth;
    }
}


int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1024, 640);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Projeto 2 - Space Invaders");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(onKeyPress);
    glutSpecialFunc(onSpecialKeyPress);
    glutMainLoop();
    
    return 0;
}

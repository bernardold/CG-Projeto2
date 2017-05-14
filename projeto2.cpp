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
#define BULLET_AMOUNT 8
#define BULLET_SPEED 15
#define BULLET_WIDTH 4
#define BULLET_HEIGHT 4

typedef struct {
    GLint active;
    GLint  x;
    GLint  y;
}  Bullet;
GLint shoot = 0;
Bullet bullets[BULLET_AMOUNT];


typedef struct {
    GLint width;
    GLint height;
    GLint speed;
    GLint posX;
    GLint posY;
} Ship;
Ship ship = {50, 30, 5, 500, 30};

typedef struct {
    GLsizei w, h;
} Window;
Window window = {1024, 640};

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);    // Especificoes de observacao de cena
    gluOrtho2D(0, window.w, 0, window.h);
}

void reshape(GLsizei w, GLsizei h) {
    glutReshapeWindow(window.w, window.h);   // Nao permite a alteracao de tamanho da janela
}

void moveBullet () {
    // Spawn new bullet
    if (shoot == 1) {
        for(int i = 0; i < BULLET_AMOUNT; i++) {
            if(bullets[i].active == 0) {
                bullets[i] = {1, ship.posX+(ship.width/2 - BULLET_WIDTH/2), ship.posY+ship.height + BULLET_HEIGHT/2};
                break;
            }
        }
        shoot = 0;
    }
    
    // Move and remove bullets
    for(int i = 0; i < BULLET_AMOUNT; i++) {
        if(bullets[i].active == 1) {
            bullets[i].y = bullets[i].y + BULLET_SPEED;
            
            if (bullets[i].y > window.h) {
                bullets[i].active = 0;
            }
        }
    }
}

void drawShip() {
    glBegin(GL_QUADS);
        glVertex2i(ship.posX, ship.posY);
        glVertex2i(ship.posX+ship.width, ship.posY);
        glVertex2i(ship.posX+ship.width, ship.posY+ship.height-15);
        glVertex2i(ship.posX, ship.posY+ship.height-15);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(ship.posX+5, ship.posY+ship.height-15);
        glVertex2i(ship.posX+ship.width-5, ship.posY+ship.height-15);
        glVertex2i(ship.posX+ship.width-5, ship.posY+ship.height-10);
        glVertex2i(ship.posX+5, ship.posY+ship.height-10);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(ship.posX+(ship.width/2 - 5), ship.posY+ship.height-10);
        glVertex2i(ship.posX+(ship.width/2 + 5), ship.posY+ship.height-10);
        glVertex2i(ship.posX+(ship.width/2 + 5), ship.posY+ship.height-5);
        glVertex2i(ship.posX+(ship.width/2 - 5), ship.posY+ship.height-5);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2i(ship.posX+(ship.width/2 - 1), ship.posY+ship.height-5);
        glVertex2i(ship.posX+(ship.width/2 + 1), ship.posY+ship.height-5);
        glVertex2i(ship.posX+(ship.width/2 + 1), ship.posY+ship.height);
        glVertex2i(ship.posX+(ship.width/2 - 1), ship.posY+ship.height);
    glEnd();
    
}

void drawBullets() {
    for(int i = 0; i < BULLET_AMOUNT; i++) {
        if(bullets[i].active == 1) {
            glBegin(GL_QUADS);
                glVertex2i(bullets[i].x, bullets[i].y);
                glVertex2i(bullets[i].x+BULLET_WIDTH, bullets[i].y);
                glVertex2i(bullets[i].x+BULLET_WIDTH, bullets[i].y+BULLET_HEIGHT);
                glVertex2i(bullets[i].x, bullets[i].y+BULLET_HEIGHT);
            glEnd();
            
        }
    }
}

void tick (GLint value) {
    
    moveBullet();
    //checkMapBoundries();
    
    glutPostRedisplay();
    glutTimerFunc(33, tick, value);      /* 30 frames per second */
    
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawShip();
    drawBullets();
    
    glutPostRedisplay();    // Chama a funcao DISPLAY apos a atualizacao
    glFlush();
}

void onKeyPress(unsigned char key, int x, int y) {
    if (key == SPACEBAR) {
        shoot = 1;
    }
}

void onSpecialKeyPress(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        ship.posX -= ship.speed;
        if (ship.posX < 0) ship.posX = 0;
    }
    if (key == GLUT_KEY_RIGHT) {
        ship.posX += ship.speed;
        if (ship.posX > window.w - ship.width) ship.posX = window.w - ship.width;
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
    glutTimerFunc(33, tick, 0);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(onKeyPress);
    glutSpecialFunc(onSpecialKeyPress);
    glutMainLoop();
    
    return 0;
}

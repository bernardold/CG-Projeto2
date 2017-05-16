/*  
*   Projeto 2 - CG
*   
*   Bernardo Duarte             8598861
*   Giovani Ortolani Barbosa    8936648
*   Giovanni Robira             8531887           
*   Julia Minetto Macias        8937329
*/

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
#define BULLET_AMOUNT 4
#define BULLET_SPEED 15
#define BULLET_COOLDOWN 10
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 6

GLuint tickCount = 0;
GLint gameOver = 0, win = 0, lose = 0;


typedef struct {
    GLint active;
    GLint  x;
    GLint  y;
}  Bullet;
Bullet bullets[BULLET_AMOUNT];
GLint shoot = 0;

typedef struct {
    GLint alive;
    GLint x;
    GLint y;
    GLint width;
    GLint height;
} Alien;
Alien aliens[11][5];
GLint aliensXSpeed = 2;
GLint aliensYSpeed = 0;

typedef struct {
    GLint width;
    GLint height;
    GLint speed;
    GLint posX;
    GLint posY;
    GLint canShoot;
} Ship;
Ship ship = {50, 30, 15, 500, 30, 1};
GLuint lastShotTick = 0;

typedef struct {
    GLsizei w, h;
} Window;
Window window = {1024, 640};

void init() {
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);    // Especificoes de observacao de cena
    gluOrtho2D(0, window.w, 0, window.h);
    
    GLint initX = 80, initY = 280;
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        initX += 80;
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            aliens[i][j] = {1, initX, initY, 50, 30};
            initY += 80;
        }
        initY = 280;
    }
}

void reshape(GLsizei w, GLsizei h) {
    glutReshapeWindow(window.w, window.h);   // Nao permite a alteracao de tamanho da janela
}

void moveBullet () {
    // Spawn new bullet
    if (shoot == 1 && tickCount - lastShotTick >= BULLET_COOLDOWN) {
        for (int i = 0; i < BULLET_AMOUNT; i++) {
            if (bullets[i].active == 0) {
                bullets[i] = {1, ship.posX+(ship.width/2 - BULLET_WIDTH/2), ship.posY+ship.height + BULLET_HEIGHT/2};
                break;
            }
        }
        lastShotTick = tickCount;
    }
    shoot = 0;
    
    // Move and remove bullets
    for (int i = 0; i < BULLET_AMOUNT; i++) {
        if (bullets[i].active == 1) {
            bullets[i].y = bullets[i].y + BULLET_SPEED;
            
            if (bullets[i].y > window.h) {
                bullets[i].active = 0;
            }
        }
    }
}

void moveAliens() {
    GLint switchDirection = 0;
    
    if (tickCount % 100 == 0) aliensYSpeed = -30;
    
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            if (aliens[i][j].alive) {
                aliens[i][j].x += aliensXSpeed;
                if (aliensYSpeed != 0) aliens[i][j].y += aliensYSpeed;
            }
            
            if (aliens[i][j].x <= 0 || aliens[i][j].x + aliens[i][j].width + aliensXSpeed >= window.w) {
                switchDirection = 1;
            }
        }
    }
    
    aliensYSpeed = 0;
    if (switchDirection) aliensXSpeed *= (-1);
    
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
    for (int i = 0; i < BULLET_AMOUNT; i++) {
        if (bullets[i].active) {
            glBegin(GL_QUADS);
                glVertex2i(bullets[i].x, bullets[i].y);
                glVertex2i(bullets[i].x+BULLET_WIDTH, bullets[i].y);
                glVertex2i(bullets[i].x+BULLET_WIDTH, bullets[i].y+BULLET_HEIGHT);
                glVertex2i(bullets[i].x, bullets[i].y+BULLET_HEIGHT);
            glEnd();
            
        }
    }
}


void drawAliens() {
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            if (aliens[i][j].alive) {
                glBegin(GL_QUADS);
                    glVertex2i(aliens[i][j].x, aliens[i][j].y);
                    glVertex2i(aliens[i][j].x+aliens[i][j].width, aliens[i][j].y);
                    glVertex2i(aliens[i][j].x+aliens[i][j].width-5, aliens[i][j].y+aliens[i][j].height-10);
                    glVertex2i(aliens[i][j].x+5, aliens[i][j].y+aliens[i][j].height-10);
                glEnd();
                glBegin(GL_QUADS);
                    glVertex2i(aliens[i][j].x+15, aliens[i][j].y+aliens[i][j].height-10);
                    glVertex2i(aliens[i][j].x+aliens[i][j].width-15, aliens[i][j].y+aliens[i][j].height-10);
                    glVertex2i(aliens[i][j].x+aliens[i][j].width-15, aliens[i][j].y+aliens[i][j].height);
                    glVertex2i(aliens[i][j].x+15, aliens[i][j].y+aliens[i][j].height);
                glEnd();
            }
        }
    }
}

void checkBulletAlienCollision() {
    for (int b = 0; b < BULLET_AMOUNT; b++) {
        if (bullets[b].active) {
            
            for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
                for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
                    if (aliens[i][j].alive) {
                        
                        // if bullet tip is inside alien boundaries
                        if (bullets[b].x > aliens[i][j].x && bullets[b].x < aliens[i][j].x + aliens[i][j].width &&
                            bullets[b].y + BULLET_HEIGHT > aliens[i][j].y && bullets[b].y + BULLET_HEIGHT < aliens[i][j].y + aliens[i][j].height) {
                            aliens[i][j].alive = 0;
                            bullets[b].active = 0;
                        }
                        
                    }
                }
            }
        }
    }
}

GLint checkShipAlienCollision() {
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            if (aliens[i][j].alive) {
                
                if (aliens[i][j].y < ship.posY+ship.height) {
                    // if alien right > ship left || alien left < ship right
                    if ((aliens[i][j].x + aliens[i][j].width > ship.posX && aliens[i][j].x + aliens[i][j].width < ship.posX + ship.width) ||
                        (aliens[i][j].x < ship.posX + ship.width && aliens[i][j].x > ship.posX)) {
                        return 1;
                    }
                }
                
            }
        }
    }
    
    return 0;
}

GLint alienInvasion() {
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            if (aliens[i][j].alive) {
                
                if (aliens[i][j].y < 0) {
                    return 1;
                }
                
            }
        }
    }
    
    return 0;
}

GLint anyAlienAlive() {
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            if (aliens[i][j].alive) {
                return 1;
            }
        }
    }
    return 0;
}

GLint checkEndGame() {
    // Win conditions
    if (!anyAlienAlive()){
        win = 1;
        cout << "You won the game!" << endl;
        return 1;
    }
    
    // Lose conditions
    if (checkShipAlienCollision() || alienInvasion()) {
        lose = 1;
        cout << "You lose the game!" << endl;
        return 1;
    }
    
    return 0;
}

void tick(GLint value) {
    if (!gameOver) {
        tickCount++;
    
        moveBullet();
        moveAliens();
        checkBulletAlienCollision();
    
        gameOver = checkEndGame();
    }
    glutPostRedisplay();
    glutTimerFunc(33, tick, value);      // 30 frames per second
    
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor3f(0.0f, 1.0f, 0.0f);
    drawShip();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawBullets();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawAliens();
    
    glutPostRedisplay();    // Chama a funcao DISPLAY apos a atualizacao
    glFlush();
}

void onKeyPress(unsigned char key, int x, int y) {
    if (!gameOver) {
        if (key == SPACEBAR) {
            shoot = 1;
        }
    }
}

void onSpecialKeyPress(int key, int x, int y) {
    if (!gameOver) {
        if (key == GLUT_KEY_LEFT) {
            ship.posX -= ship.speed;
            if (ship.posX < 0) ship.posX = 0;
        }
        if (key == GLUT_KEY_RIGHT) {
            ship.posX += ship.speed;
            if (ship.posX > window.w - ship.width) ship.posX = window.w - ship.width;
        }
    }
}


int main (int argc, char * argv[]) {
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

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
#include "SOIL.h"
using namespace std;

#define SPACEBAR 32
#define BULLET_AMOUNT 4
#define BULLET_SPEED 15
#define BULLET_COOLDOWN 10
#define ENEMY_BULLET_COOLDOWN 30
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 6

GLuint tickCount = 0;
GLint gameOver = 0, win = 0, lose = 0;

typedef struct {
    GLint active;
    GLint  x;
    GLint  y;
    GLint speed;
}  Bullet;
Bullet bullets[BULLET_AMOUNT];
Bullet enemyBullets[BULLET_AMOUNT];
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
    GLint health;
} Ship;
Ship ship = {50, 30, 15, 500, 30, 3};
GLuint lastShotTick = 0, lastEnemyShot = 0;

typedef struct {
    GLsizei w, h;
} Window;
Window window = {1024, 640};

GLuint texture = 0;


//void loadTexture(void) {
//    texture = SOIL_load_OGL_texture(
//                    "image1.png",
//                    SOIL_LOAD_AUTO,
//                    SOIL_CREATE_NEW_ID,
//                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
//                    );
//
//    if(texture == 0)
//    {
//        printf("SOIL loading error: '%s'\n", SOIL_last_result());
//        exit(EXIT_FAILURE);
//    }
//
//    glBindTexture(GL_TEXTURE_2D, texture); 
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//}

//void background(void) {

//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, texture);  // Define a textura corrente
//
//    glBegin(GL_QUADS);
//        // TexCoord2i: Coord. dos pontos na textura
//        // Vertex2i: Coord. dos pontos no poligono
//        glTexCoord2i(0, 0); glVertex2i(0, 0);
//        glTexCoord2i(1, 0); glVertex2i(window.w, 0);
//        glTexCoord2i(1, 1); glVertex2i(window.w, window.h);
//        glTexCoord2i(0, 1); glVertex2i(0, window.h);
//    glEnd();
//    
//    glDisable(GL_TEXTURE_2D);
//}


void reshape(GLsizei w, GLsizei h) {
    glutReshapeWindow(window.w, window.h);   // Nao permite a alteracao de tamanho da janela
}

void init(void) {
    
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

void drawShip(void) {
    GLint h = 3 - ship.health;
    
    if (ship.health > 0) {
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
            glVertex2i(ship.posX, ship.posY+(h*5));
            glVertex2i(ship.posX+ship.width, ship.posY+(h*5));
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
}


void drawBullets(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < BULLET_AMOUNT; i++) {
        if (bullets[i].active) {
            glBegin(GL_QUADS);
                glVertex2i(bullets[i].x, bullets[i].y);
                glVertex2i(bullets[i].x+BULLET_WIDTH, bullets[i].y);
                glVertex2i(bullets[i].x+BULLET_WIDTH, bullets[i].y+BULLET_HEIGHT);
                glVertex2i(bullets[i].x, bullets[i].y+BULLET_HEIGHT);
            glEnd();
        }
        if (enemyBullets[i].active) {
            glBegin(GL_QUADS);
            glVertex2i(enemyBullets[i].x, enemyBullets[i].y);
            glVertex2i(enemyBullets[i].x+BULLET_WIDTH, enemyBullets[i].y);
            glVertex2i(enemyBullets[i].x+BULLET_WIDTH, enemyBullets[i].y+BULLET_HEIGHT);
            glVertex2i(enemyBullets[i].x, enemyBullets[i].y+BULLET_HEIGHT);
            glEnd();
        }
    }
}

void drawAliens(void) {
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            if (aliens[i][j].alive) {
                if (j == 0) {
                    glColor3f(0.0f, 1.0f, 1.0f);
                    glBegin(GL_TRIANGLES);
                        glVertex2i(aliens[i][j].x+10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+(aliens[i][j].width/2), aliens[i][j].y+15);
                    glEnd();
                    glBegin(GL_TRIANGLES);
                        glVertex2i(aliens[i][j].x+5, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-5, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+(aliens[i][j].width/2), aliens[i][j].y+aliens[i][j].height);
                    glEnd();
                    glColor3f(0.0f, 0.0f, 0.0f);
                    glBegin(GL_TRIANGLES);
                        glVertex2i(aliens[i][j].x+17, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-17, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+(aliens[i][j].width/2), aliens[i][j].y+7);
                    glEnd();
                    glPointSize(4.0);
                    glBegin(GL_POINTS);
                        glVertex2i(aliens[i][j].x+20, aliens[i][j].y+aliens[i][j].height-15);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-20, aliens[i][j].y+aliens[i][j].height-15);
                    glEnd();
                }
                else if (j == 1) {
                    glColor3f(1.0f, 0.0f, 1.0f);
                    glBegin(GL_QUADS);
                        glVertex2i(aliens[i][j].x, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-5, aliens[i][j].y+aliens[i][j].height-10);
                        glVertex2i(aliens[i][j].x+5, aliens[i][j].y+aliens[i][j].height-10);
                    glEnd();
                    glBegin(GL_QUADS);
                        glVertex2i(aliens[i][j].x+15, aliens[i][j].y+aliens[i][j].height-10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-15, aliens[i][j].y+aliens[i][j].height-10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-15, aliens[i][j].y+aliens[i][j].height);
                        glVertex2i(aliens[i][j].x+15, aliens[i][j].y+aliens[i][j].height);
                    glEnd();
                    glLineWidth(2);
                    glBegin(GL_LINES);
                        glVertex2i(aliens[i][j].x+10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+10, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-10, aliens[i][j].y+10);
                    glEnd();
                    glColor3f(0.7f, 1.0f, 0.4f);
                    glPointSize(4.0);
                    glBegin(GL_POINTS);
                        glVertex2i(aliens[i][j].x+20, aliens[i][j].y+aliens[i][j].height-15);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-20, aliens[i][j].y+aliens[i][j].height-15);
                    glEnd();
                }
                else if (j == 2) {
                    glColor3f(1.0f, 1.0f, 0.0f);
                    glBegin(GL_TRIANGLE_FAN);
                        glVertex2i(aliens[i][j].x+(aliens[i][j].width/2), aliens[i][j].y+(aliens[i][j].height/2));
                        glVertex2i(aliens[i][j].x+aliens[i][j].width, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-5, aliens[i][j].y+aliens[i][j].height-10);
                        glVertex2i(aliens[i][j].x+5, aliens[i][j].y+aliens[i][j].height-10);
                    glEnd();
                    glLineWidth(2);
                    glBegin(GL_LINES);
                        glVertex2i(aliens[i][j].x+10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+15, aliens[i][j].y+20);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-12, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-10, aliens[i][j].y+10);
                    glEnd();
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glPointSize(5.0);
                    glBegin(GL_POINTS);
                        glVertex2i(aliens[i][j].x+40, aliens[i][j].y+aliens[i][j].height-15);
                    glEnd();
                }
                else if (j == 3) {
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glBegin(GL_TRIANGLES);
                        glVertex2i(aliens[i][j].x+10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+(aliens[i][j].width/2), aliens[i][j].y+15);
                    glEnd();
                    glBegin(GL_TRIANGLES);
                        glVertex2i(aliens[i][j].x+5, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-5, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+(aliens[i][j].width/2), aliens[i][j].y+aliens[i][j].height);
                    glEnd();
                    glColor3f(0.0f, 0.0f, 0.0f);
                    glBegin(GL_TRIANGLES);
                        glVertex2i(aliens[i][j].x+17, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-17, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+(aliens[i][j].width/2), aliens[i][j].y+7);
                    glEnd();
                    glPointSize(4.0);
                    glBegin(GL_POINTS);
                        glVertex2i(aliens[i][j].x+20, aliens[i][j].y+aliens[i][j].height-15);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-20, aliens[i][j].y+aliens[i][j].height-15);
                    glEnd();
                }
                else if (j == 4) {
                    glColor3f(0.0f, 0.0f, 1.0f);
                    glBegin(GL_QUADS);
                        glVertex2i(aliens[i][j].x, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-5, aliens[i][j].y+aliens[i][j].height-10);
                        glVertex2i(aliens[i][j].x+5, aliens[i][j].y+aliens[i][j].height-10);
                    glEnd();
                    glBegin(GL_QUADS);
                        glVertex2i(aliens[i][j].x+15, aliens[i][j].y+aliens[i][j].height-10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-15, aliens[i][j].y+aliens[i][j].height-10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-15, aliens[i][j].y+aliens[i][j].height);
                        glVertex2i(aliens[i][j].x+15, aliens[i][j].y+aliens[i][j].height);
                    glEnd();
                    glLineWidth(2);
                    glBegin(GL_LINES);
                        glVertex2i(aliens[i][j].x+10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+10, aliens[i][j].y+10);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-10, aliens[i][j].y);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-10, aliens[i][j].y+10);
                    glEnd();
                    glColor3f(0.7f, 1.0f, 0.4f);
                    glPointSize(4.0);
                    glBegin(GL_POINTS);
                        glVertex2i(aliens[i][j].x+20, aliens[i][j].y+aliens[i][j].height-15);
                        glVertex2i(aliens[i][j].x+aliens[i][j].width-20, aliens[i][j].y+aliens[i][j].height-15);
                    glEnd();
                }
            }
        }      
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    //background();                   // Carrega a imagem de fundo
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawShip();

    drawBullets();

    drawAliens();

    glColor3f(1.0f, 1.0f, 1.0f);
    
    glutPostRedisplay();            // Chama a funcao DISPLAY apos a atualizacao
    glFlush();
}

void movePlayerBullet (void) {
    // Spawn new bullet
    if (shoot == 1 && tickCount - lastShotTick >= BULLET_COOLDOWN) {
        for (int i = 0; i < BULLET_AMOUNT; i++) {
            if (bullets[i].active == 0) {
                bullets[i] = {1, ship.posX+(ship.width/2 - BULLET_WIDTH/2), ship.posY+ship.height , BULLET_SPEED};
                break;
            }
        }
        lastShotTick = tickCount;
    }
    shoot = 0;
    
    // Move and remove bullets
    for (int i = 0; i < BULLET_AMOUNT; i++) {
        if (bullets[i].active) {
            bullets[i].y = bullets[i].y + bullets[i].speed;
            
            if (bullets[i].y > window.h) {
                bullets[i].active = 0;
            }
        }
    }
}

void moveAlienBullet (void) {
    Alien a;
    int j = 0;
    
    // Spawn new bullet
    if (tickCount - lastEnemyShot >= ENEMY_BULLET_COOLDOWN) {
        // ellect an alien to shoot
        for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
            while (!aliens[i][j].alive) {
                if (aliens[i][j].x - ship.posX >= -50 && aliens[i][j].x - ship.posX <= 50) {
                    a = aliens[i][j];
                    cout << i << " x " << j << endl;
                }
                j++;
            }
            if (aliens[i][j].x - ship.posX >= -50 && aliens[i][j].x - ship.posX <= 50) {
                a = aliens[i][j];
            }
            j = 0;
        }
        
        // shoot
        for (int i = 0; i < BULLET_AMOUNT; i++) {
            if (!enemyBullets[i].active) {
                enemyBullets[i] = {1, a.x+(a.width/2 - BULLET_WIDTH/2), a.y, -BULLET_SPEED};
                break;
            }
        }
        lastEnemyShot = tickCount;
    }
    
    // Move and remove bullets
    for (int i = 0; i < BULLET_AMOUNT; i++) {
        if (enemyBullets[i].active) {
            enemyBullets[i].y = enemyBullets[i].y + enemyBullets[i].speed;
            
            if (enemyBullets[i].y < 0) {
                enemyBullets[i].active = 0;
            }
        }
    }
}


void moveAliens(void) {
    GLint switchDirection = 0;
    
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            if (aliens[i][j].alive) {
                aliens[i][j].x += aliensXSpeed;
                aliens[i][j].y += aliensYSpeed;
            }
            
            if (aliens[i][j].x <= 0 || aliens[i][j].x + aliens[i][j].width + aliensXSpeed >= window.w) {
                switchDirection = 1;
            }
        }
    }
    
    aliensYSpeed = 0;
    if (switchDirection){
        if (aliensXSpeed < 0) aliensYSpeed = -28;
        aliensXSpeed *= (-1);
    }
    
}

void checkBulletAlienCollision(void) {
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

GLint checkShipAlienCollision(void) {
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

void checkBulletShipCollision(void) {
    for (int b = 0; b < BULLET_AMOUNT; b++) {
        if (enemyBullets[b].active) {

            // if bullet is inside ship boundaries
            if (enemyBullets[b].x > ship.posX && enemyBullets[b].x < ship.posX + ship.width &&
                enemyBullets[b].y > ship.posY && enemyBullets[b].y < ship.posY + ship.height) {
                ship.health--;
                enemyBullets[b].active = 0;
            }

        }
    }
}

GLint alienInvasion(void) {
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

GLint anyAlienAlive(void) {
    for (int i = 0; i < (sizeof(aliens)/sizeof(aliens[0])); i++) {
        for (int j = 0; j < (sizeof(aliens[i])/sizeof(aliens[i][0])); j++) {
            if (aliens[i][j].alive) {
                return 1;
            }
        }
    }
    return 0;
}

GLint checkEndGame(void) {
    // Win conditions
    if (!anyAlienAlive()){
        win = 1;
        cout << "You won the game!" << endl;
        return 1;
    }
    
    // Lose conditions
    if (checkShipAlienCollision() || alienInvasion() || ship.health == 0) {
        lose = 1;
        cout << "You lose the game!" << endl;
        return 1;
    }
    
    return 0;
}

void tick(GLint value) {
    if (!gameOver) {
        tickCount++;    
    
        movePlayerBullet();
        moveAliens();
        moveAlienBullet();
        checkBulletAlienCollision();
        checkBulletShipCollision();
    
        gameOver = checkEndGame();
    }
    glutPostRedisplay();
    glutTimerFunc(33, tick, value);      // 30 frames per second
    
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

int main (int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1024, 640);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Projeto 2 - Space Invaders");
    
    init();
    //loadTexture();

    glutDisplayFunc(display);
    glutTimerFunc(33, tick, 0);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(onKeyPress);
    glutSpecialFunc(onSpecialKeyPress);
    glutMainLoop();

    return 0;
}

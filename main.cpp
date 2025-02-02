#include <windows.h>
#include <gl/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sstream>

int pos[] = {10,15,20,25,30,35,40,45,50,55};
int peg[3] = {50,150,250};
int moves[1000][3];
int max_moves;
int POLES[3][10];
int top[3] = {-1,-1,-1};
int NUM_DISKS = 3;
int cnt, counter;
int line1 = 90, line2 = 85;
float ycoordinate;
int lightflag = 1, animationFlag = 1;

void push(int p, int disk) {
    POLES[p][++top[p]] = disk;
}

void pop(int p) {
    top[p]--;
}

void tower(int n, int src, int temp, int dst) {
    if(n > 0) {
        tower(n-1, src, dst, temp);
        moves[cnt][0] = n;
        moves[cnt][1] = src;
        moves[cnt][2] = dst;
        cnt++;
        tower(n-1, temp, src, dst);
    }
}

void drawcylinder() {
    GLUquadricObj *q = gluNewQuadric();
    gluCylinder(q, 2, 2, 60, 60, 20);
}

void drawBase() {
    GLUquadricObj *quadric = gluNewQuadric();
    float baseRadius = 45, baseThickness = 4;
    gluCylinder(quadric, baseRadius, baseRadius, baseThickness, 50, 1);
    glPushMatrix();
    gluDisk(quadric, 0, baseRadius, 50, 1);
    glPopMatrix();
    glPushMatrix();
    gluDisk(quadric, 0, baseRadius, 50, 1);
    glPopMatrix();
    gluDeleteQuadric(quadric);
}

void drawPegs() {
    glColor3f(1, 1, 0);
    for(int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(peg[i], 5, 0);
        glRotatef(-90, 1, 0, 0);
        drawcylinder();
        glPushMatrix();
        drawBase();
        glPopMatrix();
        glPopMatrix();
    }
}

void printString(char *text) {
    for(int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void drawText() {
    glColor3f(0, 0, 0);
    glRasterPos3f(-70, line1, 0);
    printString("Move :");

    char str[5];
    sprintf(str, "%d", counter);
    glRasterPos3f(-40, line1, 0);
    printString(str);

    glRasterPos3f(-70, line2, 0);
    printString("Disk");
    char str1[10];
    sprintf(str1, "%d", moves[counter][0]);
    glRasterPos3f(-50, line2, 0);
    printString(str1);

    glRasterPos3f(-40, line2, 0);
    printString("from");

    char src[2];
    strcpy(src, moves[counter][1] == 0 ? "A" : (moves[counter][1] == 1 ? "B" : "C"));
    glRasterPos3f(-20, line2, 0);
    printString(src);

    glRasterPos3f(-10, line2, 0);
    printString("to");

    char dst[2];
    strcpy(dst, moves[counter][2] == 0 ? "A" : (moves[counter][2] == 1 ? "B" : "C"));
    glRasterPos3f(0, line2, 0);
    printString(dst);

    glColor3f(0, 0, 1);
    glRasterPos3f(peg[0], 70, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
    glRasterPos3f(peg[1], 70, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'B');
    glRasterPos3f(peg[2], 70, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');
}

void drawSolved() {
    glColor3f(0, 0, 1);
    glRasterPos3f(peg[0], 70, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
    glRasterPos3f(peg[1], 70, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'B');
    glRasterPos3f(peg[2], 70, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'C');

    glColor3f(0, 0, 1);
    glRasterPos3f(-60, 80, 0);
    if(moves[0][0] == 0) {
        printString("Select the number of disks");
    } else {
        printString("Solved !!");
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    gluLookAt(0, ycoordinate, 0, 0, 0, -1, 0, 1, 0);
    drawPegs();

    for(int i = 0; i < 3; i++) {
        int k = 0;
        for(int j = 0; j <= top[i]; j++) {
            glPushMatrix();
            glTranslatef(peg[i], pos[k++], 0);
            glRotatef(90, 1, 0, 0);
            glColor3f(0, 1, 0);
            glutSolidTorus(2, 6 * POLES[i][j], 20, 20);
            glPopMatrix();
        }
    }
    glPopMatrix();
    glDisable(GL_LIGHTING);

    if(counter == max_moves)
        drawSolved();
    else
        drawText();

    if(lightflag)
        glEnable(GL_LIGHTING);
    glutSwapBuffers();
}

void lighting() {
    GLfloat shininess[] = {50};
    GLfloat white[] = {0.6, 0.6, 0.6, 1};
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat[]){100, 60, 10, 0.0});
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glEnable(GL_LIGHT0);
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-80, 350, -10, 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    lighting();
}

void mouse(int btn, int mode, int x, int y) {
    if(btn == 4 && mode == GLUT_DOWN) {
        if(counter < max_moves) {
            pop(moves[counter][1]);
            push(moves[counter][2], moves[counter][0]);
            counter++;
        }
    }

    if(btn == 3 && mode == GLUT_DOWN) {
        if(counter > 0) {
            counter--;
            pop(moves[counter][2]);
            push(moves[counter][1], moves[counter][0]);
        }
    }
    glutPostRedisplay();
}

void restart() {
    memset(POLES, 0, sizeof(POLES));
    memset(moves, 0, sizeof(POLES));
    memset(top, -1, sizeof(top));
    cnt = 0, counter = 0;
    ycoordinate = 0.1;
    max_moves = pow(2, NUM_DISKS) - 1;

    for(int i = NUM_DISKS; i > 0; i--) {
        push(0, i);
    }

    tower(NUM_DISKS, 0, 1, 2);
}

void processMenuRestart(int option) {
    if(option == 0) {
        restart();
        glutPostRedisplay();
    }
}

void processMenuExit(int option) {
    if(option == 0) exit(0);
}

void processMenuSolveCompletely(int option) {
    int temp = animationFlag;
    animationFlag = 0;

    while(counter < max_moves) {
        mouse(4, GLUT_DOWN, 0, 0);
        display();
        for(int i = 0; i < 1000000; i++)
            for(int j = 0; j < 100; j++);
        usleep(1000000);
    }

    animationFlag = temp;
}

void processMenuNumDisks(int option) {
    NUM_DISKS = option;
    restart();
    glutPostRedisplay();
}

void createGLUTMenus() {
    int menu = glutCreateMenu(processMenuNumDisks);
    for(int i = 3; i <= 8; i++) {
        std::ostringstream oss;
        oss << i;
        glutAddMenuEntry(oss.str().c_str(), i);
    }


    int menuExit = glutCreateMenu(processMenuExit);
    glutAddMenuEntry("Yes", 0);
    glutAddMenuEntry("No", 1);

    int menuRestart = glutCreateMenu(processMenuRestart);
    glutAddMenuEntry("Yes", 0);
    glutAddMenuEntry("No", 1);

    int menuSolveCompletely = glutCreateMenu(processMenuSolveCompletely);
    glutAddMenuEntry("Start", 0);

    glutCreateMenu(NULL);
    glutAddSubMenu("Number of Disks", menu);
    glutAddSubMenu("Solve Completely", menuSolveCompletely);
    glutAddSubMenu("Restart", menuRestart);
    glutAddSubMenu("Exit", menuExit);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tower of Hanoi");
    init();
    glutDisplayFunc(display);
    createGLUTMenus();
    glutMainLoop();
    return 0;
}

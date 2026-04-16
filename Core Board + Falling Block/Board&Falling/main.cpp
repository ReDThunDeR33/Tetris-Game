#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <sstream>
#include <windows.h>
#include <GL/glut.h>

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


}

// global varable use na korle alada function e problem hobe
const int Column = 10;
    const int Row = 20;

    float L = -1.0f;
    float R =  0.2f;
    float D = -1.0f;
    float U =  1.0f;

    int blockCol = 5;
    int blockRow = 19;

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

//// right side area of the game


	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.2f, 1.0f);
	glVertex2f(0.2f, -1.0f);

	glEnd();

// left side area of the game
	glBegin(GL_POLYGON);
	glColor3f(0.05, 0.05, 0.08);
	glVertex2f(-1.0f,1.0f);
	glVertex2f(-1.0f,-1.0f);
	glVertex2f(0.2f,-1.0f);
	glVertex2f(0.2f,1.0f);
	glEnd();

// grid boxes

    float BoxWidth = (R - L) / Column;
    float BoxHeight = (U - D) / Row;

    glColor3f(0.75f, 0.75f, 0.75f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);

    for (int c = 0; c <= Column; c++) {
        float x = L + c * BoxWidth;
        glVertex2f(x, D);
        glVertex2f(x, U);
    }

    for (int r = 0; r <= Row; r++) {
        float y = D + r * BoxHeight;
        glVertex2f(L,  y);
        glVertex2f(R, y);
    }

    glEnd();

// box (for plactice not for use . after arindom's work)



    float x1 = L + blockCol * BoxWidth;
    float y1 = D + blockRow * BoxHeight;
    float x2 = x1 + BoxWidth;
    float y2 = y1 + BoxHeight;

    glColor3f(1.0f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();


        glFlush();

    }


    //falling by timer

    void update(int value) {
        blockRow=blockRow-1;

        if (blockRow < 0)
            {

            blockRow = 19;
        }
        glutPostRedisplay();
        glutTimerFunc(400, update, 0);
    }

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(450, 600);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Board&Falling");
glutTimerFunc(400, update, 0); // glutmainloop() er age dite hobe naile kaaj kore na
    glutDisplayFunc(display);
    initGL();
    glutMainLoop();
    return 0;
}

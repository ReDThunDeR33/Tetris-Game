#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <sstream>
#include <windows.h>
#include <GL/glut.h>

int pieceX = 3;
int pieceY = 16;

int T_shape[4][4] = {
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
void update(int value) {
    pieceY--;

    if (pieceY < 0) {
        pieceY = 19;
    }

    glutPostRedisplay();

    glutTimerFunc(500, update, 0);
}
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            if (pieceX > 0) pieceX--;
            break;
        case 'd':
            if (pieceX < 6) pieceX++;
            break;
    }
    glutPostRedisplay();
}

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

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

    const int Column = 10;
    const int Row = 20;

    float L = -1.0f;
    float R =  0.2f;
    float D = -1.0f;
    float U =  1.0f;

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
glColor3f(0.6f, 0.0f, 0.8f);
    for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {

        if (T_shape[row][col] == 1) {

            int actualCol = pieceX + col;
            int actualRow = pieceY - row;

            float x1 = L + actualCol * BoxWidth;
            float y1 = D + actualRow * BoxHeight;
            float x2 = x1 + BoxWidth;
            float y2 = y1 + BoxHeight;

            glBegin(GL_QUADS);
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
            glEnd();
        }
    }
}
//falling and timer

    glFlush();

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(450, 600);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Board&Falling");

    glutTimerFunc(500, update, 0);
    glutKeyboardFunc(handleKeypress);

    glutDisplayFunc(display);
    initGL();
    glutMainLoop();
    return 0;
}

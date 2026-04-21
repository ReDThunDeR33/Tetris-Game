#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <sstream>
#include <windows.h>
#include <GL/glut.h>

int pieceX = 4;
int pieceY = 20;

// 0: I, 1: O, 2: T, 3: S, 4: Z, 5: J, 6: L
int shapes[7][4][4] = {
    {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}, // I
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // Z
    {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}}  // L
};

// Standard colors but less saturated (R, G, B)
float colors[7][3] = {
    {0.4f, 0.7f, 0.7f}, // Cyan (I)
    {0.8f, 0.8f, 0.4f}, // Yellow (O)
    {0.6f, 0.4f, 0.7f}, // Purple (T)
    {0.4f, 0.7f, 0.4f}, // Green (S)
    {0.8f, 0.4f, 0.4f}, // Red (Z)
    {0.4f, 0.4f, 0.7f}, // Blue (J)
    {0.8f, 0.6f, 0.4f}  // Orange (L)
};

int currentShapeIndex = (rand() % 4); // Start with


void update(int value) {
    pieceY--;
    if (pieceY < 0) {
        pieceY = 20;
        pieceX = 4;
        currentShapeIndex = rand() % 4;
    }

    glutPostRedisplay();
    glutTimerFunc(500, update, 0);

}
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            if (pieceX > 0) pieceX--;
            else if (pieceX <= 0) pieceX = 9;

            break;
        case 'd':
            if (pieceX < 8) pieceX++;
            else if(pieceX >= 8) pieceX = 0;
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
glPushMatrix();
// box (for plactice not for use . after arindom's work)
//glColor3f(0.6f, 0.0f, 0.8f);
    for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {

        if (shapes[currentShapeIndex][row][col] == 1) {

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

            glColor3f(0.1f, 0.1f, 0.1f); // border, dont edit
            glBegin(GL_LINE_LOOP);
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
            glEnd();
            // Reset color for the next block part
            glColor3f(colors[currentShapeIndex][0], colors[currentShapeIndex][1], colors[currentShapeIndex][2]);
        }
    }
}
//falling and timer
glPopMatrix();

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

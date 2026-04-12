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

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.2f, 1.0f);
	glVertex2f(0.2f, -1.0f);

	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.05, 0.05, 0.08);
	glVertex2f(-1.0f,1.0f);
	glVertex2f(-1.0f,-1.0f);
	glVertex2f(0.2f,-1.0f);
	glVertex2f(0.2f,1.0f);
	glEnd();






	glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(450, 600);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Board&Falling");

    glutDisplayFunc(display);
    initGL();
    glutMainLoop();
    return 0;
}

#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

const int GRID_W = 10, GRID_H = 20, CELL = 28;
const int WIN_W = GRID_W * CELL, WIN_H = GRID_H * CELL;

int blockX = GRID_W / 2, blockY = 0;
float colorR = 0.2f, colorG = 0.8f, colorB = 1.0f;

void drawCell(int x, int y, float r, float g, float b) {
    int px = x * CELL, py = y * CELL;
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2i(px+1, py+1); glVertex2i(px+CELL-1, py+1);
    glVertex2i(px+CELL-1, py+CELL-1); glVertex2i(px+1, py+CELL-1);
    glEnd();
}

void drawGrid() {
    glColor3f(0.2f, 0.2f, 0.2f);
    for (int r=0; r<=GRID_H; r++) {
        glBegin(GL_LINES);
        glVertex2i(0, r*CELL); glVertex2i(WIN_W, r*CELL);
        glEnd();
    }
    for (int c=0; c<=GRID_W; c++) {
        glBegin(GL_LINES);
        glVertex2i(c*CELL, 0); glVertex2i(c*CELL, WIN_H);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawCell(blockX, blockY, colorR, colorG, colorB);
    glutSwapBuffers();
}

void update(int) {
    blockY++;
    if (blockY >= GRID_H - 1) {
        blockY = 0;
        blockX = rand() % GRID_W;
        colorR = (rand()%100)/100.0f;
        colorG = (rand()%100)/100.0f;
        colorB = (rand()%100)/100.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(250, update, 0);
}

void special(int key, int, int) {
    if (key == GLUT_KEY_LEFT && blockX > 0) blockX--;
    if (key == GLUT_KEY_RIGHT && blockX < GRID_W-1) blockX++;
    if (key == GLUT_KEY_DOWN && blockY < GRID_H-1) blockY++;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, WIN_H, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    srand((unsigned)time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("Part 1 - Core");
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutReshapeFunc(reshape);
    glutTimerFunc(250, update, 0);
    glutMainLoop();
    return 0;
}

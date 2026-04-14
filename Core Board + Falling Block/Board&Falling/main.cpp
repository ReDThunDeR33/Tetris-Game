#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <sstream>
#include <windows.h>

// ================= GAME STATE =================
int score = 0;
int highScore = 0;
bool gameOver = false;

// ================= TEXT DRAW =================
void drawText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

// ================= SCORE SYSTEM =================
void addScore(std::string type) {
    if (gameOver) return;

    if (type == "easy") score += 1;
    else if (type == "medium") score += 2;
    else if (type == "fast") score += 3;

    if (score > highScore) {
        highScore = score;
    }
}

// ================= LEVEL =================
int getLevel() {
    return 1 + score / 20;
}

// ================= DISPLAY =================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Divider line
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.2f, 1.0f);
    glVertex2f(0.2f, -1.0f);
    glEnd();

    // Left game area
    glBegin(GL_POLYGON);
    glColor3f(0.05, 0.05, 0.08);
    glVertex2f(-1.0f,1.0f);
    glVertex2f(-1.0f,-1.0f);
    glVertex2f(0.2f,-1.0f);
    glVertex2f(0.2f,1.0f);
    glEnd();

    // ================= UI TEXT =================
    glColor3f(1,1,1);

    drawText(0.25f, 0.8f, "SCORE: " + std::to_string(score));
    drawText(0.25f, 0.7f, "HIGH SCORE: " + std::to_string(highScore));
    drawText(0.25f, 0.6f, "LEVEL: " + std::to_string(getLevel()));

    drawText(0.25f, 0.4f, "CONTROLS:");
    drawText(0.25f, 0.3f, "E = Easy Drop");
    drawText(0.25f, 0.2f, "M = Medium Drop");
    drawText(0.25f, 0.1f, "F = Fast Drop");

    if (gameOver) {
        drawText(-0.2f, 0.0f, "GAME OVER");
        drawText(-0.3f, -0.1f, "Press R to Restart");
    }

    glFlush();
}

// ================= KEYBOARD =================
void keyboard(unsigned char key, int x, int y) {

    if (key == 'e') addScore("easy");
    if (key == 'm') addScore("medium");
    if (key == 'f') addScore("fast");

    if (key == 'g') gameOver = true;  // simulate game over

    if (key == 'r' || key == 'R') {
        score = 0;
        gameOver = false;
    }

    glutPostRedisplay();
}

// ================= INIT =================
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// ================= MAIN =================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(450, 600);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Board & Scoring UI");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    initGL();
    glutMainLoop();
    return 0;
}

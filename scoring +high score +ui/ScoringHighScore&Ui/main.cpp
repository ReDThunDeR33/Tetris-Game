#include <GL/glut.h>
#include <GL/glut.h>
#include <cstdio>
#include <stdio.h>

int score = 0;
int highScore = 0;
int gameOver = 0;

// ================= DRAW TEXT =================
void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
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
    glBegin(GL_LINES);
    glVertex2f(0.2f, 1);
    glVertex2f(0.2f, -1);
    glEnd();

    // UI text
    char text[50];

    sprintf(text, "Score: %d", score);
    drawText(0.3f, 0.8f, text);

    sprintf(text, "High Score: %d", highScore);
    drawText(0.3f, 0.7f, text);

    sprintf(text, "Level: %d", getLevel());
    drawText(0.3f, 0.6f, text);

    drawText(0.3f, 0.4f, "Controls:");
    drawText(0.3f, 0.3f, "E = Easy");
    drawText(0.3f, 0.2f, "M = Medium");
    drawText(0.3f, 0.1f, "F = Fast");

    if (gameOver == 1) {
        drawText(-0.2f, 0.0f, "GAME OVER");
        drawText(-0.3f, -0.1f, "Press R to Restart");
    }

    glFlush();
}

// ================= KEYBOARD =================
void keyboard(unsigned char key, int x, int y) {

    if (gameOver == 0) {
        if (key == 'e') score += 1;
        if (key == 'm') score += 2;
        if (key == 'f') score += 3;

        if (score > highScore) {
            highScore = score;
        }
    }

    // Simulate game over
    if (key == 'g') {
        gameOver = 1;
    }

    // Restart
    if (key == 'r' || key == 'R') {
        score = 0;
        gameOver = 0;
    }

    glutPostRedisplay();
}

// ================= INIT =================
void init() {
    glClearColor(0, 0, 0, 1);
}

// ================= MAIN =================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 600);
    glutCreateWindow("Simple Score UI");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    init();
    glutMainLoop();
    return 0;
}

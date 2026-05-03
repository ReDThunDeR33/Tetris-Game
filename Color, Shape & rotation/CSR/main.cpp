#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

// Board size
const int ROWS = 20;
const int COLS = 10;

// Board (0 = empty)
int board[ROWS][COLS] = {0};

// Piece position
int pieceX = 4;
int pieceY = 19;

// Shapes (7 tetrominoes)
int shapes[7][4][4] = {
    {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}, // I
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // Z
    {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}, // J
    {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}}  // L
};

// Colors
float colors[7][3] = {
    {0.4f, 0.7f, 0.7f},
    {0.8f, 0.8f, 0.4f},
    {0.6f, 0.4f, 0.7f},
    {0.4f, 0.7f, 0.4f},
    {0.8f, 0.4f, 0.4f},
    {0.4f, 0.4f, 0.7f},
    {0.8f, 0.6f, 0.4f}
};

int currentShapeIndex;

// ============================
// Collision Check
// ============================
bool checkCollision(int newX, int newY) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {

            if (shapes[currentShapeIndex][r][c] == 1) {

                int x = newX + c;
                int y = newY - r;

                if (x < 0 || x >= COLS) return true;
                if (y < 0) return true;

                if (y < ROWS && board[y][x] != 0)
                    return true;
            }
        }
    }
    return false;
}

// ============================
// Lock piece to board
// ============================
void placePiece() {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {

            if (shapes[currentShapeIndex][r][c] == 1) {

                int x = pieceX + c;
                int y = pieceY - r;

                if (y >= 0 && y < ROWS)
                    board[y][x] = currentShapeIndex + 1;
            }
        }
    }
}

// ============================
// Spawn new piece
// ============================
void newPiece() {
    pieceX = 4;
    pieceY = 19;
    currentShapeIndex = rand() % 7;
}

// ============================
// Game update
// ============================
void update(int value) {

    if (!checkCollision(pieceX, pieceY - 1)) {
        pieceY--;
    } else {
        placePiece();
        newPiece();
    }

    glutPostRedisplay();
    glutTimerFunc(400, update, 0);
}

// ============================
// Controls
// ============================
void handleKeypress(unsigned char key, int x, int y) {

    if (key == 'a') {
        if (!checkCollision(pieceX - 1, pieceY))
            pieceX--;
    }

    if (key == 'd') {
        if (!checkCollision(pieceX + 1, pieceY))
            pieceX++;
    }

    glutPostRedisplay();
}

// ============================
// Draw everything
// ============================
void display() {
    // Clear first
    glClear(GL_COLOR_BUFFER_BIT);

    float L = -1.0f;
    float R =  0.2f;
    float D = -1.0f;
    float U =  1.0f;

    float boxW = (R - L) / (float)COLS;
    float boxH = (U - D) / (float)ROWS;

    // =========================
    // Draw placed blocks
    // =========================
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] != 0) {
                int shape = board[r][c] - 1;

                float x1 = L + c * boxW;
                float y1 = D + r * boxH;
                float x2 = x1 + boxW;
                float y2 = y1 + boxH;

                glColor3f(colors[shape][0], colors[shape][1], colors[shape][2]);
                glBegin(GL_QUADS);
                    glVertex2f(x1, y1);
                    glVertex2f(x2, y1);
                    glVertex2f(x2, y2);
                    glVertex2f(x1, y2);
                glEnd();
            }
        }
    }

    // =========================
    // Draw falling piece
    // =========================
    glColor3f(colors[currentShapeIndex][0],
              colors[currentShapeIndex][1],
              colors[currentShapeIndex][2]);

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (shapes[currentShapeIndex][r][c] == 1) {
                int x = pieceX + c;
                int y = pieceY - r; // keep if your coordinate system expects this

                float x1 = L + x * boxW;
                float y1 = D + y * boxH;
                float x2 = x1 + boxW;
                float y2 = y1 + boxH;

                glBegin(GL_QUADS);
                    glVertex2f(x1, y1);
                    glVertex2f(x2, y1);
                    glVertex2f(x2, y2);
                    glVertex2f(x1, y2);
                glEnd();
            }
        }
    }

    // =========================
    // Draw grid (vertical and horizontal lines)
    // =========================
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        // vertical lines
        for (int c = 0; c <= COLS; c++) {
            float x = L + c * boxW;
            glVertex2f(x, D);
            glVertex2f(x, U);
        }
        // horizontal lines
        for (int r = 0; r <= ROWS; r++) {
            float y = D + r * boxH;
            glVertex2f(L, y);
            glVertex2f(R, y);
        }
    glEnd();

    // Flush / swap buffers
    // If using double buffering (GLUT/GLFW), call glutSwapBuffers() or glfwSwapBuffers(window)
    // If single buffered, glFlush() is sufficient.
    #ifdef USE_DOUBLE_BUFFER
        glutSwapBuffers();
    #else
        glFlush();
    #endif
}


// ============================
// Init
// ============================
void init() {
    glClearColor(0,0,0,1);
    srand(time(0));
    newPiece();
}

// ============================
// Main
// ============================
int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(450, 600);
    glutCreateWindow("Simple Tetris");

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(400, update, 0);

    init();
    glutMainLoop();

    return 0;
}

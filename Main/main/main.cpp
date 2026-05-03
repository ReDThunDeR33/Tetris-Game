#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>

// ================= CONSTANTS & GLOBALS =================
const int ROWS = 20;
const int COLS = 10;

// Screen layout boundaries
const float L = -1.0f; // Left border of board
const float R =  0.2f; // Right border of board (start of UI)
const float D = -1.0f; // Bottom border
const float U =  1.0f; // Top border

// Board (0 = empty, >0 = colored block)
int board[ROWS][COLS] = {0};

// Active Piece position and state
int pieceX = 3;
int pieceY = 19;
int currentShapeIndex;
int currentShape[4][4]; // Holds the currently rotating matrix
int dropType = 0; // 0: normal(easy), 1: medium(soft drop), 2: fast(hard drop)

// Game State
int score = 0;
int highScore = 0;
int level = 1;
int gameOver = 0;

// Shapes (7 tetrominoes inside 4x4 grids)
const int originalShapes[7][4][4] = {
    {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}, // I
    {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}, // Z
    {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}, // J
    {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}}  // L
};

// Colors for the 7 shapes
const float colors[7][3] = {
    {0.0f, 1.0f, 1.0f}, // I - Cyan
    {1.0f, 1.0f, 0.0f}, // O - Yellow
    {0.5f, 0.0f, 0.5f}, // T - Purple
    {0.0f, 1.0f, 0.0f}, // S - Green
    {1.0f, 0.0f, 0.0f}, // Z - Red
    {0.0f, 0.0f, 1.0f}, // J - Blue
    {1.0f, 0.5f, 0.0f}  // L - Orange
};

// ================= UTILITY =================
void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

// ================= LOGIC & COLLISION =================
bool checkCollision(int newX, int newY, int shapeMatrix[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (shapeMatrix[r][c] != 0) {
                int x = newX + c;
                int y = newY - r;

                // Wall and floor collision
                if (x < 0 || x >= COLS || y < 0) return true;

                // Placed block collision
                if (y < ROWS && board[y][x] != 0) return true;
            }
        }
    }
    return false;
}

void rotateCurrentShape() {
    // 'O' shape (index 1) does not need rotation
    if (currentShapeIndex == 1) return;

    int temp[4][4] = {0};
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            temp[c][3 - r] = currentShape[r][c];
        }
    }

    if (!checkCollision(pieceX, pieceY, temp)) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                currentShape[r][c] = temp[r][c];
            }
        }
    }
}

void updateLevelAndScore(int pointsToAdd) {
    score += pointsToAdd;
    if (score > highScore) highScore = score;
    level = 1 + (score / 20); // Level calculation formula
}

void clearLines() {
    int linesCleared = 0;
    for (int r = 0; r < ROWS; r++) {
        bool full = true;
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            linesCleared++;
            // Move everything down
            for (int row = r; row < ROWS - 1; row++) {
                for (int col = 0; col < COLS; col++) {
                    board[row][col] = board[row + 1][col];
                }
            }
            // Clear top row
            for (int col = 0; col < COLS; col++) board[ROWS - 1][col] = 0;
            r--; // Recheck current row
        }
    }

    // Score bonus for cleared lines
    if (linesCleared > 0) {
        updateLevelAndScore(linesCleared * 10);
    }
}

void newPiece() {
    pieceX = 3;
    pieceY = 19;
    dropType = 0; // reset drop speed points
    currentShapeIndex = rand() % 7;

    // Copy new shape to active rotating matrix
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            currentShape[r][c] = originalShapes[currentShapeIndex][r][c];
        }
    }

    if (checkCollision(pieceX, pieceY, currentShape)) {
        gameOver = 1;
    }
}

void placePiece() {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (currentShape[r][c] != 0) {
                int x = pieceX + c;
                int y = pieceY - r;
                if (y >= 0 && y < ROWS) {
                    board[y][x] = currentShapeIndex + 1; // +1 to distinguish from empty (0)
                }
            }
        }
    }

    // Apply placement points based on drop type
    int placePoints = 1; // Easy (normal fall)
    if (dropType == 1) placePoints = 2; // Medium (soft drop)
    else if (dropType == 2) placePoints = 3; // Fast (hard drop)

    updateLevelAndScore(placePoints);

    clearLines();
    newPiece();
}

// ================= ENGINE & INPUT =================
void update(int value) {
    if (!gameOver) {
        if (!checkCollision(pieceX, pieceY - 1, currentShape)) {
            pieceY--;
        } else {
            placePiece();
        }
    }
    glutPostRedisplay();
    glutTimerFunc(400, update, 0);
}

void handleSpecialKeypress(int key, int x, int y) {
    if (gameOver) return;

    if (key == GLUT_KEY_UP) {
        rotateCurrentShape();
    }
    else if (key == GLUT_KEY_LEFT) {
        if (!checkCollision(pieceX - 1, pieceY, currentShape)) pieceX--;
    }
    else if (key == GLUT_KEY_RIGHT) {
        if (!checkCollision(pieceX + 1, pieceY, currentShape)) pieceX++;
    }
    else if (key == GLUT_KEY_DOWN) {
        if (!checkCollision(pieceX, pieceY - 1, currentShape)) {
            pieceY--;
            dropType = 1; // Mark as medium drop
        }
    }
    glutPostRedisplay();
}

void handleKeypress(unsigned char key, int x, int y) {
    if (gameOver) {
        if (key == 'r' || key == 'R') {
            score = 0;
            level = 1;
            gameOver = 0;
            for (int r = 0; r < ROWS; r++)
                for (int c = 0; c < COLS; c++)
                    board[r][c] = 0;
            newPiece();
        }
        return;
    }

    // WASD alternatives
    if ((key == 'w' || key == 'W')) rotateCurrentShape();
    if ((key == 'a' || key == 'A') && !checkCollision(pieceX - 1, pieceY, currentShape)) pieceX--;
    if ((key == 'd' || key == 'D') && !checkCollision(pieceX + 1, pieceY, currentShape)) pieceX++;
    if ((key == 's' || key == 'S') && !checkCollision(pieceX, pieceY - 1, currentShape)) {
        pieceY--;
        dropType = 1; // Medium drop
    }

    // Hard drop (Spacebar)
    if (key == ' ') {
        while (!checkCollision(pieceX, pieceY - 1, currentShape)) {
            pieceY--;
        }
        dropType = 2; // Fast drop
        placePiece();
    }

    glutPostRedisplay();
}

// ================= RENDERING =================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Draw Background Splits
    // Left Play Area
    glColor3f(0.05f, 0.05f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(L, U); glVertex2f(L, D); glVertex2f(R, D); glVertex2f(R, U);
    glEnd();

    // Right UI Boundary Line
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex2f(R, U); glVertex2f(R, D);
    glEnd();

    float boxW = (R - L) / COLS;
    float boxH = (U - D) / ROWS;

    // 2. Draw Grid
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int c = 0; c <= COLS; c++) {
        float x = L + c * boxW;
        glVertex2f(x, D); glVertex2f(x, U);
    }
    for (int r = 0; r <= ROWS; r++) {
        float y = D + r * boxH;
        glVertex2f(L, y); glVertex2f(R, y);
    }
    glEnd();

    // 3. Draw Placed Blocks
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] != 0) {
                int colorIndex = board[r][c] - 1;
                float x1 = L + c * boxW;
                float y1 = D + r * boxH;

                glColor3f(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
                glBegin(GL_QUADS);
                    glVertex2f(x1, y1); glVertex2f(x1 + boxW, y1);
                    glVertex2f(x1 + boxW, y1 + boxH); glVertex2f(x1, y1 + boxH);
                glEnd();

                // Draw block borders
                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_LINE_LOOP);
                    glVertex2f(x1, y1); glVertex2f(x1 + boxW, y1);
                    glVertex2f(x1 + boxW, y1 + boxH); glVertex2f(x1, y1 + boxH);
                glEnd();
            }
        }
    }

    // 4. Draw Active Falling Piece
    if (!gameOver) {
        glColor3f(colors[currentShapeIndex][0], colors[currentShapeIndex][1], colors[currentShapeIndex][2]);
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (currentShape[r][c] != 0) {
                    float x1 = L + (pieceX + c) * boxW;
                    float y1 = D + (pieceY - r) * boxH;

                    glBegin(GL_QUADS);
                        glVertex2f(x1, y1); glVertex2f(x1 + boxW, y1);
                        glVertex2f(x1 + boxW, y1 + boxH); glVertex2f(x1, y1 + boxH);
                    glEnd();

                    glColor3f(0.0f, 0.0f, 0.0f); // Outline
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(x1, y1); glVertex2f(x1 + boxW, y1);
                        glVertex2f(x1 + boxW, y1 + boxH); glVertex2f(x1, y1 + boxH);
                    glEnd();

                    // Reset color for next quad
                    glColor3f(colors[currentShapeIndex][0], colors[currentShapeIndex][1], colors[currentShapeIndex][2]);
                }
            }
        }
    }

    // 5. Draw UI Elements (Right Side)
    char text[50];
    glColor3f(1.0f, 1.0f, 1.0f); // White text

    sprintf(text, "SCORE");
    drawText(0.35f, 0.8f, text);
    sprintf(text, "%d", score);
    drawText(0.35f, 0.7f, text);

    sprintf(text, "HIGH SCORE");
    drawText(0.35f, 0.5f, text);
    sprintf(text, "%d", highScore);
    drawText(0.35f, 0.4f, text);

    sprintf(text, "LEVEL");
    drawText(0.35f, 0.2f, text);
    sprintf(text, "%d", level);
    drawText(0.35f, 0.1f, text);

    // Controls Help Text
    glColor3f(0.6f, 0.6f, 0.6f);
    drawText(0.25f, -0.4f, "W / UP: Rotate");
    drawText(0.25f, -0.5f, "A / D:  Move");
    drawText(0.25f, -0.6f, "S / DN: Soft Drop");
    drawText(0.25f, -0.7f, "SPACE:  Hard Drop");

    // Game Over Overlay
    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText(-0.4f, 0.1f, "G A M E  O V E R");
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(-0.45f, -0.1f, "Press 'R' to Restart");
    }

    glFlush();
}

// ================= INIT & MAIN =================
void initGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    srand(time(0));
    newPiece();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 700);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Tetris Game - Merged");

    initGL();

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress); // For Arrow Keys
    glutTimerFunc(400, update, 0);

    glutMainLoop();
    return 0;
}

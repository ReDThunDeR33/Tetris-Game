#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>

// Board size
const int ROWS = 20;
const int COLS = 10;

// Board
int board[ROWS][COLS] = {0};

// Piece position
int pieceX = 4;
int pieceY = 19;

// Score system
int score = 0;
int highScore = 0;
int gameOver = 0;

// Shapes
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
    {0.4f,0.7f,0.7f},{0.8f,0.8f,0.4f},{0.6f,0.4f,0.7f},
    {0.4f,0.7f,0.4f},{0.8f,0.4f,0.4f},{0.4f,0.4f,0.7f},
    {0.8f,0.6f,0.4f}
};

int currentShapeIndex;

// ================= TEXT =================
void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
    }
}

// ================= COLLISION =================
bool checkCollision(int newX, int newY) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {

            if (shapes[currentShapeIndex][r][c]) {

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

// ================= PLACE PIECE =================
void placePiece() {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {

            if (shapes[currentShapeIndex][r][c]) {

                int x = pieceX + c;
                int y = pieceY - r;

                if (y >= 0 && y < ROWS)
                    board[y][x] = currentShapeIndex + 1;
            }
        }
    }
}

// ================= CLEAR LINES =================
void clearLines() {
    for (int r = 0; r < ROWS; r++) {

        bool full = true;

        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            // Move everything down
            for (int row = r; row < ROWS - 1; row++) {
                for (int col = 0; col < COLS; col++) {
                    board[row][col] = board[row + 1][col];
                }
            }

            // Clear top row
            for (int col = 0; col < COLS; col++)
                board[ROWS - 1][col] = 0;

            r--; // recheck row

            score += 5;

            if (score > highScore)
                highScore = score;
        }
    }
}

// ================= NEW PIECE =================
void newPiece() {
    pieceX = 4;
    pieceY = 19;
    currentShapeIndex = rand() % 7;

    if (checkCollision(pieceX, pieceY)) {
        gameOver = 1;
    }
}

// ================= UPDATE =================
void update(int value) {

    if (gameOver) return;

    if (!checkCollision(pieceX, pieceY - 1)) {
        pieceY--;
    } else {
        placePiece();
        clearLines();
        newPiece();
    }

    glutPostRedisplay();
    glutTimerFunc(400, update, 0);
}

// ================= INPUT =================
void handleKeypress(unsigned char key, int x, int y) {

    if (gameOver) {
        if (key == 'r' || key == 'R') {
            score = 0;
            gameOver = 0;

            // clear board
            for (int r = 0; r < ROWS; r++)
                for (int c = 0; c < COLS; c++)
                    board[r][c] = 0;

            newPiece();
        }
        return;
    }

    if (key == 'a' && !checkCollision(pieceX - 1, pieceY))
        pieceX--;

    if (key == 'd' && !checkCollision(pieceX + 1, pieceY))
        pieceX++;

    glutPostRedisplay();
}

// ================= DISPLAY =================
void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    float L = -1.0f, R = 0.2f;
    float D = -1.0f, U = 1.0f;

    float boxW = (R - L) / COLS;
    float boxH = (U - D) / ROWS;

    // Grid
    glColor3f(0.7f,0.7f,0.7f);
    glBegin(GL_LINES);

    for (int c = 0; c <= COLS; c++) {
        float x = L + c * boxW;
        glVertex2f(x,D); glVertex2f(x,U);
    }

    for (int r = 0; r <= ROWS; r++) {
        float y = D + r * boxH;
        glVertex2f(L,y); glVertex2f(R,y);
    }

    glEnd();

    // Placed blocks
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {

            if (board[r][c]) {

                int s = board[r][c] - 1;

                float x1 = L + c * boxW;
                float y1 = D + r * boxH;

                glColor3f(colors[s][0],colors[s][1],colors[s][2]);

                glBegin(GL_QUADS);
                glVertex2f(x1,y1);
                glVertex2f(x1+boxW,y1);
                glVertex2f(x1+boxW,y1+boxH);
                glVertex2f(x1,y1+boxH);
                glEnd();
            }
        }
    }

    // Falling piece
    glColor3f(colors[currentShapeIndex][0],
              colors[currentShapeIndex][1],
              colors[currentShapeIndex][2]);

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {

            if (shapes[currentShapeIndex][r][c]) {

                int x = pieceX + c;
                int y = pieceY - r;

                float x1 = L + x * boxW;
                float y1 = D + y * boxH;

                glBegin(GL_QUADS);
                glVertex2f(x1,y1);
                glVertex2f(x1+boxW,y1);
                glVertex2f(x1+boxW,y1+boxH);
                glVertex2f(x1,y1+boxH);
                glEnd();
            }
        }
    }

    // ================= UI =================
    char text[50];

    sprintf(text,"Score: %d",score);
    drawText(0.3f,0.8f,text);

    sprintf(text,"High: %d",highScore);
    drawText(0.3f,0.7f,text);

    if (gameOver) {
        drawText(-0.3f,0.0f,"GAME OVER");
        drawText(-0.4f,-0.1f,"Press R to Restart");
    }

    glFlush();
}

// ================= INIT =================
void init() {
    glClearColor(0,0,0,1);
    srand(time(0));
    newPiece();
}

// ================= MAIN =================
int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500,600);
    glutCreateWindow("Tetris Game");

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(400, update, 0);

    init();
    glutMainLoop();

    return 0;
}

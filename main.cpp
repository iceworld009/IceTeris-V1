#include <iostream>
#include <conio.h>
#include <ctime>
using namespace std;
int speed=900000,multiplier=300;
const int HEIGHT = 20, WIDTH = 10;
int board[HEIGHT + 2][WIDTH + 2] = {0};
int x, y, shape, rot, score = 0;
bool gameOver = false;
bool ansi_supported = true;

const char T[5][4][4][4] = {
    // I
    {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
     {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
     {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}},
    // O
    {{{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
     {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
     {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
     {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}},
    // L
    {{{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
     {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
     {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}},
    // T
    {{{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
     {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
     {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}},
    // S
    {{{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
     {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
     {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}}}
};

void moveCursorTopLeft() {
    if (ansi_supported) cout << "\x1B[H";
    else for (int i = 0; i < 30; i++) cout << '\n';
}

void draw() {
    moveCursorTopLeft();
    cout << "Score: " << score << "\n\n";
    for (int i = 1; i <= HEIGHT; i++) {
        cout << "  ";
        for (int j = 1; j <= WIDTH; j++) {
            if (board[i][j] == 0) cout << ". ";
            else if (board[i][j] == 1) cout << "@ ";
            else cout << "# ";
        }
        cout << "\n";
    }
    cout << "\nControls: a-left d-right w-rotate s-drop r-quit\n";
}

bool canPlace(int nx, int ny, int r) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (T[shape][r][i][j]) {
                int xi = nx + i, yj = ny + j;
                if (xi < 1 || xi > HEIGHT || yj < 1 || yj > WIDTH || board[xi][yj] == 2)
                    return false;
            }
    return true;
}

void putBlock(int val) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (T[shape][rot][i][j])
                board[x + i][y + j] = val;
}

void newBlock() {
    shape = rand() % 5;
    rot = 0;
    x = 1;
    y = WIDTH / 2 - 1;
    if (!canPlace(x, y, rot)) gameOver = true;
    else putBlock(1);
}

void fixBlock() {
    putBlock(2);
}

void clearLines() {
    for (int i = HEIGHT; i >= 1; i--) {
        bool full = true;
        for (int j = 1; j <= WIDTH; j++)
            if (board[i][j] != 2) full = false;
        if (full) {
            for (int k = i; k >= 2; k--)
                for (int j = 1; j <= WIDTH; j++)
                    board[k][j] = board[k - 1][j];
            for (int j = 1; j <= WIDTH; j++)
                board[1][j] = 0;
            score += 100;
            i++;
        }
    }
}

void handleInput() {
    if (_kbhit()) {
        char ch = _getch();
        putBlock(0);
        if (ch == 'a' && canPlace(x, y - 1, rot)) y--;
        else if (ch == 'd' && canPlace(x, y + 1, rot)) y++;
        else if (ch == 's') while (canPlace(x + 1, y, rot)) x++;
        else if (ch == 'w') {
            int next = (rot + 1) % 4;
            if (canPlace(x, y, next)) rot = next;
        }
        else if (ch == 'r') gameOver = true;
        else if(ch=='m')multiplier+=10;
        else if(ch=='n')multiplier-=10;
        putBlock(1);
    }
}

void tick() {
    putBlock(0);
    if (canPlace(x + 1, y, rot)) {
        x++;
        putBlock(1);
    } else {
        putBlock(1);
        fixBlock();
        clearLines();
        newBlock();
    }
}

void delay() {
    for (int i = 0; i < speed; i++)
        for (int j = 0; j < multiplier; j++);

}

int main() {
    srand(time(0));
    cout << "\x1B[2J";
    cout << "Do you see a clean screen? (y/n): ";
    char ans = _getch();
    if (ans == 'n' || ans == 'N') ansi_supported = false;

    newBlock();
    while (!gameOver) {
        draw();
        handleInput();
        tick();
        delay();
    }
    cout << "\nGAME OVER. Final Score: " << score << "\n";
    return 0;
}

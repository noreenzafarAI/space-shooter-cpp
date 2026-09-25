#include <iostream> 
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
using namespace std;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class SpaceShooter {
private:
    int width, height;
    int x, y;
    int score;
    bool gameover;

    struct Coordinate {
        int x, y;
    };

    vector<Coordinate> bullets;
    vector<Coordinate> enemies;

public:
    SpaceShooter(int width, int height): width(width), height(height), x(width/2), y(height-5), score(0), gameover(false) {}

    void drawPlayer() {
        gotoxy(x-2, y);   cout << " /^\\ ";
        gotoxy(x-2, y+1); cout << "/   \\";
        gotoxy(x-2, y+2); cout << "|___|";
    }

    void erasePlayer() {
        for (int i = 0; i < 3; i++) {
            gotoxy(x-2, y+i);
            cout << "      "; // Increased to 6 spaces to fully erase
        }
    }

    void moveLeft() {
        if (x > 3) {
            erasePlayer();
            x--;
            drawPlayer();
        }
    }

    void moveRight() {
        if (x < width - 4) {
            erasePlayer();
            x++;
            drawPlayer();
        }
    }

    void shoot() {
        bullets.push_back({x, y-1});
    }

    void drawBullets() {
        for (auto& b : bullets) {
            gotoxy(b.x, b.y);
            cout << "|";
        }
    }

    void eraseBullets() {
        for (auto& b : bullets) {
            gotoxy(b.x, b.y);
            cout << " ";
        }
    }

    void moveBullets() {
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i].y--;

            for (int j = 0; j < enemies.size(); j++) {
                if (abs(bullets[i].x - enemies[j].x) <= 2 && 
                    abs(bullets[i].y - enemies[j].y) <= 1) {
                    bullets.erase(bullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    score += 10;
                    i--;
                    break;
                }
            }

            if (i >= 0 && bullets[i].y <= 1) {
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }
    }

    void drawEnemies() {
        for (auto& e : enemies) {
            gotoxy(e.x-1, e.y);   cout << "\\v/";
           
        }
    }

    void eraseEnemies() {
        for (auto& e : enemies) {
            gotoxy(e.x-1, e.y);   cout << "   ";
            gotoxy(e.x, e.y+1);   cout << "   ";
        }
    }

    void moveEnemies() {
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].y++;

            if ((abs(enemies[i].x - x) <= 2) && 
                (abs(enemies[i].y - y) <= 3)) {
                gameover = true;
                break;
            }

            if (enemies[i].y >= height - 1) {
                enemies.erase(enemies.begin() + i);
                i--;
            }
        }
    }

    void generateEnemies() {
        if (rand() % 100 < 5) {
            int enemyX = rand() % (width - 4) + 2;
            enemies.push_back({enemyX, 2});
        }
    }

    bool isGameOver() const {
        return gameover;
    }

    int getScore() const {
        return score;
    }

    void drawScore() {
        gotoxy(width / 2 - 5, 0);
        cout << "Score: " << score;
    }
};

int main() {
    system("mode con: lines=40 cols=60");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    srand(time(0));
    int width = 60;
    int height = 40;
    SpaceShooter game(width, height);

    // Instructions
    cout << "Space Shooter Game" << endl;
    cout << "" << endl;
    cout << "1. Use LEFT and RIGHT arrow keys to move" << endl;
    cout << "2. Press SPACEBAR to fire" << endl;
    cout << "3. Press ESC to quit game" << endl << endl;

    cout << "Your aircraft: " << endl;
    cout << " /^\\ " << endl;
    cout << "/   \\" << endl;
    cout << "|___|" << endl << endl;

    cout << "Enemy aircraft: " << endl;
    cout << "\\v/" << endl;
   

    system("pause");
    system("cls");

    // Draw initial player position
    game.drawPlayer();

    while (!game.isGameOver()) {
        // Input
        if (_kbhit()) {
            char ch = _getch();
            if (ch == -32 || ch == 0) {
                ch = _getch();
                if (ch == 75) game.moveLeft();
                else if (ch == 77) game.moveRight();
            } else if (ch == ' ') {
                game.shoot();
            } else if (ch == 27) {
                break;
            }
        }

        // Erase Previous
        game.eraseBullets();
        game.eraseEnemies();

        // Update State
        game.moveBullets();
        game.moveEnemies();
        game.generateEnemies();

        // Draw New Frame
        game.drawBullets();
        game.drawEnemies();
        game.drawScore();

        Sleep(80);  // frame delay
    }

    // Game Over screen
    system("cls");
    gotoxy(width / 2 - 5, height / 2);
    cout << "Game Over!";
    gotoxy(width / 2 - 7, height / 2 + 1);
    cout << "Your Score: " << game.getScore() << endl;

    system("pause");
return 0;
}
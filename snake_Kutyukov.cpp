#include <iostream>
#include <vector>
#include <iterator>
#include <random>
#include <conio.h>
#include <deque>  
#include "windows.h"

const char SNAKE_HEAD = '@';
const char SNAKE_BODY = 'o';
const char APPLE = '*';     
const char WALL = '#';      
const char EMPTY = '.';   
const int snakeDelay = 150;
const std::string InputWidthMessage = "Введите ширину поля (8-20): ";
const std::string InputHeightMessage = "Введите высоту поля (8-20): ";
const std::string Score = "Score: ";
const std::string GameOver = "Game Over!";
const std::string ThanksMessage = "Спасибо за игру!";
const std::string RestartMessage = "Хотите перезапустить игру? (y/Y): ";

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int _x, int _y) : x(_x), y(_y) {}
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
private:
    std::vector<std::vector<char>> field; 
    std::deque<Point> snake;              
    Point apple;                          
    Direction dir;                        
    bool gameOver;                        
    int score;                            
    int width, height;                    

public:
    // Инициализация игрового поля
    SnakeGame(int w, int h) : width(w), height(h), dir(RIGHT), gameOver(false), score(0) {
        field = std::vector<std::vector<char>>(height, std::vector<char>(width, EMPTY));
   //использовать одинарные for
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                    field[i][j] = WALL;
                }
            }
        }
        snake.push_back(Point(width / 2, height / 2));  
        generateApple();
        updateField();
    }

    void generateApple() {
        std::random_device rd;
        std::uniform_int_distribution<int> distX(1, width - 2); 
        std::uniform_int_distribution<int> distY(1, height - 2); 
        apple.x = distX(rd);
        apple.y = distY(rd);
    }

    void updateField() {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (field[i][j] != WALL) {
                    field[i][j] = EMPTY;
                }
            }
        }
        field[apple.y][apple.x] = APPLE;
        // Отобразить змейку
        if (!snake.empty()) {
            field[snake.front().y][snake.front().x] = SNAKE_HEAD;
            for (size_t i = 1; i < snake.size(); ++i) {
                field[snake[i].y][snake[i].x] = SNAKE_BODY;
            }
        }
    }
//отключить курсор, не использовать system cls
    void drawField() {
        system("cls");  
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                std::cout << field[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << Score << score << std::endl;
    }

    void moveSnake() {
        Point newHead = snake.front();

        switch (dir) {
        case UP:    
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
        }

        if (newHead.x <= 0 || newHead.x >= width - 1 || newHead.y <= 0 || newHead.y >= height - 1) {
            gameOver = true;
            return;
        }

        for (const auto& segment : snake) {
            if (segment.x == newHead.x && segment.y == newHead.y) {
                gameOver = true;
                return;
            }
        }

        if (newHead.x == apple.x && newHead.y == apple.y) {
            score++;
            snake.push_front(newHead);
            generateApple();  
        }
        else {
            snake.push_front(newHead);
            snake.pop_back();
        }

        updateField();
    }

    void input() {
        if (_kbhit()) {  
            char ch = _getch();
            if (ch == 'w' || ch == 'W' || ch == 72) { 
                if (dir != DOWN) dir = UP;
            }
            else if (ch == 's' || ch == 'S' || ch == 80) { 
                if (dir != UP) dir = DOWN;
            }
            else if (ch == 'a' || ch == 'A' || ch == 75) { 
                if (dir != RIGHT) dir = LEFT;
            }
            else if (ch == 'd' || ch == 'D' || ch == 77) { 
                if (dir != LEFT) dir = RIGHT;
            }
        }
    }

    void run() {
        while (!gameOver) {
            input();   
            moveSnake(); 
            drawField(); 
            Sleep(snakeDelay);  
        }
        std::cout << GameOver << std::endl;
    }

    bool restart() {
        char choice;
        std::cout << RestartMessage;
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            snake.clear();
            score = 0;
            gameOver = false;
            snake.push_back(Point(width / 2, height / 2)); 
            generateApple();
            updateField();
            return true;  
        }
        else {
            std::cout << ThanksMessage << std::endl;
            return false; 
        }
    }
};

int main() {
    setlocale(LC_ALL, "RU"); 
    int width, height;
    std::cout << InputWidthMessage;
    std::cin >> width;
    std::cout << InputHeightMessage;
    std::cin >> height;
    SnakeGame game(width, height);

    bool playAgain = true;
    do {
        game.run();  
        playAgain = game.restart(); 
    } 
    while (playAgain); 
    return 0;
}
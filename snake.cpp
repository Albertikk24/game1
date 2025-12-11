/*********************************
 * Автор:     Иккерт А.С.        *
 * Название:  змейка             *
 * Вариант:   7                  *
 *********************************/


#include <iostream>
#include <conio.h> // для _kbhit() и _getch()
#include <windows.h> // для Sleep()
#include <vector>

using namespace std;

// Константы для размера поля
const int WIDTH = 20;
const int HEIGHT = 20;

// Направления движения змейки
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Класс для координат
class Position {
public:
    int x, y;
    
    Position(int x = 0, int y = 0) : x(x), y(y) {}
};

// Класс Змейка
class Snake {
private:
    vector<Position> body; // вектор для хранения сегментов тела
    Direction dir; // текущее направление
    bool gameOver; // состояние игры
    Position food; // позиция еды
    int score; // счет
    
public:
    Snake() {
        // Инициализация змейки
        gameOver = false;
        dir = STOP;
        score = 0;
        
        // Начальная позиция змейки (посередине поля)
        body.push_back(Position(WIDTH / 2, HEIGHT / 2));
        
        // Генерируем первую еду
        generateFood();
    }
    
    // Генерация еды в случайном месте
    void generateFood() {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        
        // Проверяем, чтобы еда не появилась на змейке
        for (auto segment : body) {
            if (segment.x == food.x && segment.y == food.y) {
                generateFood(); // если попала на змейку, генерируем заново
                break;
            }
        }
    }
    
    // Отрисовка игрового поля
    void draw() {
        system("cls"); // очищаем консоль (для Windows)
        
        // Верхняя граница
        for (int i = 0; i < WIDTH + 2; i++)
            cout << "#";
        cout << endl;
        
        // Игровое поле
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                // Левая граница
                if (x == 0) cout << "#";
                
                // Голова змейки
                if (x == body[0].x && y == body[0].y)
                    cout << "O";
                // Тело змейки
                else if (isBodySegment(x, y))
                    cout << "o";
                // Еда
                else if (x == food.x && y == food.y)
                    cout << "F";
                // Пустая клетка
                else
                    cout << " ";
                
                // Правая граница
                if (x == WIDTH - 1) cout << "#";
            }
            cout << endl;
        }
        
        // Нижняя граница
        for (int i = 0; i < WIDTH + 2; i++)
            cout << "#";
        cout << endl;
        
        // Вывод счета
        cout << "Score: " << score << endl;
        cout << "Controls: WASD to move, X to exit" << endl;
    }
    
    // Проверка, является ли клетка частью тела
    bool isBodySegment(int x, int y) {
        for (int i = 1; i < body.size(); i++) {
            if (body[i].x == x && body[i].y == y)
                return true;
        }
        return false;
    }
    
    // Обработка ввода с клавиатуры
    void input() {
        if (_kbhit()) { // если была нажата клавиша
            switch (_getch()) { // получаем код клавиши
                case 'a':
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 'd':
                    if (dir != LEFT) dir = RIGHT;
                    break;
                case 'w':
                    if (dir != DOWN) dir = UP;
                    break;
                case 's':
                    if (dir != UP) dir = DOWN;
                    break;
                case 'x':
                    gameOver = true;
                    break;
            }
        }
    }
    
    // Логика игры
    void logic() {
        if (dir == STOP) return;
        
        // Сохраняем старую позицию головы
        Position prevHead = body[0];
        Position prev = prevHead, curr;
        
        // Двигаем голову
        switch (dir) {
            case LEFT:
                body[0].x--;
                break;
            case RIGHT:
                body[0].x++;
                break;
            case UP:
                body[0].y--;
                break;
            case DOWN:
                body[0].y++;
                break;
        }
        
        // Двигаем остальные сегменты тела
        for (int i = 1; i < body.size(); i++) {
            curr = body[i];
            body[i] = prev;
            prev = curr;
        }
        
        // Если съели еду
        if (body[0].x == food.x && body[0].y == food.y) {
            score += 10;
            // Добавляем новый сегмент в конец
            body.push_back(prev);
            // Генерируем новую еду
            generateFood();
        }
        
        // Проверка столкновения со стенами
        if (body[0].x < 0 || body[0].x >= WIDTH || 
            body[0].y < 0 || body[0].y >= HEIGHT) {
            gameOver = true;
        }
        
        // Проверка столкновения с собой
        for (int i = 1; i < body.size(); i++) {
            if (body[0].x == body[i].x && body[0].y == body[i].y) {
                gameOver = true;
                break;
            }
        }
    }
    
    // Запуск игры
    void run() {
        while (!gameOver) {
            draw();     // рисуем поле
            input();    // обрабатываем ввод
            logic();    // обновляем логику
            Sleep(100); // задержка для регулировки скорости (в миллисекундах)
        }
        
        // Конец игры
        system("cls");
        cout << "======== GAME OVER ========" << endl;
        cout << "Your score: " << score << endl;
        cout << "===========================" << endl;
    }
};

int main() {
    // Устанавливаем seed для случайных чисел
    srand(time(NULL));
    
    // Создаем и запускаем игру
    Snake game;
    game.run();
    
    return 0;
}
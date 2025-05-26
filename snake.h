#ifndef SNAKE_H
#define SNAKE_H

#define WIDTH 40
#define HEIGHT 20
#define MAX_LEN 100

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[MAX_LEN];
    int length;
    int dir; // 0: up, 1: right, 2: down, 3: left
} Snake;

void gotoxy(int x, int y);
void hidecursor();
void draw_border();
void initGame(Snake *snake, Point *food);
void placeFood(Snake *snake, Point *food);
int checkCollision(Snake *snake, int x, int y);
int update(Snake *snake, Point *food);
void draw(Snake *snake, Point *food);
void saveHighScore(const char *name, int score);
void showHighScores();

#endif

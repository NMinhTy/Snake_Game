#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "snake.h"
#include <conio.h>

void gotoxy(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidecursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {100, FALSE};
    SetConsoleCursorInfo(consoleHandle, &info);
}

void draw_border() {
    for (int i = 0; i <= WIDTH + 1; i++) {
        gotoxy(i, 0); printf("#");
        gotoxy(i, HEIGHT + 1); printf("#");
    }
    for (int i = 0; i <= HEIGHT + 1; i++) {
        gotoxy(0, i); printf("#");
        gotoxy(WIDTH + 1, i); printf("#");
    }
}

void initGame(Snake *snake, Point *food) {
    snake->length = 3;
    snake->dir = 1;
    int startX = WIDTH / 2;
    int startY = HEIGHT / 2;

    for (int i = 0; i < snake->length; i++) {
        snake->body[i].x = startX - i;
        snake->body[i].y = startY;
    }

    placeFood(snake, food);
}

void placeFood(Snake *snake, Point *food) {
    int collision;
    do {
        collision = 0;
        food->x = rand() % WIDTH + 1;
        food->y = rand() % HEIGHT + 1;
        for (int i = 0; i < snake->length; i++) {
            if (snake->body[i].x == food->x && snake->body[i].y == food->y) {
                collision = 1;
                break;
            }
        }
    } while (collision);
}

int checkCollision(Snake *snake, int x, int y) {
    if (x <= 0 || x > WIDTH || y <= 0 || y > HEIGHT)
        return 1;
    for (int i = 0; i < snake->length; i++) {
        if (snake->body[i].x == x && snake->body[i].y == y)
            return 1;
    }
    return 0;
}

int update(Snake *snake, Point *food) {
    Point newHead = snake->body[0];
    switch (snake->dir) {
        case 0: newHead.y--; break;
        case 1: newHead.x++; break;
        case 2: newHead.y++; break;
        case 3: newHead.x--; break;
    }

    if (checkCollision(snake, newHead.x, newHead.y)) {
        return 0; // va chạm
    }

    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = newHead;

    if (newHead.x == food->x && newHead.y == food->y) {
        if (snake->length < MAX_LEN) snake->length++;
        placeFood(snake, food);
    }

    return 1;
}

void draw(Snake *snake, Point *food) {
    system("cls");
    draw_border();

    gotoxy(food->x, food->y);
    printf("F");

    for (int i = 0; i < snake->length; i++) {
        gotoxy(snake->body[i].x, snake->body[i].y);
        printf(i == 0 ? "O" : "o");
    }

    gotoxy(WIDTH - 10, 1);
    printf("Score: %d", snake->length - 3);
}

void saveHighScore(const char *name, int score) {
    FILE *f = fopen("highscores.txt", "a+");
    if (!f) return;

    int id = 1;
    char line[128];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] != '\n') id++;
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d", tm_info);

    fprintf(f, "%d,%s,%s,%d\n", id, date, name, score);
    fclose(f);
}

// --- Phần cập nhật: showHighScores theo thứ tự giảm dần score ---

typedef struct {
    int id;
    char date[20];
    char name[32];
    int score;
} HighScoreEntry;

int compareScoresDesc(const void *a, const void *b) {
    HighScoreEntry *entryA = (HighScoreEntry *)a;
    HighScoreEntry *entryB = (HighScoreEntry *)b;
    return entryB->score - entryA->score; // Sắp xếp giảm dần theo score
}

void showHighScores() {
    system("cls");
    printf("=== HIGH SCORES ===\n\n");

    FILE *f = fopen("highscores.txt", "r");
    if (!f) {
        printf("No high scores yet.\n");
    } else {
        HighScoreEntry scores[100];  // Giả sử tối đa 100 bản ghi
        int count = 0;

        char line[128];
        while (fgets(line, sizeof(line), f) && count < 100) {
            if (sscanf(line, "%d,%19[^,],%31[^,],%d",
                       &scores[count].id,
                       scores[count].date,
                       scores[count].name,
                       &scores[count].score) == 4) {
                count++;
            }
        }
        fclose(f);

        // Sắp xếp danh sách điểm theo điểm giảm dần
        qsort(scores, count, sizeof(HighScoreEntry), compareScoresDesc);

        // In ra danh sách đã sắp xếp
        for (int i = 0; i < count; i++) {
            printf("%d. %s - %s - %d\n",
                   scores[i].id,
                   scores[i].date,
                   scores[i].name,
                   scores[i].score);
        }
    }

    printf("\nPress any key to return to menu...");
    getch();
}

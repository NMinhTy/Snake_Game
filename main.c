#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "snake.h"

void showMenu() {
    system("cls");
    printf("==== SNAKE GAME ====\n\n");
    printf("1. Start Game\n");
    printf("2. View Highscores\n");
    printf("3. Exit\n");
    printf("\nSelect option: ");
}

void playGame() {
    Snake snake;
    Point food;
    char name[32];

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    initGame(&snake, &food);
    hidecursor();

    while (1) {
        if (kbhit()) {
            char ch = getch();
            switch (ch) {
                case 'w': case 'W': if (snake.dir != 2) snake.dir = 0; break;
                case 'd': case 'D': if (snake.dir != 3) snake.dir = 1; break;
                case 's': case 'S': if (snake.dir != 0) snake.dir = 2; break;
                case 'a': case 'A': if (snake.dir != 1) snake.dir = 3; break;
                case 'q': case 'Q':
                    system("cls");
                    printf("Quit game. Score: %d\n", snake.length - 3);
                    Sleep(1000);
                    return;
            }
        }

        if (!update(&snake, &food)) {
            system("cls");
            printf("=== GAME OVER ===\n");
            printf("Score: %d\n", snake.length - 3);
            saveHighScore(name, snake.length - 3);
            printf("\nReturning to menu...\n");
            Sleep(2000);
            break;
        }

        draw(&snake, &food);
        Sleep(100);
    }
}

int main() {
    srand((unsigned int)time(NULL));

    while (1) {
        showMenu();
        char choice = getch();
        switch (choice) {
            case '1':
                system("cls");
                playGame();
                break;
            case '2':
                showHighScores();
                break;
            case '3':
                return 0;
            default:
                printf("\nInvalid option.\n");
                Sleep(1000);
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define ROWS 6
#define COLS 6
#define MAX_LENGTH (ROWS * COLS)

typedef struct {
    int row, col;
} Segment;

Segment snake[MAX_LENGTH];
int length = 1;
char direction = 'd'; // Start moving right

int **createBoard() {
    int **board = malloc(ROWS * sizeof(int *));
    for (int i = 0; i < ROWS; i++) {
        board[i] = calloc(COLS, sizeof(int));
    }
    return board;
}

void destroyBoard(int **board) {
    for (int i = 0; i < ROWS; i++) {
        free(board[i]);
    }
    free(board);
}

void printBoard(int **board) {
    system("cls");

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int isHead = (snake[0].row == i && snake[0].col == j);
            int isBody = 0;

            for (int k = 1; k < length; k++) {
                if (snake[k].row == i && snake[k].col == j) {
                    isBody = 1;
                    break;
                }
            }

            if (isHead)
                printf("h "); // Head 
            else if (isBody)
                printf("b "); // Body 
            else if (board[i][j] == 2)
                printf("f "); // Food
            else
                printf(". ");
        }
        printf("\n");
    }
}

void placeFood(int **board) {
    int row, col;
    do {
        row = rand() % ROWS;
        col = rand() % COLS;
    } while (board[row][col] != 0);
    board[row][col] = 2;
}

void moveSnake(int **board) {
    Segment head = snake[0];
    Segment newHead = head;

    if (direction == 'w') newHead.row--;
    if (direction == 's') newHead.row++;
    if (direction == 'a') newHead.col--;
    if (direction == 'd') newHead.col++;

    // Wall collision
    if (newHead.row < 0 || newHead.row >= ROWS || newHead.col < 0 || newHead.col >= COLS) {
        printf("oh.. Snake hit the wall! Game Over!\n");
        exit(0);
    }

    // Self-collision check
    for (int i = 0; i < length; i++) {
        if (snake[i].row == newHead.row && snake[i].col == newHead.col) {
            printf("well Snake bit itself! Game Over!\n");
            exit(0);
        }
    }

    int ate = (board[newHead.row][newHead.col] == 2);

    for (int i = length; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = newHead;

    if (ate) {
        length++;
        if (length == MAX_LENGTH) {
            printf("yay You filled the board! You win nya~!\n");
            exit(0);
        }
        placeFood(board);
    } else {
        Segment tail = snake[length];
        board[tail.row][tail.col] = 0;
    }

    // Mark new snake position
    for (int i = 0; i < length; i++) {
        board[snake[i].row][snake[i].col] = 1;
    }
}

int main() {
    srand(time(NULL));
    int **board = createBoard();

    // Start snake at (1,1)
    snake[0].row = 1;
    snake[0].col = 1;
    board[1][1] = 1;
    placeFood(board);

    while (1) {
        if (_kbhit()) {
            char input = _getch();
            if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
                direction = input;
            }
        }

        moveSnake(board);
        printBoard(board);
        Sleep(400);
    }

    destroyBoard(board);
    return 0;
}
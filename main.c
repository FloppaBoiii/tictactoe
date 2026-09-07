#include <stdio.h>

#include "raylib.h"
#include "menu.h"
#include "synthmusic.h"

int checkWin(char board[], char currentPlayer) {

    int win = 0;

    for (int i = 0; i < 9; i += 3) {

        if (board[i] == currentPlayer &&
            board[i + 1] == currentPlayer &&
            board[i + 2] == currentPlayer) {

            win = 1;

        }

    }

    for (int i = 0; i < 3; i++) {

        if (board[i] == currentPlayer &&
            board[i + 3] == currentPlayer &&
            board[i + 6] == currentPlayer) {

            win = 1;

        }

    }

    int diagonal1 = 1;
    int diagonal2 = 1;

    for (int row = 0; row < 3; row++) {

        if (board[row * 3 + row] != currentPlayer) {

            diagonal1 = 0;

        }

        if (board[row * 3 + (2 - row)] != currentPlayer) {

            diagonal2 = 0;

        }

    }

    if (diagonal1 == 1 || diagonal2 == 1) {

        win = 1;

    }

    return win;

}

int main() {

    InitWindow(600, 600, "Tic-Tac-Toe");

    Image icon = LoadImage("icon.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(icon);
    UnloadImage(icon);

    InitAudioDevice();

    InitSynthMusic();

    int gameState = 0;

    printf("Hello, Tic-Tac-Toe!\n");

    char currentPlayer = 'X';

    char board[9] = {

        ' ', ' ', ' ',
        ' ', ' ', ' ',
        ' ', ' ', ' '

    };

    int turn = 0;

    int cellSize = 200;

    int gameWon = 0;

    char winner = ' ';

    Color background = { 8, 11, 32, 255 };

    Color boardColor = { 232, 60, 255, 255 };

    Color cyan = { 120, 245, 255, 255 };

    Color pink = { 232, 60, 255, 255 };

    Color purple = { 122, 60, 255, 255 };

    Color white = { 216, 247, 255, 255 };

    Color buttonColor = { 10, 25, 55, 255 };

    while (!WindowShouldClose()) {

        int symbolSize = cellSize / 2 - 7;

        if (gameState == 0) {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                int mouseX = GetMouseX();

                int mouseY = GetMouseY();

                if (mouseX >= 214 && mouseX <= 386 &&
                    mouseY >= 284 && mouseY <= 340) {

                    gameState = 1;

                }

                if (mouseX >= 15 && mouseX <= 55 &&
                    mouseY >= 15 && mouseY <= 55) {

                    gameState = 3;

                }

            }

        }

        else if (gameState == 1) {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                int mouseX = GetMouseX();

                int mouseY = GetMouseY();

                int column = mouseX / cellSize;

                int row = mouseY / cellSize;

                int i = row * 3 + column;

                if (board[i] == ' ') {

                    board[i] = currentPlayer;

                    turn++;

                    if (checkWin(board, currentPlayer) == 1) {

                        printf("Player %c wins!\n", currentPlayer);

                        gameWon = 1;

                        winner = currentPlayer;

                        gameState = 2;

                    }

                    else if (turn == 9) {

                        printf("It's a draw!\n");

                        gameState = 2;

                    }

                    else {

                        if (currentPlayer == 'X') {

                            currentPlayer = 'O';

                        }

                        else {

                            currentPlayer = 'X';

                        }

                    }

                }

            }

        }

        else if (gameState == 2) {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                int mouseX = GetMouseX();

                int mouseY = GetMouseY();

                if (mouseX >= 200 && mouseX <= 400 &&
                    mouseY >= 390 && mouseY <= 450) {

                    gameState = 0;

                    currentPlayer = 'X';

                    board[0] = ' ';
                    board[1] = ' ';
                    board[2] = ' ';
                    board[3] = ' ';
                    board[4] = ' ';
                    board[5] = ' ';
                    board[6] = ' ';
                    board[7] = ' ';
                    board[8] = ' ';

                    turn = 0;

                    gameWon = 0;

                    winner = ' ';

                }

            }

        }

        else if (gameState == 3) {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                int mouseX = GetMouseX();

                int mouseY = GetMouseY();

                if (mouseX >= 15 && mouseX <= 55 &&
                    mouseY >= 15 && mouseY <= 55) {

                    gameState = 0;

                }

            }

        }

        BeginDrawing();

        ClearBackground(background);

        if (gameState == 0) {

            drawMenu();

            DrawCircleLines(35, 35, 22, (Color){ 0, 229, 255, 35 });

            DrawCircleLines(35, 35, 19, (Color){ 0, 229, 255, 60 });

            DrawCircleLines(35, 35, 18, cyan);

            DrawCircleLines(35, 35, 7, cyan);

            DrawLine(35, 11, 35, 4, cyan);
            DrawLine(35, 66, 35, 59, cyan);
            DrawLine(11, 35, 4, 35, cyan);
            DrawLine(66, 35, 59, 35, cyan);
            DrawLine(18, 18, 13, 13, cyan);
            DrawLine(52, 52, 57, 57, cyan);
            DrawLine(52, 18, 57, 13, cyan);
            DrawLine(18, 52, 13, 57, cyan);

        }

        else if (gameState == 1) {

            DrawRectangle(0, 0, 600, 600, background);

            DrawLineEx(
                (Vector2){ cellSize, 0 },
                (Vector2){ cellSize, 600 },
                22,
                (Color){ 232, 60, 255, 25 }
            );

            DrawLineEx(
                (Vector2){ cellSize, 0 },
                (Vector2){ cellSize, 600 },
                14,
                (Color){ 122, 60, 255, 80 }
            );

            DrawLineEx(
                (Vector2){ cellSize, 0 },
                (Vector2){ cellSize, 600 },
                7,
                boardColor
            );

            DrawLineEx(
                (Vector2){ cellSize * 2, 0 },
                (Vector2){ cellSize * 2, 600 },
                22,
                (Color){ 232, 60, 255, 25 }
            );

            DrawLineEx(
                (Vector2){ cellSize * 2, 0 },
                (Vector2){ cellSize * 2, 600 },
                14,
                (Color){ 122, 60, 255, 80 }
            );

            DrawLineEx(
                (Vector2){ cellSize * 2, 0 },
                (Vector2){ cellSize * 2, 600 },
                7,
                boardColor
            );

            DrawLineEx(
                (Vector2){ 0, cellSize },
                (Vector2){ 600, cellSize },
                22,
                (Color){ 232, 60, 255, 25 }
            );

            DrawLineEx(
                (Vector2){ 0, cellSize },
                (Vector2){ 600, cellSize },
                14,
                (Color){ 122, 60, 255, 80 }
            );

            DrawLineEx(
                (Vector2){ 0, cellSize },
                (Vector2){ 600, cellSize },
                7,
                boardColor
            );

            DrawLineEx(
                (Vector2){ 0, cellSize * 2 },
                (Vector2){ 600, cellSize * 2 },
                22,
                (Color){ 232, 60, 255, 25 }
            );

            DrawLineEx(
                (Vector2){ 0, cellSize * 2 },
                (Vector2){ 600, cellSize * 2 },
                14,
                (Color){ 122, 60, 255, 80 }
            );

            DrawLineEx(
                (Vector2){ 0, cellSize * 2 },
                (Vector2){ 600, cellSize * 2 },
                7,
                boardColor
            );

            for (int i = 0; i < 9; i++) {

                int row = i / 3;

                int column = i % 3;

                int centerX = column * cellSize + cellSize / 2;

                int centerY = row * cellSize + cellSize / 2;

                if (board[i] == 'X') {

                    DrawLineEx(
                        (Vector2){
                            centerX - symbolSize,
                            centerY - symbolSize
                        },
                        (Vector2){
                            centerX + symbolSize,
                            centerY + symbolSize
                        },
                        22,
                        (Color){ 0, 229, 255, 25 }
                    );

                    DrawLineEx(
                        (Vector2){
                            centerX + symbolSize,
                            centerY - symbolSize
                        },
                        (Vector2){
                            centerX - symbolSize,
                            centerY + symbolSize
                        },
                        22,
                        (Color){ 0, 229, 255, 25 }
                    );

                    DrawLineEx(
                        (Vector2){
                            centerX - symbolSize,
                            centerY - symbolSize
                        },
                        (Vector2){
                            centerX + symbolSize,
                            centerY + symbolSize
                        },
                        14,
                        (Color){ 0, 229, 255, 80 }
                    );

                    DrawLineEx(
                        (Vector2){
                            centerX + symbolSize,
                            centerY - symbolSize
                        },
                        (Vector2){
                            centerX - symbolSize,
                            centerY + symbolSize
                        },
                        14,
                        (Color){ 0, 229, 255, 80 }
                    );

                    DrawLineEx(
                        (Vector2){
                            centerX - symbolSize,
                            centerY - symbolSize
                        },
                        (Vector2){
                            centerX + symbolSize,
                            centerY + symbolSize
                        },
                        8,
                        cyan
                    );

                    DrawLineEx(
                        (Vector2){
                            centerX + symbolSize,
                            centerY - symbolSize
                        },
                        (Vector2){
                            centerX - symbolSize,
                            centerY + symbolSize
                        },
                        8,
                        cyan
                    );

                }

                if (board[i] == 'O') {

                    float oRadius = symbolSize - 5;

                    DrawCircleLines(
                        centerX,
                        centerY,
                        oRadius + 12,
                        (Color){ 0, 229, 255, 25 }
                    );

                    DrawCircleLines(
                        centerX,
                        centerY,
                        oRadius + 7,
                        (Color){ 0, 229, 255, 80 }
                    );

                    DrawCircleLines(
                        centerX,
                        centerY,
                        oRadius + 4,
                        (Color){ 0, 229, 255, 120 }
                    );

                    DrawCircleLines(
                        centerX,
                        centerY,
                        oRadius + 3,
                        cyan
                    );

                    DrawCircleLines(
                        centerX,
                        centerY,
                        oRadius + 2,
                        cyan
                    );

                    DrawCircleLines(
                        centerX,
                        centerY,
                        oRadius + 1,
                        cyan
                    );

                    DrawCircleLines(
                        centerX,
                        centerY,
                        oRadius,
                        cyan
                    );

                }

            }

        }

        else if (gameState == 2) {

            ClearBackground(background);

            DrawRectangle(
                0,
                0,
                600,
                600,
                (Color){ 5, 7, 22, 245 }
            );

            if (gameWon == 1) {

                Color winColor;

                if (winner == 'X') {

                    winColor = cyan;

                }

                else {

                    winColor = cyan;

                }

                int winWidth = MeasureText("WIN", 100);

                DrawText(
                    "WIN",
                    (600 - winWidth) / 2 + 4,
                    180,
                    100,
                    (Color){ winColor.r, winColor.g, winColor.b, 35 }
                );

                DrawText(
                    "WIN",
                    (600 - winWidth) / 2,
                    180,
                    100,
                    winColor
                );

                char winnerText[20];

                sprintf(winnerText, "Player %c wins!", winner);

                int winnerWidth = MeasureText(winnerText, 40);

                DrawText(
                    winnerText,
                    (600 - winnerWidth) / 2 + 2,
                    300,
                    40,
                    (Color){ 216, 247, 255, 40 }
                );

                DrawText(
                    winnerText,
                    (600 - winnerWidth) / 2,
                    300,
                    40,
                    white
                );

            }

            else {

                int drawWidth = MeasureText("DRAW", 100);

                DrawText(
                    "DRAW",
                    (600 - drawWidth) / 2 + 4,
                    180,
                    100,
                    (Color){ purple.r, purple.g, purple.b, 35 }
                );

                DrawText(
                    "DRAW",
                    (600 - drawWidth) / 2,
                    180,
                    100,
                    purple
                );

                int drawTextWidth = MeasureText("It's a draw!", 40);

                DrawText(
                    "It's a draw!",
                    (600 - drawTextWidth) / 2 + 2,
                    300,
                    40,
                    (Color){ 216, 247, 255, 40 }
                );

                DrawText(
                    "It's a draw!",
                    (600 - drawTextWidth) / 2,
                    300,
                    40,
                    white
                );

            }

            DrawRectangle(
                200,
                390,
                200,
                60,
                buttonColor
            );

            DrawRectangleLinesEx(
                (Rectangle){ 194, 384, 212, 72 },
                3,
                (Color){ 232, 60, 255, 35 }
            );

            DrawRectangleLinesEx(
                (Rectangle){ 198, 388, 204, 64 },
                2,
                pink
            );

            int menuWidth = MeasureText("MAIN MENU", 30);

            DrawText(
                "MAIN MENU",
                200 + (200 - menuWidth) / 2 + 2,
                405,
                30,
                (Color){ 232, 60, 255, 40 }
            );

            DrawText(
                "MAIN MENU",
                200 + (200 - menuWidth) / 2,
                405,
                30,
                white
            );

        }

        else if (gameState == 3) {

            DrawText(
                "OPTIONS",
                220,
                100,
                50,
                cyan
            );

            DrawText(
                "Click the gear to return",
                165,
                200,
                25,
                white
            );

            DrawCircleLines(
                35,
                35,
                22,
                (Color){ 0, 229, 255, 35 }
            );

            DrawCircleLines(
                35,
                35,
                19,
                (Color){ 0, 229, 255, 60 }
            );

            DrawCircleLines(
                35,
                35,
                18,
                cyan
            );

            DrawCircleLines(
                35,
                35,
                7,
                cyan
            );

            DrawLine(35, 11, 35, 4, cyan);
            DrawLine(35, 66, 35, 59, cyan);
            DrawLine(11, 35, 4, 35, cyan);
            DrawLine(66, 35, 59, 35, cyan);
            DrawLine(18, 18, 13, 13, cyan);
            DrawLine(52, 52, 57, 57, cyan);
            DrawLine(52, 18, 57, 13, cyan);
            DrawLine(18, 52, 13, 57, cyan);

        }

        EndDrawing();

    }

    CloseSynthMusic();

    CloseAudioDevice();

    CloseWindow();

    return 0;

}

#include "raylib.h"
#include "menu.h"

void drawMenu(void) {

    static Texture2D background = { 0 };

    if (background.id == 0) {
        background = LoadTexture("background.png");
    }

    DrawTexturePro(
        background,
        (Rectangle){0, 0, background.width, background.height},
        (Rectangle){0, 0, 600, 600},
        (Vector2){0, 0},
        0,
        WHITE
    );
}
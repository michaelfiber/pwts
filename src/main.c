#include "raylib.h"
#include "emscripten/emscripten.h"

void update();

const int ScreenWidth = 1280;
const int ScreenHeight = 720;

Texture2D raylib_logo;
Texture2D ship;

int main(void)
{
    InitWindow(ScreenWidth, ScreenHeight, "Hello World!");

    raylib_logo = LoadTexture("resources/raylib_logo.png");
    ship = LoadTexture("resources/roci.png");

    emscripten_set_main_loop(update, 60, 1);

    UnloadTexture(raylib_logo);

    CloseWindow();
}

void update()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Hello World!", 100, 100, GetFontDefault().baseSize * 4, WHITE);
    DrawTexture(ship, 100, 200, WHITE);
    EndDrawing();
}

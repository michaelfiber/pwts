#include <math.h>
#include "raylib.h"
#include "emscripten/emscripten.h"
#include "missiles.h"
#include "bullets.h"
#include "particles.h"
#include "asteroid.h"
#include "collider.h"
#include "explosion.h"
#include "starfield.h"
#include "title.h"
#include "game.h"
#include "briefing.h"

void update();

int state = 0;

const int ScreenWidth = 1280;
const int ScreenHeight = 720;

Shader glow_shader;

float screen_countdown;

char victory_text[] = "Congratulations! You have defeated the enemy!";
char defeat_text[] = "You lose. Try again!";

bool draw_victory()
{
    screen_countdown -= GetFrameTime();

    int size = GetFontDefault().baseSize * 4;
    int width = MeasureText(victory_text, size);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(victory_text, GetScreenWidth() / 2 - width / 2, GetScreenHeight() / 2 - 20, size, GREEN);
    EndDrawing();

    return screen_countdown > 0;
}

bool draw_defeat()
{
    screen_countdown -= GetFrameTime();

    int size = GetFontDefault().baseSize * 4;
    int width = MeasureText(defeat_text, size);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(defeat_text, GetScreenWidth() / 2 - width / 2, GetScreenHeight() / 2 - 20, size, RED);
    EndDrawing();

    return screen_countdown > 0;
}

int main(void)
{
    InitWindow(ScreenWidth, ScreenHeight, "Hello World!");

    glow_shader = LoadShader(0, "resources/shader.fs");
    init_missiles();
    init_title();

    HideCursor();

    // start the main loop wiuth emscripten.
    emscripten_set_main_loop(update, 0, 1);

    CloseWindow();
}

void update()
{
    if (state == 0) // title screen
    {
        if (draw_title(glow_shader))
        {
            return;
        }
        init_game();
        state = 1;
    }

    else if (state == 1)
    {
        if (draw_briefing())
        {
            return;
        }
        state = 2;
    }

    else if (state == 2)
    {
        if (draw_game(glow_shader))
        {
            return;
        }

        screen_countdown = 10.0f;

        if (is_victory)
        {
            state = 3;
            return;
        }
        state = 4;
    }
    else if (state == 3)
    {
        // show victory screen
        if (draw_victory())
        {
            return;
        }
        init_title();
        state = 0;
    }
    else if (state == 4)
    {
        // show defeat screen
        if (draw_defeat())
        {
            return;
        }
        init_title();
        state = 0;
    }
}

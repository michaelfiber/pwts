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

void update();

int state = 0;

const int ScreenWidth = 1280;
const int ScreenHeight = 720;

Shader glow_shader;


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
        if (draw_game(glow_shader))
        {
            return;
        }

        state = 2;
    }
}

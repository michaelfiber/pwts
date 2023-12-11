#include <math.h>
#include "raylib.h"
#include "emscripten/emscripten.h"
#include "missiles.h"
#include "bullets.h"

void update();

const int ScreenWidth = 1280;
const int ScreenHeight = 720;

Camera2D camera;

Ship player = {0};

int main(void)
{
    InitWindow(ScreenWidth, ScreenHeight, "Hello World!");

    init_missiles();

    HideCursor();

    player.tex = LoadTexture("resources/ship.png");
    player.thruster_power = 150.0f;
    player.engine_power = 150.0f;
    player.bullet_speed = 1500.0f;

    camera.offset.x = ScreenWidth / 2;
    camera.offset.y = ScreenHeight / 2;
    camera.zoom = 1.0f;

    emscripten_set_main_loop(update, 60, 1);

    CloseWindow();
}

void update()
{
    if (IsKeyDown(KEY_D))
    {
        player.delta_rot += player.thruster_power * GetFrameTime();
    }

    if (IsKeyDown(KEY_A))
    {
        player.delta_rot -= player.thruster_power * GetFrameTime();
    }

    if (IsKeyDown(KEY_W))
    {
        player.vel.x += cosf(player.rot * DEG2RAD) * player.engine_power * GetFrameTime();
        player.vel.y += sinf(player.rot * DEG2RAD) * player.engine_power * GetFrameTime();
    }

    Vector2 mouse_pos = GetMousePosition();
    Vector2 targeter = (Vector2){
        (mouse_pos.x - camera.offset.x) / camera.zoom,
        (mouse_pos.y - camera.offset.y) / camera.zoom};

    player.loc.x += player.vel.x * GetFrameTime();
    player.loc.y += player.vel.y * GetFrameTime();
    player.rot += player.delta_rot * GetFrameTime();

    if (player.rot > 360)
    {
        player.rot -= 360;
    }
    if (player.rot < -360)
    {
        player.rot += 360;
    }

    float mouseWheel = GetMouseWheelMove();
    if (fabsf(mouseWheel) > 0.1f)
    {
        camera.zoom += mouseWheel * GetFrameTime();
    }

    if (camera.zoom < 0.1f)
    {
        camera.zoom = 0.1f;
    }

    update_missiles(targeter, player);
    update_bullets(targeter, player);

    BeginDrawing();
    {
        ClearBackground(BLACK);

        BeginMode2D(camera);
        {
            DrawTexturePro(player.tex, (Rectangle){0, 0, player.tex.width, player.tex.height}, (Rectangle){player.loc.x, player.loc.y, player.tex.width, player.tex.height}, (Vector2){player.tex.width / 2, player.tex.height / 2}, player.rot, WHITE);

            draw_missiles(camera);
            draw_bullets(camera);

            DrawCircleLines(targeter.x, targeter.y, 5 / camera.zoom, GREEN);
        }
        EndMode2D();

        DrawText(TextFormat("%f\n%f", player.rot, camera.zoom), 20, 40, GetFontDefault().baseSize * 2, RED);

        DrawFPS(20, 20);
    }
    EndDrawing();
}

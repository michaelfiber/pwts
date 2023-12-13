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

float railgun_cooldown = 0.0f;
typedef struct
{
    float alpha;
    Vector2 start;
    Vector2 end;
} Beam;

Beam beam;

Shader shader;

RenderTexture2D render_texture;

int main(void)
{
    InitWindow(ScreenWidth, ScreenHeight, "Hello World!");
    render_texture = LoadRenderTexture(ScreenWidth, ScreenHeight);

    shader = LoadShader(0, "resources/shader.fs");

    init_missiles();

    HideCursor();

    player.tex = LoadTexture("resources/ship.png");
    player.thruster_power = 150.0f;
    player.engine_power = 150.0f;
    player.bullet_speed = 1500.0f;

    camera.offset.x = ScreenWidth / 2;
    camera.offset.y = ScreenHeight / 2;
    camera.zoom = 1.0f;

    emscripten_set_main_loop(update, 0, 1);

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

    if (railgun_cooldown > 0.0f)
    {
        railgun_cooldown -= GetFrameTime();
    }

    if (IsKeyPressed(KEY_SPACE) && railgun_cooldown <= 0.0f)
    {
        player.vel.x += cosf((player.rot + 180) * DEG2RAD) * 5000 * GetFrameTime();
        player.vel.y += sinf((player.rot + 180) * DEG2RAD) * 5000 * GetFrameTime();
        beam.start.x = player.loc.x;
        beam.start.y = player.loc.y;
        beam.end.x = beam.start.x + cosf(player.rot * DEG2RAD) * 5000;
        beam.end.y = beam.start.y + sinf(player.rot * DEG2RAD) * 5000;
        beam.alpha = 1.0f;
    }

    if (beam.alpha > 0.0f)
    {
        beam.alpha -= GetFrameTime();
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

    BeginTextureMode(render_texture);
    {
        ClearBackground(BLACK);

        BeginMode2D(camera);
        {
            if (beam.alpha > 0.0f)
            {
                DrawLineEx(beam.start, beam.end, (2 - beam.alpha) * 2, Fade(WHITE, beam.alpha));
            }

            draw_missiles(camera);
            draw_bullets(camera);

            DrawCircleLines(targeter.x, targeter.y, 5 / camera.zoom, GREEN);
        }
        EndMode2D();
    }
    EndTextureMode();

    BeginDrawing();
    {
        BeginShaderMode(shader);
        {
            DrawTextureRec(render_texture.texture, (Rectangle){0, 0, ScreenWidth, -ScreenHeight}, (Vector2){0, 0}, WHITE);
        }
        EndShaderMode();

        BeginMode2D(camera);
        {
            DrawTexturePro(player.tex, (Rectangle){0, 0, player.tex.width, player.tex.height}, (Rectangle){player.loc.x, player.loc.y, player.tex.width, player.tex.height}, (Vector2){player.tex.width / 2, player.tex.height / 2}, player.rot, WHITE);
        }
        EndMode2D();

        DrawText(TextFormat("%f\n%f", player.rot, camera.zoom), 20, 40, GetFontDefault().baseSize * 2, RED);

        DrawFPS(20, 20);        
    }
    EndDrawing();
}

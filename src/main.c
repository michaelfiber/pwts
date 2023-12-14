#include <math.h>
#include "raylib.h"
#include "emscripten/emscripten.h"
#include "missiles.h"
#include "bullets.h"
#include "particles.h"

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
RenderTexture2D starfield_texture;

#define STARS_MAX 8000

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
    player.bullet_speed = 800.0f;

    player.em_engine.width = 5.0f;
    player.em_engine.color = (Color){ 150, 200, 255, 255 };
    player.em_engine.rate = 30.0f;
    player.em_engine.life = 2.5f;
    player.em_engine.thrust = 100.0f;
    player.em_engine.size = 5.0f;
    
    player.em_bow_p.width = 2.0f;
    player.em_bow_p.color = (Color){ 200, 100, 100, 255 };
    player.em_bow_p.rate = 15.0f;
    player.em_bow_p.life = 1.5f;
    player.em_bow_p.thrust = 50.0f;
    player.em_bow_p.size = 2.0f;

    player.em_bow_s.width = 2.0f;
    player.em_bow_s.color = (Color){ 200, 100, 100, 255 };
    player.em_bow_s.rate = 15.0f;
    player.em_bow_s.life = 1.5f;
    player.em_bow_s.thrust = 50.0f;
    player.em_bow_s.size = 2.0f;

    camera.offset.x = ScreenWidth / 2;
    camera.offset.y = ScreenHeight / 2;
    camera.zoom = 1.0f;

    // generate the starfield
    starfield_texture = LoadRenderTexture(10000, 10000);

    BeginTextureMode(starfield_texture);
    {
        ClearBackground(BLACK);

        Color star_color = (Color){150, 200, 255, 200};

        for (int i = 0; i < STARS_MAX; i++)
        {
            float x = GetRandomValue(0, 10000);
            float y = GetRandomValue(0, 10000);
            float z = GetRandomValue(1, 20) / 10.0f;
            DrawCircle(x, y, z, star_color);
        }
    }
    EndTextureMode();

    // start the main loop wiuth emscripten.
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

    player.em_engine.on = IsKeyDown(KEY_W);
    player.em_bow_p.on = IsKeyDown(KEY_D);
    player.em_bow_s.on = IsKeyDown(KEY_A);

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
        ((mouse_pos.x - camera.offset.x) / camera.zoom) + player.loc.x,
        ((mouse_pos.y - camera.offset.y) / camera.zoom) + player.loc.y};

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

    // float mouseWheel = GetMouseWheelMove();
    // if (fabsf(mouseWheel) > 0.1f)
    // {
    //     camera.zoom += mouseWheel * GetFrameTime();
    // }

    // if (camera.zoom < 0.1f)
    // {
    //     camera.zoom = 0.1f;
    // }

    camera.target.x = player.loc.x;
    camera.target.y = player.loc.y;

    update_missiles(targeter, player);
    update_bullets(targeter, player);
    update_ship(&player);

    BeginTextureMode(render_texture);
    {
        ClearBackground(BLACK);

        BeginMode2D(camera);
        {
            DrawTexturePro(starfield_texture.texture, (Rectangle){ 0, 0, 10000, -10000}, (Rectangle){ 0, 0, 10000, 10000}, (Vector2){ 5000, 5000 }, 0.0f, WHITE);
            DrawRectangleLines(-5000, -5000, 10000, 10000, GREEN);

            if (beam.alpha > 0.0f)
            {
                DrawLineEx(beam.start, beam.end, (2 - beam.alpha) * 2, Fade(WHITE, beam.alpha));
            }

            draw_missiles(camera);
            draw_bullets(camera);
            
            draw_emitter(&player.em_engine);
            draw_emitter(&player.em_bow_p);
            draw_emitter(&player.em_bow_s);

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

#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "game.h"
#include "starfield.h"
#include "missiles.h"
#include "bullets.h"
#include "explosion.h"
#include "asteroid.h"
#include "enemy.h"
#include "my-library.h"
#include "puff.h"
#include "sounds.h"

bool is_victory;

float round_end_timer;
bool show_colliders = false;
int level = 1;

Ship player = {0};
typedef struct
{
    float alpha;
    Vector2 start;
    Vector2 end;
} Beam;

Beam beam;

RenderTexture2D render_texture;
Camera2D camera;

int on_segment(Vector2 p, Vector2 q, Vector2 r)
{
    if (q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&
        q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y))
        return 1;
    return 0;
}

int orientation(Vector2 p, Vector2 q, Vector2 r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0;             // collinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

int do_intersect(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return 1; // They intersect

    if (o1 == 0 && on_segment(p1, p2, q1))
        return 1; // p1 , q1 and p2 are collinear and p2 lies on segment p1q1
    if (o2 == 0 && on_segment(p1, q2, q1))
        return 1; // p1 , q1 and q2 are collinear and q2 lies on segment p1q1
    if (o3 == 0 && on_segment(p2, p1, q2))
        return 1; // p2 , q2 and p1 are collinear and p1 lies on segment p2q2
    if (o4 == 0 && on_segment(p2, q1, q2))
        return 1; // p2 , q2 and q1 are collinear and q1 lies on segment p2q2

    return 0; // Doesn't intersect
}

int line_intersects_rectangle(Vector2 lineStart, Vector2 lineEnd, Rectangle rect)
{
    Vector2 topLeft = (Vector2){rect.x, rect.y};
    Vector2 bottomRight = (Vector2){rect.x + rect.width, rect.y + rect.height};

    Vector2 topRight = {bottomRight.x, topLeft.y};
    Vector2 bottomLeft = {topLeft.x, bottomRight.y};

    // Check if the line intersects any of the rectangle edges
    if (do_intersect(lineStart, lineEnd, topLeft, topRight) ||
        do_intersect(lineStart, lineEnd, topRight, bottomRight) ||
        do_intersect(lineStart, lineEnd, bottomRight, bottomLeft) ||
        do_intersect(lineStart, lineEnd, bottomLeft, topLeft))
    {
        return 1; // Intersects
    }

    return 0; // Doesn't intersect
}

void add_enemies(int count)
{
    for (int i = 0; i < count; i++)
    {
        float angle = GetRandomValue(0, 359) * DEG2RAD;
        float distance = GetRandomValue(900, MAP_HEIGHT / 2) * 0.9f;

        add_enemy((Vector2){cosf(angle) * distance, sinf(angle) * distance}, 100.0f, LoadTexture("resources/ship.png"));
    }
}

void init_game()
{
    int a = 0;
    int e = 0;

    for (int i = 0; i < level; i++) {
        a += 50;
        e += 4;
    }

    round_end_timer = 5.0f;

    init_asteroid(a);
    add_enemies(e);

    camera.offset.x = GetScreenWidth() / 2;
    camera.offset.y = GetScreenHeight() / 2;
    camera.zoom = 1.0f;

    if (render_texture.id == 0)
    {
        render_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    if (player.tex.id == 0)
    {
        player.tex = LoadTexture("resources/ship.png");
    }

    player.thruster_power = 150.0f + level * 15;
    player.engine_power = 50.0f + level * 5;
    player.bullet_speed = 800.0f;
    
    player.vel.x = 0;
    player.vel.y = 0;
    player.loc.x = 0;
    player.loc.y = 0;
    player.rot = 0;
    player.delta_rot = 0;

    player.emitters[0].em.width = 5.0f;
    player.emitters[0].em.color = (Color){150, 200, 255, 255};
    player.emitters[0].em.rate = 30.0f;
    player.emitters[0].em.life = 2.5f;
    player.emitters[0].em.thrust = 100.0f;
    player.emitters[0].em.size = 5.0f;
    player.emitters[0].distance = -10;
    player.emitters[0].offset = 180;

    player.emitters[1].em.width = 2.0f;
    player.emitters[1].em.color = (Color){255, 200, 200, 255};
    player.emitters[1].em.rate = 15.0f;
    player.emitters[1].em.life = 1.5f;
    player.emitters[1].em.thrust = 50.0f;
    player.emitters[1].em.size = 2.0f;
    player.emitters[1].distance = 15;
    player.emitters[1].offset = -90;

    player.emitters[2].em.width = 2.0f;
    player.emitters[2].em.color = (Color){255, 200, 200, 255};
    player.emitters[2].em.rate = 15.0f;
    player.emitters[2].em.life = 1.5f;
    player.emitters[2].em.thrust = 50.0f;
    player.emitters[2].em.size = 2.0f;
    player.emitters[2].distance = 15;
    player.emitters[2].offset = 90;

    init_ship(&player);

    Say("destroy asteroids and avoid missiles to survive");
}

void draw_radar()
{
    static int width = MAP_WIDTH / 25;
    static int height = MAP_HEIGHT / 25;

    DrawRectangleLines(GetScreenWidth() - width - 10, GetScreenHeight() - height - 10, width, height, RED);

    for (int i = 0; i < ASTEROID_MAX; i++)
    {
        if (asteroids[i].life > 0.0f)
        {
            DrawCircle(
                GetScreenWidth() - width - 10 + width / 2 + (asteroids[i].loc.dest.x + asteroids[i].loc.dest.width / 2) / 25,
                GetScreenHeight() - height - 10 + height / 2 + (asteroids[i].loc.dest.y + asteroids[i].loc.dest.height / 2) / 25,
                4.0f,
                GRAY);
        }
    }

    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemies[i].life > 0.0f)
        {
            DrawCircle(
                GetScreenWidth() - width - 10 + width / 2 + (enemies[i].loc.dest.x) / 25,
                GetScreenHeight() - height - 10 + height / 2 + (enemies[i].loc.dest.y) / 25,
                4.0f,
                RED);
        }
    }

    DrawCircle(
        GetScreenWidth() - width - 10 + width / 2 + player.loc.x / 25,
        GetScreenHeight() - height - 10 + height / 2 + player.loc.y / 25,
        4.0f,
        GREEN);

    int ui_size = GetFontDefault().baseSize * 2;

    DrawText("HEALTH", GetScreenWidth() - 10 - MeasureText("HEALTH", ui_size), 420, ui_size, WHITE);
    float health = player.life / 1000.0f * width;
    DrawRectangle(GetScreenWidth() - 10 - width, 440, health, 20, GREEN);
    DrawRectangleLines(GetScreenWidth() - 10 - width, 440, width, 20, WHITE);

    if (player.broken_gun_timer > 0.0f)
    {
        DrawText("PDC DAMAGED", GetScreenWidth() / 2 - MeasureText("PDC DAMAGED", ui_size) / 2, GetScreenHeight() / 2 + 100, ui_size, RED);
        float level = player.broken_gun_timer;
        DrawRectangle(
            GetScreenWidth() / 2 - 50,
            GetScreenHeight() / 2 + 120,
            10 * level,
            10, RED);
    }

    if (player.broken_railgun_timer > 0.0f)
    {
        DrawText("RAIL GUN DAMAGED", GetScreenWidth() / 2 - MeasureText("RAIL GUN DAMAGED", ui_size) / 2, GetScreenHeight() / 2 + 120, ui_size, RED);
        float level = player.broken_railgun_timer;
        DrawRectangle(
            GetScreenWidth() / 2 - 50,
            GetScreenHeight() / 2 + 140,
            10 * level,
            10, RED);
    }

    if (player.broken_missile_timer > 0.0f)
    {
        DrawText("MISSILE LAUNCHER DAMAGED", GetScreenWidth() / 2 - MeasureText("MISSILE LAUNCHER DAMAGED", ui_size) / 2, GetScreenHeight() / 2 + 140, ui_size, RED);
        float level = player.broken_missile_timer;
        DrawRectangle(
            GetScreenWidth() / 2 - 50,
            GetScreenHeight() / 2 + 160,
            10 * level,
            10, RED);
    }
}

bool draw_game(Shader glow_shader)
{
    if (player.life <= 0.0f)
    {
        is_victory = false;
        level = 1;
        return false;
    }

    if (enemy_count == 0)
    {
        round_end_timer -= GetFrameTime();
    }

    if (round_end_timer <= 0.0f)
    {
        level++;
        if (level == 6)
        {
            is_victory = true;
            level = 1;
            return false;
        }
        init_game();
    }

    if (IsKeyPressed(KEY_C))
    {
        show_colliders = !show_colliders;
    }

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
        play_sound(SOUND_ENGINE);
    }

    player.emitters[0].em.on = IsKeyDown(KEY_W);
    player.emitters[1].em.on = IsKeyDown(KEY_D);
    player.emitters[2].em.on = IsKeyDown(KEY_A);

    if (player.railgun_cooldown > 0.0f)
    {
        player.railgun_cooldown -= GetFrameTime();
    }

    if (IsKeyPressed(KEY_SPACE) && player.railgun_cooldown <= 0.0f && player.broken_railgun_timer <= 0.0f)
    {
        play_sound(SOUND_RAIL);
        player.vel.x += cosf((player.rot + 180) * DEG2RAD) * 5000 * GetFrameTime();
        player.vel.y += sinf((player.rot + 180) * DEG2RAD) * 5000 * GetFrameTime();
        beam.start.x = player.loc.x;
        beam.start.y = player.loc.y;
        beam.end.x = beam.start.x + cosf(player.rot * DEG2RAD) * 5000;
        beam.end.y = beam.start.y + sinf(player.rot * DEG2RAD) * 5000;
        for (int i = 0; i < ASTEROID_MAX; i++)
        {
            if (!asteroids[i].invuln && asteroids[i].life > 0 && line_intersects_rectangle(beam.start, beam.end, asteroids[i].loc.dest))
            {
                destroy_asteroid(&asteroids[i]);
            }
        }
        beam.alpha = 1.0f;
    }

    if (beam.alpha > 0.0f)
    {
        beam.alpha -= GetFrameTime();
    }

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

    if (camera.target.x < MAP_WIDTH / -2 + GetScreenWidth() / 2)
    {
        camera.target.x = MAP_WIDTH / -2 + GetScreenWidth() / 2;
    }
    else if (camera.target.x > MAP_WIDTH / 2 - GetScreenWidth() / 2)
    {
        camera.target.x = MAP_WIDTH / 2 - GetScreenWidth() / 2;
    }

    if (camera.target.y < MAP_HEIGHT / -2 + GetScreenHeight() / 2)
    {
        camera.target.y = MAP_HEIGHT / -2 + GetScreenHeight() / 2;
    }
    else if (camera.target.y > MAP_HEIGHT / 2 - GetScreenHeight() / 2)
    {
        camera.target.y = MAP_HEIGHT / 2 - GetScreenHeight() / 2;
    }

    if (player.shake_timer > 0.0f)
    {
        camera.target.x += cosf(player.shake_timer * 50) * 3;
        camera.target.y += sinf(player.shake_timer * 50) * 3;
    }

    Vector2 mouse_pos = GetMousePosition();
    Vector2 targeter = (Vector2){
        mouse_pos.x + camera.target.x - GetScreenWidth() / 2,
        mouse_pos.y + camera.target.y - GetScreenHeight() / 2};

    check_colliders();

    update_missiles(targeter, player);
    update_bullets(targeter, player);
    update_ship(&player);
    update_asteroids();
    update_explosions();
    update_enemies();
    update_puffs();

    RenderTexture2D starfield_texture = get_starfield();

    BeginTextureMode(render_texture);
    {
        ClearBackground(BLACK);

        float zoom = camera.zoom;
        camera.zoom = zoom * 0.6f;
        BeginMode2D(camera);
        {
            DrawTexturePro(starfield_texture.texture, (Rectangle){0, 0, 10000, -10000}, (Rectangle){0, 0, 10000, 10000}, (Vector2){5000, 5000}, 0.0f, WHITE);
        }
        EndMode2D();

        camera.zoom = zoom;
        BeginMode2D(camera);
        {
            DrawRectangleLines(-5000, -3000, 10000, 6000, GREEN);

            if (beam.alpha > 0.0f)
            {
                DrawLineEx(beam.start, beam.end, (2 - beam.alpha) * 2, Fade(WHITE, beam.alpha));
            }

            draw_missiles(camera);
            draw_bullets(camera);
            draw_asteroid();
            draw_puffs();
            draw_explosions();

            for (int i = 0; i < EMITTERS_MAX; i++)
            {
                draw_emitter(&player.emitters[i].em);
            }

            draw_enemies();

            DrawTexturePro(player.tex, (Rectangle){0, 0, player.tex.width, player.tex.height}, (Rectangle){player.loc.x, player.loc.y, player.tex.width, player.tex.height}, (Vector2){player.tex.width / 2, player.tex.height / 2}, player.rot, WHITE);
            // DrawCircleLines((int)player.loc.x, (int)player.loc.y, 20, WHITE);

            DrawCircleLines(targeter.x, targeter.y, 5 / camera.zoom, GREEN);

            if (show_colliders)
            {
                draw_colliders();
            }
        }
        EndMode2D();
    }
    EndTextureMode();

    BeginDrawing();
    {
        BeginShaderMode(glow_shader);
        {
            DrawTextureRec(render_texture.texture, (Rectangle){0, 0, GetScreenWidth(), -GetScreenHeight()}, (Vector2){0, 0}, WHITE);
        }
        EndShaderMode();

        draw_radar();

        // DrawText(TextFormat("%f\n%f\n%d %d\n", player.rot, camera.zoom, (int)player.loc.x, (int)player.loc.y), 20, 40, GetFontDefault().baseSize * 2, RED);
        // DrawFPS(20, 20);

        DrawText(TextFormat("Remaining Enemies: %d", enemy_count), 10, 10, GetFontDefault().baseSize * 2, GREEN);


        if (enemy_count == 0)
        {
            char text[] = "LEVEL COMPLETE!";
            int text_width = MeasureText(text, GetFontDefault().baseSize * 4);
            DrawText(text, GetScreenWidth() / 2 - text_width / 2, GetScreenHeight() / 2 - 20, GetFontDefault().baseSize * 4, GREEN);
        }
    }
    EndDrawing();

    return true;
}
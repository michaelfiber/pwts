#include <math.h>
#include <raylib.h>
#include "missiles.h"
#include "ship.h"

#define EXPLOSION_MAX 100

typedef struct
{
    Vector2 pos;
    float radius;
    float alpha;
} Explosion;

Target targets[TARGET_MAX];
Vector2 targeter;
Explosion explosions[EXPLOSION_MAX];
Texture2D missile_tex;

void add_explosion(Vector2 pos, float radius)
{
    for (int i = 0; i < EXPLOSION_MAX; i++)
    {
        if (explosions[i].alpha > 0.0f)
        {
            continue;
        }

        explosions[i].alpha = 1.0f;
        explosions[i].pos.x = pos.x;
        explosions[i].pos.y = pos.y;
        explosions[i].radius = radius;
        break;
    }
}

void update_explosions()
{
    for (int i = 0; i < EXPLOSION_MAX; i++)
    {
        if (explosions[i].alpha > 0.0f)
        {
            explosions[i].alpha -= GetFrameTime();

            if (explosions[i].alpha < 0.0f)
            {
                explosions[i].alpha = 0.0f;
            }
        }
    }
}

void init_missiles()
{
    missile_tex = LoadTexture("resources/missile.png");
}

void update_missiles(Vector2 targeter, Ship player)
{
    if (IsMouseButtonPressed(1))
    {
        for (int i = 0; i < TARGET_MAX; i++)
        {
            if (!targets[i].is_active)
            {
                targets[i].is_active = true;
                targets[i].pos.x = targeter.x;
                targets[i].pos.y = targeter.y;
                targets[i].missile.pos.x = player.loc.x;
                targets[i].missile.pos.y = player.loc.y;
                targets[i].missile.vel.x = player.vel.x + 50 * cosf((player.rot + 90) * DEG2RAD);
                targets[i].missile.vel.y = player.vel.y + 50 * sinf((player.rot + 90) * DEG2RAD);
                targets[i].missile.state = MISSILE_FLOAT;
                targets[i].missile.fuse = 0.5f;
                targets[i].missile.rot = player.rot;
                break;
            }
        }
    }

    for (int i = 0; i < TARGET_MAX; i++)
    {
        if (!targets[i].is_active)
        {
            continue;
        }

        switch (targets[i].missile.state)
        {
        case MISSILE_GO:
            // check for collision
            if (CheckCollisionCircles(targets[i].pos, 100, targets[i].missile.pos, 10))
            {
                targets[i].is_active = false;
                add_explosion(targets[i].pos, 100.0f);
            }
            break;
        case MISSILE_FLOAT:
            targets[i].missile.fuse -= GetFrameTime();
            if (targets[i].missile.fuse <= 0.0f)
            {
                targets[i].missile.state = MISSILE_GO;
                targets[i].missile.rot = atan2f(targets[i].missile.pos.y - targets[i].pos.y, targets[i].missile.pos.x - targets[i].pos.x) * RAD2DEG;
                targets[i].missile.vel.x = cosf(targets[i].missile.rot * DEG2RAD) * 1000;
                targets[i].missile.vel.y = sinf(targets[i].missile.rot * DEG2RAD) * 1000;
            }
            break;
        }

        targets[i].missile.pos.x -= targets[i].missile.vel.x * GetFrameTime();
        targets[i].missile.pos.y -= targets[i].missile.vel.y * GetFrameTime();
    }

    update_explosions();
}

void draw_missiles(Camera2D camera)
{
    // Draw the missiles.
    for (int i = 0; i < TARGET_MAX; i++)
    {
        if (!targets[i].is_active)
        {
            continue;
        }

        DrawTexturePro(missile_tex, (Rectangle){0, 0, missile_tex.width, missile_tex.height}, (Rectangle){targets[i].missile.pos.x, targets[i].missile.pos.y, missile_tex.width, missile_tex.height}, (Vector2){missile_tex.width / 2, missile_tex.height / 2}, targets[i].missile.rot, WHITE);

        DrawCircleLines(targets[i].pos.x, targets[i].pos.y, 5 / camera.zoom, RED);
    }

    // Draw the explosions.
    for (int i = 0; i < EXPLOSION_MAX; i++)
    {
        if (explosions[i].alpha > 0.0f)
        {
            Color c = Fade(WHITE, explosions[i].alpha);
            DrawCircle(explosions[i].pos.x, explosions[i].pos.y, explosions[i].radius, c);
        }
    }
}
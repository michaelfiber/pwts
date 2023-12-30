#include <math.h>
#include <raylib.h>
#include "missiles.h"
#include "ship.h"
#include "explosion.h"

Target targets[TARGET_MAX];
Vector2 targeter;
Texture2D missile_tex;

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
            if (CheckCollisionCircles(targets[i].pos, 10, targets[i].missile.pos, 10) || targets[i].missile.loc.is_hitting_type == LOC_TYPE_ASTEROID || targets[i].missile.loc.is_hitting_type == LOC_TYPE_BULLET)
            {
                targets[i].is_active = false;
                add_explosion(targets[i].missile.pos, 50.0f, 50);
                remove_collider(&targets[i].missile.loc);
            }
            break;
        case MISSILE_FLOAT:
            targets[i].missile.fuse -= GetFrameTime();
            if (targets[i].missile.fuse <= 0.0f)
            {
                targets[i].missile.state = MISSILE_GO;
                targets[i].missile.rot = atan2f(targets[i].missile.pos.y - targets[i].pos.y, targets[i].missile.pos.x - targets[i].pos.x) * RAD2DEG;
                targets[i].missile.vel.x = cosf(targets[i].missile.rot * DEG2RAD) * 400;
                targets[i].missile.vel.y = sinf(targets[i].missile.rot * DEG2RAD) * 400;
                add_collider(&targets[i].missile.loc, LOC_TYPE_MISSILE);
            }
            break;
        }

        targets[i].missile.pos.x -= targets[i].missile.vel.x * GetFrameTime();
        targets[i].missile.pos.y -= targets[i].missile.vel.y * GetFrameTime();
        targets[i].missile.loc.dest.x = targets[i].missile.pos.x - 5;
        targets[i].missile.loc.dest.y = targets[i].missile.pos.y - 5;
        targets[i].missile.loc.dest.width = 10;
        targets[i].missile.loc.dest.height = 10;
    }
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
}
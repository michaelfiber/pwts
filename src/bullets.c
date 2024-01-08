#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include "bullets.h"
#include "ship.h"
#include "explosion.h"
#include "sounds.h"

#define BULLET_MAX 32000
Bullet bullets[BULLET_MAX];

float gun_cooldown;
int bullet_count = 0;

void add_bullet(Vector2 vel, Vector2 pos, float power, float life)
{
    int next_bullet = 0;

    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (bullets[i].life <= 0.0f)
        {
            next_bullet = i;
            break;
        }
    }

    bullets[next_bullet].life = life;
    bullets[next_bullet].power = power;
    bullets[next_bullet].vel.x = vel.x;
    bullets[next_bullet].vel.y = vel.y;
    bullets[next_bullet].loc.dest.x = pos.x - 1;
    bullets[next_bullet].loc.dest.y = pos.y - 1;
    bullets[next_bullet].loc.dest.width = 2;
    bullets[next_bullet].loc.dest.height = 2;

    add_collider(&bullets[next_bullet].loc, LOC_TYPE_BULLET);
}

void update_bullets(Vector2 targeter, Ship player)
{
    gun_cooldown -= GetFrameTime();

    float gun_normal = atan2f(targeter.y - player.loc.y, targeter.x - player.loc.x);

    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (bullets[i].life <= 0.0f)
        {
            continue;
        }

        bullets[i].loc.dest.x += bullets[i].vel.x * GetFrameTime();
        bullets[i].loc.dest.y += bullets[i].vel.y * GetFrameTime();
        bullets[i].life -= GetFrameTime();

        switch (bullets[i].loc.is_hitting_type)
        {
        case LOC_TYPE_ENEMY:
        case LOC_TYPE_ASTEROID:
        case LOC_TYPE_MISSILE:
            add_explosion((Vector2){bullets[i].loc.dest.x + bullets[i].vel.x * GetFrameTime(), bullets[i].loc.dest.y + bullets[i].vel.y * GetFrameTime()}, 5.0f, 10);
            bullets[i].life = -1.0f;
            break;
        }

        if (bullets[i].life <= 0.0f)
        {
            remove_collider(&bullets[i].loc);
        }
        bullet_count++;
    }

    if (IsMouseButtonDown(0) && gun_cooldown <= 0.0f && player.broken_gun_timer <= 0.0f)
    {
        play_sound(SOUND_PDC);

        gun_cooldown = 0.05f;

        float offset = gun_normal - player.rot * DEG2RAD;

        if ((offset > 0 && offset < PI) || offset < -PI)
        {
            add_bullet(
                (Vector2){
                    player.vel.x + player.bullet_speed * cosf(gun_normal),
                    player.vel.y + player.bullet_speed * sinf(gun_normal)},
                (Vector2){
                    player.loc.x + 2 * cosf((player.rot + 90) * DEG2RAD),
                    player.loc.y + 2 * sinf((player.rot + 90) * DEG2RAD)},
                1.0f,
                3.0f);
        }

        if ((offset > -PI && offset < 0) || offset > PI)
        {
            add_bullet(
                (Vector2){
                    player.vel.x + player.bullet_speed * cosf(gun_normal),
                    player.vel.y + player.bullet_speed * sinf(gun_normal)},
                (Vector2){
                    player.loc.x + 4 * cosf((player.rot - 90) * DEG2RAD),
                    player.loc.y + 4 * sinf((player.rot - 90) * DEG2RAD)},
                1.0f,
                3.0f);
        }
    }
}

void draw_bullets(Camera2D camera)
{
    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (bullets[i].life > 0.0f)
        {
            DrawRectangleRec(bullets[i].loc.dest, WHITE);
        }
    }
}
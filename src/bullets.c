#include <math.h>
#include <raylib.h>
#include "bullets.h"
#include "ship.h"

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
    bullets[next_bullet].pos.x = pos.x;
    bullets[next_bullet].pos.y = pos.y;
}

void update_bullets(Vector2 targeter, Ship player)
{
    gun_cooldown -= GetFrameTime();

    float gun_normal = atan2f(targeter.y - player.loc.y, targeter.x - player.loc.x);

    if (IsMouseButtonDown(0) && gun_cooldown <= 0.0f)
    {
        gun_cooldown = 0.05f;

        float offset = gun_normal - player.rot * DEG2RAD;

        if ((offset > 0 && offset < PI) || offset < -PI)
        {
            add_bullet(
                (Vector2){
                    player.vel.x + player.bullet_speed * cosf(gun_normal),
                    player.vel.y + player.bullet_speed * sinf(gun_normal)},
                (Vector2){
                    player.loc.x + 25 * cosf((player.rot + 90) * DEG2RAD),
                    player.loc.y + 25 * sinf((player.rot + 90) * DEG2RAD)},
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
                    player.loc.x + 25 * cosf((player.rot - 90) * DEG2RAD),
                    player.loc.y + 25 * sinf((player.rot - 90) * DEG2RAD)},
                1.0f,
                3.0f);
        }
    }

    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (bullets[i].life <= 0.0f)
        {
            continue;
        }

        bullets[i].pos.x += bullets[i].vel.x * GetFrameTime();
        bullets[i].pos.y += bullets[i].vel.y * GetFrameTime();
        bullets[i].life -= GetFrameTime();
        bullet_count++;
    }
}

void draw_bullets(Camera2D camera)
{
    for (int i = 0; i < BULLET_MAX; i++)
    {
        DrawRectangle(bullets[i].pos.x - (2 / camera.zoom), bullets[i].pos.y - (2 / camera.zoom), 4 / camera.zoom, 4 / camera.zoom, WHITE);
    }
}
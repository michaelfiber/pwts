#include "enemy.h"
#include "missiles.h"
#include "game.h"

Enemy enemies[MAX_ENEMY];

void update_enemies()
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemies[i].life <= 0.0f)
        {
            continue;
        }

        enemies[i].missile_timer -= GetFrameTime();
        if (enemies[i].missile_timer <= 0.0f && CheckCollisionPointCircle(player.loc, (Vector2){enemies[i].loc.dest.x, enemies[i].loc.dest.y}, 700.0f))
        {
            enemies[i].missile_timer = enemies[i].missile_wait;
            fire_missile((Vector2){enemies[i].loc.dest.x, enemies[i].loc.dest.y}, (Vector2){player.loc.x + player.vel.x, player.loc.y + player.vel.y});
        }
    }
}

void add_enemy(Vector2 loc, float life, Texture2D tex)
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemies[i].life <= 0.0f)
        {
            enemies[i].life = life;
            enemies[i].loc.dest.x = loc.x - tex.width / 2;
            enemies[i].loc.dest.y = loc.y - tex.height / 2;
            enemies[i].loc.dest.width = tex.width;
            enemies[i].loc.dest.height = tex.height;
            enemies[i].tex = tex;
            enemies[i].missile_timer = 1.0f;
            enemies[i].missile_wait = enemies[i].missile_timer;
            add_collider(&enemies[i].loc, LOC_TYPE_ENEMY);
            break;
        }
    }
}

void draw_enemies()
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemies[i].life <= 0.0f)
        {
            continue;
        }

        DrawTexturePro(enemies[i].tex, (Rectangle){0, 0, enemies[i].tex.width, enemies[i].tex.height}, enemies[i].loc.dest, (Vector2){0, 0}, 0.0f, WHITE);
    }
}
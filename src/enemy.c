#include "enemy.h"
#include "missiles.h"
#include "game.h"
#include "puff.h"

Enemy enemies[MAX_ENEMY];
int enemy_count = 0;

void update_enemies()
{
    enemy_count = 0;
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemies[i].life <= 0.0f)
        {
            continue;
        }

        enemy_count++;

        enemies[i].missile_timer -= GetFrameTime();

        if (enemies[i].missile_timer <= 0.0f && CheckCollisionPointCircle(player.loc, (Vector2){enemies[i].loc.dest.x, enemies[i].loc.dest.y}, 700.0f))
        {
            enemies[i].missile_timer = enemies[i].missile_wait;
            float angle = 90 * DEG2RAD;
            if (player.loc.y > enemies[i].loc.dest.y)
            {
                angle = 270 * DEG2RAD;
            }
            fire_missile((Vector2){enemies[i].loc.dest.x, enemies[i].loc.dest.y}, (Vector2){player.loc.x + player.vel.x, player.loc.y + player.vel.y}, angle);
        }

        switch (enemies[i].loc.is_hitting_type)
        {
        case LOC_TYPE_BULLET:
            enemies[i].life -= 10.0f;
            break;
        case LOC_TYPE_MISSILE:
            enemies[i].life -= 100.0f;
            break;
        }

        if (enemies[i].life <= 0.0f)
        {
            TraceLog(LOG_INFO, "Destroy enemy");
            add_puff((Vector2){enemies[i].loc.dest.x + enemies[i].loc.dest.width / 2, enemies[i].loc.dest.y + enemies[i].loc.dest.height / 2}, 1000.0f);
            remove_collider(&enemies[i].loc);
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
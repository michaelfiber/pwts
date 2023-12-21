#include "math.h"
#include "explosion.h"

#define EXPLOSIONS_MAX 100

Explosion e[EXPLOSIONS_MAX];

void add_explosion(Vector2 pos, float radius, int particle_count)
{
    if (particle_count > 50)
    {
        particle_count = 50;
    }

    for (int i = 0; i < EXPLOSIONS_MAX; i++)
    {
        if (e[i].life <= 0.0f)
        {
            e[i].life = 1.0f;
            e[i].center = pos;
            e[i].radius = radius;
            for (int j = 0; j < 50; j++)
            {
                if (j > particle_count)
                {
                    e[i].sparks[i].life = 0.0f;
                    continue;
                }
                e[i].sparks[j].life = 1.0f + GetRandomValue(0, 5);
                float angle = GetRandomValue(0, 359) * 1.0f;
                e[i].sparks[j].vel.x = cosf(angle * DEG2RAD);
                e[i].sparks[j].vel.y = sinf(angle * DEG2RAD);
                e[i].sparks[j].pos.x = pos.x + radius * e[i].sparks[j].vel.x;
                e[i].sparks[j].pos.y = pos.y + radius * e[i].sparks[j].vel.y;
                e[i].sparks[j].color = RED;
            }
            break;
        }
    }
}

void update_explosions()
{
    for (int i = 0; i < EXPLOSIONS_MAX; i++)
    {
        if (e[i].life <= 0.0f)
        {
            continue;
        }

        for (int j = 0; j < 50; j++)
        {
            if (e[i].sparks[j].life <= 0.0f)
            {
                continue;
            }
            e[i].sparks[j].life -= GetFrameTime();
        }

        e[i].life -= GetFrameTime();
    }
}

void draw_explosions()
{
    for (int i = 0; i < EXPLOSIONS_MAX; i++)
    {
        if (e[i].life <= 0.0f)
        {
            continue;
        }

        DrawCircleV(e[i].center, e[i].radius + (1 - e[i].life) * e[i].radius / 2.0f, Fade(GOLD, e[i].life));
        for (int j = 0; j < 50; j++)
        {
            if (e[i].sparks[j].life > 0.0f)
            {
                DrawCircleV(e[i].sparks[j].pos, 2.0f, e[i].sparks[j].color);
            }
        }
    }
}
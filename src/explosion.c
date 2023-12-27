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
            for (int j = 0; j < SPARKS_MAX; j++)
            {
                if (j > particle_count)
                {
                    e[i].sparks[i].life = 0.0f;
                    continue;
                }
                e[i].sparks[j].life = 1.0f + GetRandomValue(0, 10) / 10.0f;
                float angle = GetRandomValue(0, 359) * 1.0f;
                e[i].sparks[j].vel.x = cosf(angle * DEG2RAD) * GetRandomValue(0, radius);
                e[i].sparks[j].vel.y = sinf(angle * DEG2RAD) * GetRandomValue(0, radius);
                e[i].sparks[j].pos.x = pos.x + cosf(angle * DEG2RAD) * radius;
                e[i].sparks[j].pos.y = pos.y + sinf(angle * DEG2RAD) * radius;
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
        if (e[i].life > 0.0f)
        {
            e[i].life -= GetFrameTime();
        }

        for (int j = 0; j < SPARKS_MAX; j++)
        {
            if (e[i].sparks[j].life > 0.0f)
            {
                e[i].sparks[j].pos.x += e[i].sparks[j].vel.x * GetFrameTime();
                e[i].sparks[j].pos.y += e[i].sparks[j].vel.y * GetFrameTime();
                e[i].sparks[j].life -= GetFrameTime();
            }
        }

    }
}

void draw_explosions()
{
    for (int i = 0; i < EXPLOSIONS_MAX; i++)
    {
        if (e[i].life > 0.0f)
        {
            DrawCircleV(e[i].center, e[i].radius + (1 - e[i].life) * e[i].radius / 2.0f, Fade(GOLD, e[i].life));    
        }
        
        for (int j = 0; j < SPARKS_MAX; j++)
        {
            if (e[i].sparks[j].life > 0.0f)
            {
                DrawCircleV(e[i].sparks[j].pos, 2.0f, Fade(e[i].sparks[j].color, e[i].sparks[j].life));
            }
        }
    }
}
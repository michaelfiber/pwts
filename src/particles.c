#include "particles.h"
#include "ship.h"
#include <math.h>
#include <stdio.h>

void draw_emitter(Emitter *e)
{
    // DrawLineEx(e->loc, (Vector2){e->loc.x + cosf(e->rot * DEG2RAD) * 100, e->loc.y + sinf(e->rot * DEG2RAD) * 100}, 5.0f, e->color);
    for (int i = 0; i < PARTICLES_MAX; i++)
    {
        if (e->pool[i].life > 0.0f)
        {
            DrawCircleV(e->pool[i].loc, e->size * (e->pool[i].life / e->life), Fade(e->pool[i].color, (e->pool[i].life / e->life)));
        }
    }
}

void update_emitter(Vector2 loc, float rot, Vector2 vel, Emitter *e)
{
    e->loc.x = loc.x;
    e->loc.y = loc.y;
    e->rot = rot;
    e->vel.x = vel.x;
    e->vel.y = vel.y;

    e->cooldown -= GetFrameTime();
    if (e->cooldown < 0.0f)
    {
        if (e->on)
        {

            for (int i = 0; i < PARTICLES_MAX; i++)
            {
                if (e->pool[i].life > 0.0f)
                {
                    continue;
                }

                int offset = GetRandomValue(-e->width/2, e->width/2);

                e->pool[i].life = e->life;
                e->pool[i].loc.x = e->loc.x + offset * sinf(rot * DEG2RAD); 
                e->pool[i].loc.y = e->loc.y + offset * cosf(rot * DEG2RAD);
                e->pool[i].vel.x = e->vel.x + e->thrust * cosf(rot * DEG2RAD);
                e->pool[i].vel.y = e->vel.y + e->thrust * sinf(rot * DEG2RAD);
                e->pool[i].color = e->color;
                break;
            }
            e->cooldown = 1.0f / e->rate;
        }
    }

    for (int i = 0; i < PARTICLES_MAX; i++)
    {
        if (e->pool[i].life > 0.0f)
        {
            e->pool[i].life -= GetFrameTime();
            e->pool[i].loc.x += e->pool[i].vel.x * GetFrameTime();
            e->pool[i].loc.y += e->pool[i].vel.y * GetFrameTime();

            e->pool[i].vel.x -= e->pool[i].vel.x * GetFrameTime();
            e->pool[i].vel.y -= e->pool[i].vel.y * GetFrameTime();
        }
    }
}
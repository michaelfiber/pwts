#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "puff.h"

Color puff_colors[] = {
    GRAY,
    WHITE,
    DARKGRAY};

typedef struct
{
    Vector2 loc;
    Vector2 vel;
    int color;
    float life;
} PuffPiece;

#define PUFF_COUNT 100

typedef struct Puff
{
    PuffPiece pieces[PUFF_COUNT];
    struct Puff *next_puff;
} Puff;

Puff *root_puff;

void add_puff(Vector2 loc, float speed)
{
    TraceLog(LOG_INFO, "Add puff");

    Puff *p = MemAlloc(sizeof(Puff));
    p->next_puff = NULL;
    for (int i = 0; i < PUFF_COUNT; i++)
    {
        float puff_speed = speed / GetRandomValue(4, 12);

        p->pieces[i].life = GetRandomValue(1, 20) / 10.0f;
        p->pieces[i].color = GetRandomValue(0, 2);
        float angle = GetRandomValue(0, 359) * DEG2RAD;
        p->pieces[i].vel.x = cosf(angle) * puff_speed;
        p->pieces[i].vel.y = sinf(angle) * puff_speed;
        p->pieces[i].loc.x = loc.x;
        p->pieces[i].loc.y = loc.y;
    }

    if (root_puff == NULL)
    {
        TraceLog(LOG_INFO, "Root is null");
        root_puff = p;
        return;
    }

    TraceLog(LOG_INFO, "Search for null puff");
    Puff *target = root_puff;
    while (target != NULL)
    {
        if (target->next_puff == NULL)
        {
            TraceLog(LOG_INFO, "Add puff to the end");
            target->next_puff = p;
            return;
        }
        target = target->next_puff;
    }

    TraceLog(LOG_INFO, "Puff not placed");
    MemFree(p);
}

void update_puffs()
{
    Puff *prev = NULL;
    Puff *target = root_puff;
    int puff_count = 0;

    while (target != NULL)
    {
        puff_count++;
        int alive_count = 0;

        for (int i = 0; i < PUFF_COUNT; i++)
        {
            if (target->pieces[i].life <= 0.0f)
            {
                continue;
            }
            alive_count++;
            target->pieces[i].loc.x += target->pieces[i].vel.x * GetFrameTime();
            target->pieces[i].loc.y += target->pieces[i].vel.y * GetFrameTime();
            target->pieces[i].life -= GetFrameTime();
        }

        if (alive_count == 0)
        {
            if (prev == NULL)
            {
                root_puff = target->next_puff;
            }
            else
            {
                prev->next_puff = target->next_puff;
            }
            MemFree(target);
            target = target->next_puff;
        }
        else
        {
            prev = target;
            target = target->next_puff;
        }
    }
}

void draw_puffs()
{
    Puff *target = root_puff;
    while (target != NULL)
    {
        for (int i = 0; i < PUFF_COUNT; i++)
        {
            if (target->pieces[i].life <= 0.0f)
            {
                continue;
            }

            DrawCircleV(target->pieces[i].loc, 2.0f, puff_colors[target->pieces[i].color]);
        }
        target = target->next_puff;
    }
}

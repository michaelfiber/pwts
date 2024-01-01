#pragma once

#include "collider.h"
#include <raylib.h>

#define MAX_ENEMY 20

typedef struct
{
    float missile_wait;
    float missile_timer;
    float life;
    Texture2D tex;
    Location loc;
} Enemy;

extern Enemy enemies[MAX_ENEMY];

void add_enemy(Vector2 loc, float life, Texture2D tex);
void draw_enemies();
void update_enemies();
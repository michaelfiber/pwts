#pragma once
#include <raylib.h>

typedef struct
{
    Vector2 pos;
    Vector2 vel;
    float life;
    Color color;
} Spark;

typedef struct
{
    float radius;
    Vector2 center;
    float life;
    Spark sparks[25];
} Explosion;

void add_explosion(Vector2 pos, float radius, int particle_count);
void update_explosions();
void draw_explosions();
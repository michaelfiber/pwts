#pragma once
#include <raylib.h>
#include "collider.h"

typedef struct
{
    Location loc;
    int type;
    int frame;
    float cool_down;
    float rate;
    float rot;
    float delta_rot;
    int life;
    Vector2 vel;
} Asteroid;

#define ASTEROID_MAX 500
extern Asteroid asteroids[ASTEROID_MAX];

void init_asteroid(int asteroid_count);
void update_asteroids();
void draw_asteroid();
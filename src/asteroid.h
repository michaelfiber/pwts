#pragma once

typedef struct
{
    Rectangle dest;
    int type;
    int frame;
    float cool_down;
    float rate;
    float rot;
    float delta_rot;
} Asteroid;

#define ASTEROID_MAX 500
extern Asteroid asteroids[ASTEROID_MAX];

void init_asteroid();
void update_asteroids();
void draw_asteroid();
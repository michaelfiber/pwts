#pragma once
#include <raylib.h>

#define PARTICLES_MAX 100

typedef struct
{
    Vector2 loc;
    Vector2 vel;
    Color color;
    float life;
} Particle;

typedef struct 
{
    bool on;
    float rate;
    Vector2 loc;
    float rot;
    Color color;

    // Time in seconds the particle lives for
    float life;
    
    Vector2 vel;
    
    // Vel added to emitter vel on emit
    float thrust;
    
    // Initial particle size
    float size;

    float width;
    float cooldown;

    Particle pool[PARTICLES_MAX];
} Emitter;

void draw_emitter(Emitter *e);
void update_emitter(Vector2 loc, float rot, Vector2 vel, Emitter *e);
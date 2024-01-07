#pragma once

#include <raylib.h>
#include "particles.h"
#include "collider.h"

#define EMITTERS_MAX 3

typedef struct
{
    Emitter em;
    float distance;
    float offset;
} EmitterPlacement;

typedef struct
{
    float broken_gun_timer;
    float broken_railgun_timer;
    float broken_missile_timer;

    float shake_timer;
    float life;
    Texture2D tex;
    float rot;
    float delta_rot;
    Vector2 loc;
    Vector2 vel;

    float thruster_power;
    float engine_power;
    float bullet_speed;
    
    EmitterPlacement emitters[EMITTERS_MAX];

    float railgun_cooldown;

    Location collider;

} Ship;

void init_ship(Ship *s);
void update_ship(Ship *s);
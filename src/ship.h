#pragma once

#include <raylib.h>
#include "particles.h"

typedef struct
{
    Texture2D tex;
    float rot;
    float delta_rot;
    Vector2 loc;
    Vector2 vel;

    float thruster_power;
    float engine_power;
    float bullet_speed;

    Emitter em_engine;
    Emitter em_bow_p;
    Emitter em_bow_s;
    Emitter em_stern_p;
    Emitter em_stern_s;
} Ship;

void update_ship(Ship *s);
#pragma once

#include <raylib.h>

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
} Ship;
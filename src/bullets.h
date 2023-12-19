#pragma once

#include <raylib.h>
#include "ship.h"
#include "collider.h"

typedef struct
{
    Location loc;
    Vector2 vel;
    float life;
    float power;
} Bullet;

void update_bullets(Vector2 targeter, Ship player);
void draw_bullets(Camera2D camera);
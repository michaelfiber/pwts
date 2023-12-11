#pragma once

#include <raylib.h>
#include "ship.h"

typedef struct
{
    Vector2 pos;
    Vector2 vel;
    float life;
    float power;
} Bullet;

void update_bullets(Vector2 targeter, Ship player);
void draw_bullets(Camera2D camera);
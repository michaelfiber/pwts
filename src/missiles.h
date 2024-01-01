#pragma once

#include <raylib.h>
#include "ship.h"
#include "collider.h"

#define MISSILE_FLOAT 0
#define MISSILE_GO 1
#define MISSILE_EXPLODE 2
#define TARGET_MAX 16

typedef struct
{
    bool is_active;
    Vector2 pos;
    Location loc;
    Vector2 vel;
    float fuse;
    int state;
    float rot;
    float target_rot;
} Missile;

typedef struct
{
    bool is_player;
    bool is_active;
    Vector2 pos;
    Missile missile;
} Target;

void init_missiles();
void update_missiles(Vector2 targeter, Ship player);
void draw_missiles(Camera2D camera);
void fire_missile(Vector2 pos, Vector2 dest);
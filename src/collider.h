#pragma once
#include <raylib.h>

#define LOC_TYPE_BULLET 0
#define LOC_TYPE_MISSILE 1
#define LOC_TYPE_ASTEROID 2
#define LOC_TYPE_SHIP 3

typedef struct
{
    Rectangle dest;
    int coll_id;
    int is_hitting_type;
} Location;

void add_collider(Location *loc, int type);
void remove_collider(Location *loc);
void draw_colliders();
void check_colliders();
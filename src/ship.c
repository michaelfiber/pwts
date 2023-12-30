#include <raylib.h>
#include "ship.h"
#include <math.h>
#include "collider.h"
#include "game.h"

void init_ship(Ship *s)
{
    add_collider(&s->collider, LOC_TYPE_SHIP);
}

void update_ship(Ship *s)
{
    s->loc.x += s->vel.x * GetFrameTime();
    s->loc.y += s->vel.y * GetFrameTime();
    s->rot += s->delta_rot * GetFrameTime();

    if (s->vel.x < 0 && s->loc.x < MAP_WIDTH / -2 - s->tex.width / 2)
    {
        s->loc.x = MAP_WIDTH / 2 + s->tex.width / 2;
    }
    else if (s->vel.x > 0 && s->loc.x > MAP_WIDTH / 2 + s->tex.width / 2)
    {
        s->loc.x = MAP_WIDTH / -2 - s->tex.width / 2;
    }

    if (s->vel.y < 0 && s->loc.y < MAP_HEIGHT / -2 - s->tex.width / 2)
    {
        s->loc.y = MAP_HEIGHT / 2 + s->tex.width / 2;
    }
    else if (s->vel.y > 0 && s->loc.y > MAP_HEIGHT / 2 + s->tex.width / 2)
    {
        s->loc.y = MAP_HEIGHT / -2 - s->tex.width / 2;
    }

    s->collider.dest.x = s->loc.x - s->tex.width / 2;
    s->collider.dest.y = s->loc.y - s->tex.width / 2;
    s->collider.dest.width = s->tex.width;
    s->collider.dest.height = s->tex.width;

    for (int i = 0; i < EMITTERS_MAX; i++)
    {
        update_emitter((Vector2){s->loc.x + s->emitters[i].distance * cosf(s->rot * DEG2RAD), s->loc.y + s->emitters[i].distance * sinf(s->rot * DEG2RAD)}, s->rot + s->emitters[i].offset, s->vel, &s->emitters[i].em);
    }
}

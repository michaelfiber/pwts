#include <raylib.h>
#include "ship.h"
#include <math.h>

void update_ship(Ship *s)
{
    for (int i = 0; i < EMITTERS_MAX; i++)
    {
        update_emitter((Vector2){s->loc.x + s->emitters[i].distance * cosf(s->rot * DEG2RAD), s->loc.y + s->emitters[i].distance * sinf(s->rot * DEG2RAD)}, s->rot + s->emitters[i].offset, s->vel, &s->emitters[i].em);
    }
}

#include <raylib.h>
#include "ship.h"
#include <math.h>

void update_ship(Ship *s)
{
    update_emitter((Vector2){s->loc.x - 10 * cosf(s->rot * DEG2RAD), s->loc.y - 10 * sinf(s->rot * DEG2RAD)}, s->rot + 180, s->vel, &s->em_engine);
    update_emitter((Vector2){s->loc.x + 15 * cosf(s->rot * DEG2RAD), s->loc.y + 15 * sinf(s->rot * DEG2RAD)}, s->rot - 90, s->vel, &s->em_bow_p);
    update_emitter((Vector2){s->loc.x + 15 * cosf(s->rot * DEG2RAD), s->loc.y + 15 * sinf(s->rot * DEG2RAD)}, s->rot + 90, s->vel, &s->em_bow_s);
}

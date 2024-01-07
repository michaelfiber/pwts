#include <raylib.h>
#include "ship.h"
#include <math.h>
#include "collider.h"
#include "game.h"
#include "my-library.h"

void init_ship(Ship *s)
{
    if (level == 1)
    {
        add_collider(&s->collider, LOC_TYPE_SHIP);
    }
    
    s->life = 1000.0f;
    s->loc.x = 0;
    s->loc.y = 0;

    s->shake_timer = 0.0f;
    s->broken_gun_timer = 0;
    s->broken_missile_timer = 0;
    s->broken_railgun_timer = 0;
}

void update_ship(Ship *s)
{
    if (s->broken_gun_timer > 0.0f)
    {
        s->broken_gun_timer -= GetFrameTime();
        if (s->broken_gun_timer <= 0.0f)
        {
            Say("p d c back online");
        }
    }

    if (s->broken_missile_timer > 0.0f)
    {
        s->broken_missile_timer -= GetFrameTime();
        if (s->broken_missile_timer <= 0.0f)
        {
            Say("missile launcher repaired");
        }
    }

    if (s->broken_railgun_timer > 0.0f)
    {
        s->broken_railgun_timer -= GetFrameTime();
        if (s->broken_railgun_timer <= 0.0f)
        {
            Say("rail gun ready for action");
        }
    }

    if (s->shake_timer >= 0.0f)
    {
        s->shake_timer -= GetFrameTime();
    }

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

    if ((s->collider.is_hitting_type == LOC_TYPE_ASTEROID || s->collider.is_hitting_type == LOC_TYPE_MISSILE) && s->shake_timer <= 0.0f)
    {
        TraceLog(LOG_INFO, TextFormat("hit asteroid %f", s->shake_timer));

        if (s->collider.is_hitting_type == LOC_TYPE_ASTEROID)
        {
            s->life -= 50.0f;
        }

        if (s->collider.is_hitting_type == LOC_TYPE_MISSILE)
        {
            s->life -= 100.0f;
        }

        s->shake_timer = 2.0f;

        int damaged = GetRandomValue(0, 8);
        switch (damaged)
        {
        case 1:
            s->broken_gun_timer = GetRandomValue(1, 10);
            Say("P D C damaged");
            break;
        case 2:
            s->broken_missile_timer = GetRandomValue(1, 10);
            Say("missile launcher knocked out");
            break;
        case 3:
            s->broken_railgun_timer = GetRandomValue(1, 10);
            Say("rail gun offline");
            break;
        }
    }
}

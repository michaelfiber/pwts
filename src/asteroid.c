#include <raylib.h>
#include "asteroid.h"
#include "collider.h"

#define FRAME_COUNT 48

Texture2D tex[3];

Rectangle frames[FRAME_COUNT];
int frame_width = 128;
int frame_height = 128;

Asteroid asteroids[ASTEROID_MAX];

void init_asteroid()
{
    // Add to credits: https://opengameart.org/content/asteroid-sprite-sheets
    if (tex[0].id == 0)
    {
        tex[0] = LoadTexture("resources/asteroid1.png");
        tex[1] = LoadTexture("resources/asteroid2.png");
        tex[2] = LoadTexture("resources/asteroid3.png");
    }

    int i = 0;
    for (int y = 0; y < 7; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            if (i >= FRAME_COUNT)
            {
                break;
            }

            frames[i].x = x * frame_width;
            frames[i].y = y * frame_height;
            frames[i].width = frame_width;
            frames[i].height = frame_height;
            i++;
        }
    }

    for (int a = 0; a < ASTEROID_MAX; a++)
    {
        remove_collider(&asteroids[a].loc);
        
        asteroids[a].type = GetRandomValue(0, 2);
        asteroids[a].frame = GetRandomValue(0, FRAME_COUNT - 1);
        asteroids[a].rate = GetRandomValue(1, 5) / 10.0f;
        asteroids[a].loc.dest.x = GetRandomValue(-5000, 5000);
        asteroids[a].loc.dest.y = GetRandomValue(-5000, 5000);
        asteroids[a].loc.dest.width = GetRandomValue(0, 96) + 32;
        asteroids[a].loc.dest.height = asteroids[a].loc.dest.width;
        asteroids[a].delta_rot = GetRandomValue(-500, 500);
        asteroids[a].life = (asteroids[a].loc.dest.width - 32) * 10;
        add_collider(&asteroids[a].loc, LOC_TYPE_ASTEROID);
    }
}

void update_asteroids()
{
    for (int i = 0; i < ASTEROID_MAX; i++)
    {
        if (asteroids[i].life <= 0)
            continue;

        asteroids[i].cool_down -= GetFrameTime();

        if (asteroids[i].cool_down <= 0.0f)
        {
            asteroids[i].frame++;
            if (asteroids[i].frame >= FRAME_COUNT)
            {
                asteroids[i].frame = 0;
            }
            asteroids[i].cool_down = asteroids[i].rate;
        }

        switch (asteroids[i].loc.is_hitting_type)
        {
        case LOC_TYPE_BULLET:
            asteroids[i].life -= 1;
            break;
        case LOC_TYPE_MISSILE:
            asteroids[i].life -= 50;
            break;
        }

        if (asteroids[i].life <= 0)
        {
            remove_collider(&asteroids[i].loc);
        }
    }
}

void draw_asteroid()
{
    for (int i = 0; i < ASTEROID_MAX; i++)
    {
        if (asteroids[i].life <= 0)
        {
            continue;
        }
        DrawTexturePro(tex[asteroids[i].type], frames[asteroids[i].frame], asteroids[i].loc.dest, (Vector2){0, 0}, 0.0f, WHITE);
    }
}
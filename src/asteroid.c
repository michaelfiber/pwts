#include <raylib.h>
#include "asteroid.h"

#define FRAME_COUNT 48

Texture2D tex;

Rectangle frames[FRAME_COUNT];
int frame_width = 37;
int frame_height = 37;

Asteroid asteroids[ASTEROID_MAX];

void init_asteroid()
{
    tex = LoadTexture("resources/asteroid1.png");
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
        asteroids[a].frame = GetRandomValue(0, FRAME_COUNT - 1);
        asteroids[a].rate = GetRandomValue(5, 10) / 10.0f;
        asteroids[a].dest.x = GetRandomValue(-5000, 5000);
        asteroids[a].dest.y = GetRandomValue(-5000, 5000);
        asteroids[a].dest.width = GetRandomValue(32, 128);
        asteroids[a].dest.height = asteroids[a].dest.width;
        asteroids[a].delta_rot = GetRandomValue(-500, 500);
    }
}

void update_asteroids()
{
    for (int i = 0; i < ASTEROID_MAX; i++)
    {
        //asteroids[i].rot += asteroids[i].delta_rot * GetFrameTime();

        asteroids[i].cool_down -= GetFrameTime();
        if (asteroids[i].cool_down < 0.0f)
        {
            asteroids[i].frame++;
            if (asteroids[i].frame >= FRAME_COUNT)
            {
                asteroids[i].frame = 0;
            }
            asteroids[i].cool_down = asteroids[i].rate;
        }
    }
}

void draw_asteroid()
{
    for (int i = 0; i < ASTEROID_MAX; i++)
    {
        DrawTexturePro(tex, frames[asteroids[i].frame], asteroids[i].dest, (Vector2){asteroids[i].dest.width / 2.0f, asteroids[i].dest.height / 2.0f}, 0.0f, WHITE);
    }
}
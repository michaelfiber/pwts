#include <math.h>
#include <raylib.h>
#include "asteroid.h"
#include "collider.h"
#include "game.h"
#include "puff.h"

#define FRAME_COUNT 48

Texture2D tex[3];

Rectangle frames[FRAME_COUNT];
int frame_width = 128;
int frame_height = 128;

Asteroid asteroids[ASTEROID_MAX];

void add_asteroid(Asteroid asteroid);

void destroy_asteroid(Asteroid *a)
{
    add_puff((Vector2){a->loc.dest.x + a->loc.dest.width / 2, a->loc.dest.y + a->loc.dest.height / 2}, a->loc.dest.width * 10);
    remove_collider(&a->loc);

    float original_life = (a->loc.dest.width - 32) * 100;
    if (original_life > 250)
    {
        float balance = GetRandomValue(0, 3) / 10.0f;
        float width = a->loc.dest.width / 2 + a->loc.dest.width * balance;
        int life = (width + 32) / 10.0f;

        add_asteroid((Asteroid){
            .type = a->type,
            .frame = GetRandomValue(0, FRAME_COUNT - 1),
            .rate = a->rate,
            .loc.dest.x = a->loc.dest.x,
            .loc.dest.y = a->loc.dest.y,
            .loc.dest.width = width,
            .loc.dest.height = width,
            .delta_rot = a->delta_rot,
            .life = life,
            .vel.x = GetRandomValue(-150, 150),
            .vel.y = GetRandomValue(-100, 100)});

        width = a->loc.dest.width / 2 - a->loc.dest.width * balance;
        life = (width + 32) / 10.0f;

        add_asteroid((Asteroid){
            .type = a->type,
            .frame = GetRandomValue(0, FRAME_COUNT - 1),
            .rate = a->rate,
            .loc.dest.x = a->loc.dest.x + a->loc.dest.width - width,
            .loc.dest.y = a->loc.dest.y + a->loc.dest.width - width,
            .loc.dest.width = width,
            .loc.dest.height = width,
            .delta_rot = a->delta_rot,
            .life = life,
            .vel.x = GetRandomValue(-150, 150),
            .vel.y = GetRandomValue(-100, 100)});
    }
    a->life = 0;
}

void add_asteroid(Asteroid asteroid)
{
    for (int a = 0; a < ASTEROID_MAX; a++)
    {
        if (asteroids[a].life == 0)
        {
            asteroids[a].type = asteroid.type;
            asteroids[a].frame = asteroid.frame;
            asteroids[a].rate = asteroid.rate;
            asteroids[a].loc.dest.x = asteroid.loc.dest.x;
            asteroids[a].loc.dest.y = asteroid.loc.dest.y;
            asteroids[a].loc.dest.width = asteroid.loc.dest.width;
            asteroids[a].loc.dest.height = asteroid.loc.dest.height;
            asteroids[a].delta_rot = asteroid.delta_rot;
            asteroids[a].life = asteroid.life;
            asteroids[a].vel.x = asteroid.vel.x;
            asteroids[a].vel.y = asteroid.vel.y;
            asteroids[a].invuln = true;
            add_collider(&asteroids[a].loc, LOC_TYPE_ASTEROID);
            break;
        }
    }
}

void add_random_asteroid()
{
    float angle = GetRandomValue(0, 359) * DEG2RAD;
    float distance = GetRandomValue(200, MAP_HEIGHT / 2 - 100);

    int width = GetRandomValue(10, 96) + 32;
    add_asteroid((Asteroid){
        .type = GetRandomValue(0, 2),
        .frame = GetRandomValue(0, FRAME_COUNT - 1),
        .rate = GetRandomValue(1, 5) / 10.0f,
        .loc.dest.x = cosf(angle) * distance,
        .loc.dest.y = sinf(angle) * distance,
        .loc.dest.width = width,
        .loc.dest.height = width,
        .delta_rot = GetRandomValue(-500, 500),
        .life = (width - 32) * 10,
        .vel.x = GetRandomValue(-150, 150),
        .vel.y = GetRandomValue(-100, 100)});
}

void init_asteroid(int asteroid_coint)
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

    // zero out the asteroids.
    for (int a = 0; a < ASTEROID_MAX; a++)
    {
        remove_collider(&asteroids[a].loc);
        asteroids[a].life = 0.0f;
    }

    for (int a = 0; a < asteroid_coint; a++)
    {
        add_random_asteroid();
    }
}

void update_asteroids()
{
    for (int i = 0; i < ASTEROID_MAX; i++)
    {
        if (asteroids[i].life <= 0.0f)
        {
            continue;
        }

        asteroids[i].loc.dest.x += asteroids[i].vel.x * GetFrameTime();
        asteroids[i].loc.dest.y += asteroids[i].vel.y * GetFrameTime();

        if (asteroids[i].vel.x < 0 && asteroids[i].loc.dest.x + asteroids[i].loc.dest.width < MAP_WIDTH / -2)
        {
            asteroids[i].loc.dest.x = MAP_WIDTH / 2;
        }
        else if (asteroids[i].vel.x > 0 && asteroids[i].loc.dest.x > MAP_WIDTH / 2)
        {
            asteroids[i].loc.dest.x = MAP_WIDTH / -2 - asteroids[i].loc.dest.width;
        }

        if (asteroids[i].vel.y < 0 && asteroids[i].loc.dest.y + asteroids[i].loc.dest.height < MAP_HEIGHT / -2)
        {
            asteroids[i].loc.dest.y = MAP_HEIGHT / 2;
        }
        else if (asteroids[i].vel.y > 0 && asteroids[i].loc.dest.y > MAP_HEIGHT / 2)
        {
            asteroids[i].loc.dest.y = MAP_HEIGHT / -2 - asteroids[i].loc.dest.height;
        }

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

        if (!asteroids[i].invuln)
        {
            switch (asteroids[i].loc.is_hitting_type)
            {
            case LOC_TYPE_BULLET:
                asteroids[i].life -= 10;
                break;
            case LOC_TYPE_MISSILE:
                asteroids[i].life -= 100;
                break;
            }
        }

        if (asteroids[i].life <= 0)
        {
            destroy_asteroid(&asteroids[i]);
        }

        asteroids[i].invuln = false;
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
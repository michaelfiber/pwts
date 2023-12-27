#include <raylib.h>
#include "starfield.h"

#define STARS_MAX 8000

RenderTexture2D starfield = {0};

RenderTexture2D get_starfield()
{
    if (starfield.id == 0)
    {
        starfield = LoadRenderTexture(12000, 12000);

        BeginTextureMode(starfield);
        {
            ClearBackground(Fade(BLACK, 0.0f));

            Color star_color = (Color){150, 200, 255, 200};

            for (int i = 0; i < STARS_MAX; i++)
            {
                float x = GetRandomValue(0, 12000);
                float y = GetRandomValue(0, 12000);
                float z = GetRandomValue(1, 20) / 10.0f;
                DrawCircle(x, y, z, star_color);
            }
        }
        EndTextureMode();
    }

    return starfield;
}
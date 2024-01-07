#include <raylib.h>
#include "briefing.h"

#define LINE_COUNT 4

float bfade[4];

char *lines[4] = 
{
    "Enemy ships are occupying the asteroid belt.",
    "Find them and destroy them before they can",
    "harvest asteroids to hurl at Earth.",
    "[ENTER]"
};
  
bool draw_briefing()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        return false;
    }

    if (bfade[0] < 1.0f)
    {
        bfade[0] += GetFrameTime();
    }
    else if (bfade[1] < 1.0f)
    {
        bfade[0] = 1.0f;
        bfade[1] += GetFrameTime();
    }
    else if (bfade[2] < 1.0f)
    {
        bfade[1] = 1.0f;
        bfade[2] += GetFrameTime();
    }
    else if (bfade[3] < 1.0f)
    {
        bfade[2] = 1.0f;
        bfade[3] += GetFrameTime();
    }
    else
    {
        bfade[3] = 1.0f;
    }

    int size = GetFontDefault().baseSize * 3;

    for (int i = 0; i < LINE_COUNT; i++)
    {
        Color c = Fade(WHITE, bfade[i]);
        int width = MeasureText(lines[i], size);
        DrawText(lines[i], GetScreenWidth() / 2 - width / 2, 100 + 100 * i, size, c);
    }

    return true;
}
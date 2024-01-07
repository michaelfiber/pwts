#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include "title.h"
#include "starfield.h"

Texture2D asteroid_tex;
RenderTexture2D title_render_texture;
float sleep = 1.0f;
float fade = 0.0f;
bool fade_out = false;
float offset = 0.0f;
int frame = 0;
float frame_duration = 0.08f;
float frame_timer = 0.15f;
Rectangle f[48];

void init_title()
{
    if (title_render_texture.id == 0)
    {
        title_render_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    if (asteroid_tex.id == 0)
    {
        asteroid_tex = LoadTexture("resources/asteroid1.png");
    }

    fade_out = false;
    fade = 0.0f;
    sleep = 1.0f;
    offset = 0.0f;
    frame = 0;
    frame_timer = 1.0f;

    int i = 0;
    for (int y = 0; y < 7; y++)
    {
        for (int x = 0; x < 7; x++)
        {
            if (i >= 48)
            {
                break;
            }

            f[i].x = x * 128;
            f[i].y = y * 128;
            f[i].width = 128;
            f[i].height = 128;
            i++;
        }
    }
}

bool draw_title(Shader glow_shader)
{
    int raylib_text_width = MeasureText("powered by raylib", GetFontDefault().baseSize * 4);
    sleep -= GetFrameTime();
    offset -= GetFrameTime() * 1000;
    frame_timer -= GetFrameTime();
    if (frame_timer < 0.0f)
    {
        frame_timer = frame_duration;
        frame++;
        if (frame >= 48)
        {
            frame = 0;
        }
    }

    if (get_starfield().texture.width + offset < GetScreenWidth())
    {
        offset = 0.0f;
    }

    if (sleep < 0.0f && !fade_out && fade < 1.0f)
    {
        fade += GetFrameTime();
        if (fade > 1.0f)
        {
            fade = 1.0f;
        }
    }

    BeginTextureMode(title_render_texture);
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.25f));
        DrawTexture(get_starfield().texture, offset, 0, Fade(WHITE, fade));
        
        DrawText("PEOPLE WHO", 104, 201, GetFontDefault().baseSize * 10, Fade(WHITE, fade));
        DrawText("THROW STONES", 354, 401, GetFontDefault().baseSize * 10, Fade(WHITE, fade));

        DrawText("PEOPLE WHO", 100, 200, GetFontDefault().baseSize * 10, Fade(BLACK, fade));
        DrawText("THROW STONES", 350, 400, GetFontDefault().baseSize * 10, Fade(BLACK, fade));

        DrawTexturePro(asteroid_tex, f[frame], (Rectangle){1004, 239, 256, 256}, (Vector2){128, 128}, 0.0f, WHITE);

        DrawText("powered by raylib", GetScreenWidth() / 2 - raylib_text_width / 2, 650, GetFontDefault().baseSize * 4, Fade(RAYWHITE, 0.75f));
    }
    EndTextureMode();

    BeginDrawing();
    {
        BeginShaderMode(glow_shader);
        {
            DrawTextureRec(title_render_texture.texture, (Rectangle){0, 0, GetScreenWidth(), -GetScreenHeight()}, (Vector2){0, 0}, WHITE);
        }
        EndShaderMode();
    }
    EndDrawing();

    if (IsKeyDown(KEY_ENTER))
    {
        fade_out = true;
    }

    if (fade_out)
    {
        fade -= GetFrameTime();
        if (fade <= 0.0f)
        {
            return false;
        }
    }

    return true;
}
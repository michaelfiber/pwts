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
float rot = 0.0f;

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

    fade = 0.0f;
    sleep = 1.0f;
}

bool draw_title(Shader glow_shader)
{
    int raylib_text_width = MeasureText("powered by raylib", GetFontDefault().baseSize * 4);
    sleep -= GetFrameTime();
    rot += GetFrameTime() * 10;
    offset -= GetFrameTime() * 1000;

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
        DrawText("PEOPLE WHO", 100, 200, GetFontDefault().baseSize * 10, Fade(BLUE, fade));
        DrawText("THROW STONES", 350, 400, GetFontDefault().baseSize * 10, Fade(BLUE, fade));

        DrawTexturePro(asteroid_tex, (Rectangle){ 0, 0, 128, 128 }, (Rectangle){ 1004, 239, 256, 256 }, (Vector2){ 128, 128 }, rot, WHITE);

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
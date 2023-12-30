#pragma once
#include <stdlib.h>
#include <raylib.h>
#include "ship.h"

#define MAP_WIDTH 10000
#define MAP_HEIGHT 6000

extern Ship player;

void init_game();
bool draw_game(Shader glow_shader);
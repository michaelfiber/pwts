#pragma once
#include <stdlib.h>
#include <raylib.h>
#include "ship.h"

extern Ship player;

void init_game();
bool draw_game(Shader glow_shader);
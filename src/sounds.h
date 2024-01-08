#pragma once

#define SOUND_PDC 0
#define SOUND_RAIL 1
#define SOUND_MISSILE 2
#define SOUND_EXPLODE 3
#define SOUND_ENGINE 4
#define SOUND_COUNT 5

void init_sounds();
void play_sound(int sound);
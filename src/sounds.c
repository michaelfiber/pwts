#include <raylib.h>
#include "sounds.h"

Sound sounds[SOUND_COUNT];

void init_sounds()
{
    sounds[SOUND_EXPLODE] = LoadSound("resources/explode.wav");
    sounds[SOUND_MISSILE] = LoadSound("resources/missile.wav");
    sounds[SOUND_PDC] = LoadSound("resources/pdc.wav");
    sounds[SOUND_RAIL] = LoadSound("resources/rail.wav");
    sounds[SOUND_ENGINE] = LoadSound("resources/engine.wav");
}

void play_sound(int sound)
{
    if (sound < SOUND_COUNT) {
        PlaySound(sounds[sound]);
    }
}
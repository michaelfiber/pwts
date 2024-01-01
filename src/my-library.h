#ifndef MYLIBRARY_H
#define MYLIBRARY_H

#include <emscripten/emscripten.h>
extern void Say(char *text);
extern void RegisterOrientationPointers(float *beta, float *gamma);
extern void Vibrate(int *durations, int durationCount);

#endif
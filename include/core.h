#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#define DEBUG false
#define GRAVITY 980

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

#define RANDOM() ((float)rand() / (float)RAND_MAX)

#endif

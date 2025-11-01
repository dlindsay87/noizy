#ifndef __SDL_INIT_H__
#define __SDL_INIT_H__

#include <SDL2/SDL.h>

#include <iostream>

const int BIT_SIZE = 8;
const int DEPTH_SIZE = 16;

static constexpr SDL_GLprofile OPENGL_PROFILE =
    SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE;

extern void initSDL();

#endif

#pragma once

#include <SDL3/SDL.h>

inline int cstrcmpi(const char* lhs, const char* rhs) {
    return SDL_strcasecmp(lhs, rhs);
}
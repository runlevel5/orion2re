#pragma once

#include <ctype.h>

#include <SDL3/SDL.h>

#if !defined(_MSC_VER) && !defined(__cdecl)
#define __cdecl
#endif

#if !defined(_MSC_VER) && !defined(__stdcall)
#define __stdcall
#endif

#if defined(_WIN32)
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

inline char* strlwr(char* text) {
    if (text == nullptr) {
        return nullptr;
    }

    for (char* cursor = text; *cursor != '\0'; ++cursor) {
        *cursor = (char)tolower((unsigned char)*cursor);
    }

    return text;
}

inline char* strupr(char* text) {
    if (text == nullptr) {
        return nullptr;
    }

    for (char* cursor = text; *cursor != '\0'; ++cursor) {
        *cursor = (char)toupper((unsigned char)*cursor);
    }

    return text;
}

#if defined(_WIN32)
#define _strlwr strlwr
#define _strupr strupr
#endif
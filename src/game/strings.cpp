#include "deps.h"

namespace strings {
    void __cdecl Swap_Short_(int16_t* a, int16_t* b) {
        int16_t temp = *a;
        *a = *b;
        *b = temp;
    }
}
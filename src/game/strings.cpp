#include "deps.h"

namespace strings {
    void __cdecl String_To_Lower_(char* str) {
        if (str == nullptr || *str == '\0') {
            return;
        }

        for (int32_t i = 0; str[i] != '\0'; ++i) {
            // Check if character is between 'A' (0x41) and 'Z' (0x5A)
            if (str[i] >= 0x41 && str[i] <= 0x5A) {
                str[i] += 0x20; // Convert to lowercase by adding 32
            }
        }
    }

    void __cdecl Swap_Short_(int16_t* a, int16_t* b) {
        int16_t temp = *a;
        *a = *b;
        *b = temp;
    }
}
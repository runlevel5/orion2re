#include "deps.h"

namespace strings {
    void Load_Strings() {
        HAROLD::Load_H_Strings_();
        RUSS::Load_Russ_Strings_();
        ESTRINGS::Load_E_Strings_();
    }

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

    void __cdecl String_To_Upper_(char* str) {
        if (str[0] == '\0') {
            return;
        }

        int32_t i = 0;
        do {
            if (str[i] >= 0x61 && str[i] <= 0x7f) {
                str[i] -= 0x20;
            }
            i++;
        } while (str[i] != '\0');
    }

    void __cdecl Swap_Short_(int16_t* a, int16_t* b) {
        int16_t temp = *a;
        *a = *b;
        *b = temp;
    }

    void __cdecl Trim_(char* str) {
        if (!str) {
            return;
        }

        // Skip leading spaces
        int first_non_space = 0;
        while (str[first_non_space] == ' ') {
            first_non_space++;
        }

        // If there are leading spaces, shift the string to the left (including null terminator)
        if (first_non_space > 0) {
            int i = 0;
            while (true) {
                str[i] = str[first_non_space + i];
                if (str[i] == '\0') {
                    break;
                }
                i++;
            }
        }

        // Find the new end of the string
        int length = 0;
        while (str[length] != '\0') {
            length++;
        }

        // Iterate backwards from the end, replacing trailing spaces with null terminators
        for (int i = length - 1; i >= 0; --i) {
            if (str[i] == ' ') {
                str[i] = '\0';
            } else {
                break;
            }
        }
    }
}
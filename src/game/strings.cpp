#include "pch.h"

namespace strings {
    const char s__00551100[1] = "";
    const char s_plus_00551118[2] = "+";
    const char s_minus_0055111c[2] = "-";
    const char s_space_00551120[2] = { '\x1B', '\0' };
    const char s_B_005560e4[2] = "B";
    const char s_S_005569cc[2] = "S";
    const char s_O_00556afc[2] = "O";
    const char s_A_00557ed4[2] = "A";
    const char s_M_00557ed8[2] = "M";
    const char s_fmt_plus_d_00557ed0[4] = "+%d";
    const char s_ld_00558280[4] = "%ld";
    const char s_0_0055110c[3] = "\x1A" "0";
    const char s_1_00551110[3] = "\x1A" "1";
    const char s__0055ac34[2] = "\t";
    const char s_t_T0_5_124__0055ac38[13] = "\x17" "t" "\a" "T0,5,124.";

    void Load_Strings() {
        HAROLD::Load_H_Strings_();
        RUSS::Load_Russ_Strings_();
        ESTRINGS::Load_E_Strings_();
    }

    void __cdecl String_To_Lower_(char* str) {
        if (*str == '\0') {
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

    void __cdecl Swap_Char_(uint8_t* a, uint8_t* b) {
        uint8_t temp = *a;
        *a = *b;
        *b = temp;
    }

    void __cdecl Swap_(int32_t* ptr1, int32_t* ptr2) {
        int32_t temp = *ptr1;
        *ptr1 = *ptr2;
        *ptr2 = temp;
    }

    void __cdecl Delete_Structure_(int32_t index, void* array_ptr, uint32_t element_size, uint32_t count) {
        if (index < (int32_t)(count - 1)) {
            struct_::Copy_Structure_(
                (uint8_t*)array_ptr + (uint32_t)index * element_size,
                (uint8_t*)array_ptr + ((uint32_t)index + 1) * element_size,
                (count - (uint32_t)index - 1) * element_size
            );
        }
    }

    void __cdecl Trim_(char* str) {
        // Skip leading spaces
        int first_non_space = 0;
        while (str[first_non_space] == ' ') {
            first_non_space++;
        }

        // If there are leading spaces, shift the string to the left (including null terminator)
        if (first_non_space > 0) {
            memmove(str, str + first_non_space, strlen(str + first_non_space) + 1);
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
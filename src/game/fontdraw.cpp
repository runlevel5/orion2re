#include "pch.h"

namespace fontdraw {

    void __cdecl Print_Character_No_Alias_ASM_(uint8_t* dest, const uint8_t* src, const uint8_t* palette, int height)
    {
        for (int y = 0; y < height; y++) {
            uint8_t* line_start = dest;
            while (true) {
                uint8_t val = *src++;
                
                if (val == 0x80) {
                    break;
                }
                
                if (val < 0x80) {
                    if (val != 0) {
                        *dest = palette[val - 1];
                    }
                    dest++;
                } else {
                    dest += (val & 0x7F);
                }
            }
            dest = line_start + platform::g_screen_res_x;
        }
    }

} // namespace fontdraw
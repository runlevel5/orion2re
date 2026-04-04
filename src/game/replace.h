#pragma once
#include <cstdint>

struct s_animation_header;

namespace replace {
    inline uint8_t& _graphics_buffer =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0057A9A8});

    inline uint8_t*& _palette_color_list =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x0057E07C});

    void __cdecl Replace_Color_All_(s_animation_header* anim, uint8_t color);

    void __cdecl Exit_With_Message_();

    void __cdecl Replace_Color_Range_(s_animation_header* anim, uint8_t* palette_map, int16_t start_color, int16_t num_colors);

    inline const char* s_Shift_Palette_Range_exceeds_25_0055b7c4 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B7C4});

    void __cdecl Shift_Palette_Range_(s_animation_header* anim, int shift);

    void __cdecl Replace_Color_(s_animation_header *anim, int16_t old_color, int8_t new_color);

    void __cdecl Establish_Color_Gradient_(int16_t scale, int16_t invert);

    void __cdecl Interlaced_Gray_Scale_Off_Page_(int8_t color_offset1, int16_t gradient_start1, int8_t color_offset2, int16_t gradient_start2, int16_t gradient_end);
}
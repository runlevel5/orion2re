#pragma once

namespace palette {
    void __cdecl Set_Palette_Gradient_(int32_t start_index, int32_t count, int32_t start_r, int32_t start_g, int32_t start_b, int32_t end_r, int32_t end_g, int32_t end_b);

    inline s_palette_entry (&_saved_palette)[256] =
        *reinterpret_cast<s_palette_entry(*)[256]>(std::uintptr_t{0x0057B1B8});

    void __cdecl Save_Palette_();

    void __cdecl Set_Palette_Changes_(int16_t start, int16_t end);

    void __cdecl Clear_Palette_Changes_(int16_t start, int16_t end);

    inline int32_t& _cycle_direction_flag =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055B2A8});

    inline int32_t& _cycle_step_value =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0057B1B0});

    inline int32_t& _cycle_color_value =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0057B1B4});

    void __cdecl Reset_Cycle_Palette_Color_();

    void __cdecl Restore_Palette_();

    void __cdecl Refresh_Palette_();

    void __cdecl Cycle_Palette_Color_(int16_t pal_idx, int16_t r1, int16_t g1, int16_t b1, int16_t r2, int16_t g2, int16_t b2, int16_t step_val);

    void __cdecl Update_Cycle_(int16_t* bound1, int16_t* bound2);

    void __cdecl Save_Glass_Remap_Block_(void* buffer);

    void* __cdecl Allocate_Glass_Remap_Next_Block_(void* block);

    void __cdecl Set_Glass_Remap_Color_(int16_t idx, uint8_t r, uint8_t g, uint8_t b, uint8_t c);

    void __cdecl Restore_Glass_Remap_Block_(void* buffer);

    void __cdecl RGB_To_HLS_(int8_t r, int8_t g, int8_t b, int32_t* h, int32_t* l, int32_t* s);

    void __cdecl HLS_To_RGB_(int32_t h, int32_t l, int32_t s, uint8_t* r, uint8_t* g, uint8_t* b);

    int32_t __cdecl Value_(int32_t p, int32_t q, int32_t t);
}
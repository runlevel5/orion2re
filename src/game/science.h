#pragma once

namespace SCIENCE {
    inline const char* const s_SR_R_x_SC_LBX_005512b0 =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x005512B0 }); // "SR_R%x_SC.LBX"

    inline const char* const s_billtext_lbx_0055135c =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x0055135C }); // "billtext.lbx"

    inline s_tech_field_data* const _technology_fields =
        reinterpret_cast<s_tech_field_data*>(std::uintptr_t{ 0x00553050 });

    void __cdecl Show_Off_Stolen_Tech_(uint16_t tech_idx, int16_t other_idx);

    inline char* s__00551100 = reinterpret_cast<char*>(std::uintptr_t{0x00551100});
    inline char* s__00551120 = reinterpret_cast<char*>(std::uintptr_t{0x00551120}); /* " " */
    inline char* s__0055136c = reinterpret_cast<char*>(std::uintptr_t{0x0055136C}); /* "\r" */

    inline s_colors& _science_high_color =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00551310});

    inline s_colors& _science_normal_color =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00551318});

    inline int16_t (&_bm_scale_x)[5] =
        *reinterpret_cast<int16_t(*)[5]>(std::uintptr_t{0x00551320});

    inline int16_t (&_bm_scale_y)[5] =
        *reinterpret_cast<int16_t(*)[5]>(std::uintptr_t{0x00551330});

    inline int16_t (&_bm_scaled_x1)[5] =
        *reinterpret_cast<int16_t(*)[5]>(std::uintptr_t{0x00551340});

    inline int16_t (&_bm_scaled_y1)[5] =
        *reinterpret_cast<int16_t(*)[5]>(std::uintptr_t{0x00551350});

    inline void*& _science_mouse_bitmap =
        *reinterpret_cast<void**>(std::uintptr_t{0x005A8BB8});

    inline void __cdecl Update_Science_Anims_(bool draw_room, bool update_room_anim, bool draw_mouse, bool draw_fields) {
        using Fn = void(__cdecl*)(bool, bool, bool, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00411640});
        fn(draw_room, update_room_anim, draw_mouse, draw_fields);
    }

    inline void __cdecl Draw_Science_Room_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004116D0});
        fn();
    }

    void __cdecl Science_Room_(char* intro_animation_name, uint16_t discovery_count, uint16_t* discovery_data, char* footer_text);

    void __cdecl Show_Off_Researched_Tech_(int16_t player_idx);
}
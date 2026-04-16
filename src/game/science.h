#pragma once
#include <cstdint>

struct s_tech_field_data;

namespace SCIENCE {
    inline const char* const s_SR_R_x_SC_LBX_005512b0 =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x005512B0 }); // "SR_R%x_SC.LBX"

    inline const char* const s_billtext_lbx_0055135c =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x0055135C }); // "billtext.lbx"

    inline s_tech_field_data* const _technology_fields =
        reinterpret_cast<s_tech_field_data*>(std::uintptr_t{ 0x00553050 });

    void __cdecl Show_Off_Stolen_Tech_(int16_t tech_idx, int16_t other_idx);

    inline void __cdecl Science_Room_(char* spy_anim, int32_t show_flag, int16_t* tech_idx, char* message_text) {
        using Fn = void(__cdecl*)(char*, int32_t, int16_t*, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00410DB0});
        fn(spy_anim, show_flag, tech_idx, message_text);
    }

    void __cdecl Show_Off_Researched_Tech_(int16_t player_idx);
}
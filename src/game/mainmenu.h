#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct s_main_menu {
    int16_t field0_0x0;
    int16_t field1_0x2;
    uint8_t field2_0x4[8];
    int16_t field_C;
    uint8_t field11_0xE[12];
    int16_t field_L;
    uint8_t field24_0x1C[12];
    int16_t field_N;
    uint8_t field37_0x2A[12];
    int16_t field_M;
    uint8_t field50_0x38[12];
    int16_t field_H;
    uint8_t field63_0x46[12];
    int16_t field_Q;
    uint8_t field76_0x54[126];
};
#pragma pack(pop)

namespace MAINMENU {
    inline s_main_menu*& _mainmenu =
        *reinterpret_cast<s_main_menu**>(std::uintptr_t{0x0056ECF0});

    inline uint32_t& _main_menu_background =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0056ECDC });

    inline const char* const s_H_005569D4 = reinterpret_cast<const char*>(0x005569D4); // "H"
    inline const char* const s_N_0055765C = reinterpret_cast<const char*>(0x0055765C); // "N"
    inline const char* const s_M_00557ED8 = reinterpret_cast<const char*>(0x00557ED8); // "M"
    inline const char* const s_save_question_gam = reinterpret_cast<const char*>(0x0055A640); // "SAVE?.GAM"
    inline const char* const s_save_percent_d_gam = reinterpret_cast<const char*>(0x0055A64C); // "SAVE%d.GAM"

    void __cdecl Main_Menu_Fade_Function_();
    void __cdecl Add_Main_Menu_Fields_();

    inline void __cdecl Main_Menu_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004e8c90})();
    }
}
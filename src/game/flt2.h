#pragma once
#include <cstdint>

namespace FLT2 {
    void __cdecl Add_Fltscrn_Small_Icon_Fields_();

    inline void __cdecl Get_Small_Icon_Animation_Size_(int16_t icon_idx, int16_t* width, int16_t* height) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004899d0});
        fn(icon_idx, width, height);
    }

    void __cdecl Fleet_Screen_Big_Icon_Fields_(int16_t* button_id_ptr, int16_t x, int16_t y, int16_t w, int16_t h);

    void __cdecl Move_Ship_To_Front_Of_List_(int16_t icon_idx);

    void __cdecl Add_Fltscrn_Big_Icon_Fields_(int16_t base_index);

    void __cdecl Set_Initial_Stack_Pointer_();

    void __cdecl Clear_All_Selection_Status_();

    void __cdecl Build_Fltscrn_Big_Icon_Fields_(int16_t stack_idx);

    bool __cdecl Ok_To_Add_Ship_(int16_t ship_idx);

    void __cdecl Set_Fleet_Screen_Stack_Owner_();

    void __cdecl Get_Fltscrn_Big_Icon_XY_(int16_t icon_idx, int16_t* out_x, int16_t* out_y);

    int16_t __cdecl Scan_Fltscrn_Big_Icons_(int16_t btn_id1, int16_t btn_id2, int16_t* out_idx1, int16_t* out_idx2);

    void __cdecl Fltscrn_Move_Ships_(int16_t target_star);

    int16_t __cdecl Get_Filter_Button_Status_();

    void __cdecl Print_Fltscrn_Scanned_Star_Name_();

    inline int16_t __cdecl Build_Fltscrn_Ship_List_(int16_t* list) {
        using Fn = int16_t(__cdecl*)(int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00487DE0});
        return fn(list);
    }

    inline s_colors& _normal_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00557DD0});
    inline s_colors& _neutral_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00557DE0});
    inline s_colors& _red_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00557DD8});

    inline char* s_s_s_005569c0 = reinterpret_cast<char*>(std::uintptr_t{0x005569C0});

    char* __cdecl Crew_Description_String_(int16_t ship_idx);

    uint8_t __cdecl Draw_Captains_Picture_(int16_t ship_idx);

    void __cdecl Print_Scanned_Ship_Data_(int16_t ship_idx);

    int16_t __cdecl Fltscrn_Ships_To_Be_Displayed_(int16_t ship_idx);

    void __cdecl Fix_Palette_(int16_t player_idx);

    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});
}
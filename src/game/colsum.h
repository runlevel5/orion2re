#pragma once
#include <cstdint>

struct s_animation_header;

namespace COLSUM {
    inline int16_t& _x_fields_3 = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00551046});
    inline s_animation_header* (&_anims)[21] = *reinterpret_cast<s_animation_header* (*)[21]>(std::uintptr_t{0x00551060});
    inline uint8_t& _draw_planet_sum = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005510C0});

    inline int16_t& _first =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005510B4});

    inline int16_t& _slider_bar_position = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005510B8});

    inline int16_t (&_list_col)[10] = *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x00563BB0});
    inline int16_t (&_list_buy_data)[10] = *reinterpret_cast<int16_t(*)[10]>(std::uintptr_t{0x00564610});
    inline int16_t& _colony_summary_saved_screen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00564624});

    void __cdecl Clear_List_Col_Array_();

    void __cdecl Init_Fields_();

    inline void __cdecl Add_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00401450});
        fn();
    }

    void __cdecl Update_Col_List_();

    void __cdecl Reset_List_Col_Array_();

    void __cdecl Load_Col_Sum_Anims_();

    inline uint8_t& _colsum_strings_been_loaded =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005510ec});

    inline char* (&_wee_event_strings)[36] =
        *reinterpret_cast<char* (*)[36]>(std::uintptr_t{0x00564668});

    void __cdecl Load_Colsum_Strings_();

    void __cdecl Sort_Col_List_();

    inline int16_t __cdecl Switched_cmp_(int16_t a1, int16_t a2) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00402AF0});
        return fn(a1, a2);
    }

    void __cdecl Draw_Colony_Summary_Screen_();

    inline void __cdecl Update_First_(int16_t n, int16_t* first, int16_t* slider_pos) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00401320});
        fn(n, first, slider_pos);
    }

    inline void __cdecl Draw_Empire_Info_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00401FD0});
        fn();
    }

    inline void __cdecl Draw_Galaxy_Map_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00402720});
        fn();
    }

    inline void __cdecl Draw_Colony_Summary_For_Colony_(int16_t index) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004021D0});
        fn(index);
    }

    inline void __cdecl Colsum_Connect_Galaxy_Map_Stars_(int16_t star1, int16_t star2, uint8_t* colors) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00402670});
        fn(star1, star2, colors);
    }

    inline void __cdecl Draw_Bar_Indicator_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00401E70});
        fn();
    }

    inline bool __cdecl Evaluate_Input_(int16_t* input_val, int16_t* scan_val) {
        using Fn = bool(__cdecl*)(int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00403000});
        return fn(input_val, scan_val);
    }

    void __cdecl Draw_Scan_Info_();

    void __cdecl Colony_Summary_Screen_();
}
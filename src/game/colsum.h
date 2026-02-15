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

    inline int8_t (&_event_good_lookup)[36] =
        *reinterpret_cast<int8_t(*)[36]>(std::uintptr_t{0x005510C8});

    inline char* (&_wee_event_strings)[36] =
        *reinterpret_cast<char* (*)[36]>(std::uintptr_t{0x00564668});

    void __cdecl Load_Colsum_Strings_();

    bool __cdecl Event_Good_(int16_t event_id);

    void __cdecl Sort_Col_List_();

    inline int16_t __cdecl Switched_cmp_(int16_t a1, int16_t a2) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00402AF0});
        return fn(a1, a2);
    }

    void __cdecl Draw_Colony_Summary_Screen_();

    void __cdecl Update_First_(int16_t param_1, int16_t* param_2, int16_t* param_3);

    inline const char* s_0_0055110c = reinterpret_cast<const char*>(std::uintptr_t{0x0055110C}); // " 0"
    inline const char* s_1_00551110 = reinterpret_cast<const char*>(std::uintptr_t{0x00551110}); // " 1"

    void __cdecl Draw_Empire_Info_();

    inline void __cdecl Draw_Galaxy_Map_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00402720});
        fn();
    }

    void __cdecl Draw_Colony_Summary_For_Colony_(int16_t index);

    inline void __cdecl Draw_Info_Pop_For_(int16_t list_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(0x00402590)(list_idx);
    }

    void __cdecl Colsum_Connect_Galaxy_Map_Stars_(int16_t star_idx1, int16_t star_idx2, s_colors* pattern);

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
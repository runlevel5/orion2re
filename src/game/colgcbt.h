#pragma once
#include <cstdint>

struct s_invasion_side_info;
struct s_animation_header;
struct s_ground_unit;

namespace COLGCBT {
    inline s_help_box (&_colony_ground_combat_screen_help)[3] =
        *reinterpret_cast<s_help_box(*)[3]>(std::uintptr_t{ 0x00558448 });
    inline s_mouse_list& _mouse_list_blank = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{ 0x005583A8 });
    inline uint8_t& _N_DEF_UNIT_ROWS = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005583B8 });
    inline uint8_t& _N_ATK_UNIT_ROWS = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005583BC });
    inline uint8_t& _title_changed_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005583C0 });
    inline int16_t& _old_ctr = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005583C4 });
    inline int16_t& _g_ctr = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005583B4 });
    inline s_invasion_side_info*& _g_attack_ptr = *reinterpret_cast<s_invasion_side_info**>(std::uintptr_t{ 0x0056C470 });
    inline s_animation_header*& _explosion_anim = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0056C474});
    inline uint16_t& _display_fast = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0056C478 });
    inline s_animation_header*& _heart_anim = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0056C47C});
    inline uint16_t& _start_combat = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0056C480 });
    inline s_animation_header*& _frame_anim = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0056C484});
    inline s_invasion_side_info*& _g_defend_ptr = *reinterpret_cast<s_invasion_side_info**>(std::uintptr_t{ 0x0056C488 });
    inline int16_t (&_fields)[2] =
        *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x0056CC60});

    inline int16_t& _top_line =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CC64});

    inline s_animation_header* (&_anims)[40] =
        *reinterpret_cast<s_animation_header*(*)[40]>(std::uintptr_t{0x005A5280});

    inline uint8_t& _player_color_array = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005583C8});
    inline uint8_t& _short_player_color_array = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558408});

    void __cdecl Replace_Colgcbt_Color_With_Player_Colors_(s_animation_header* anim, int16_t player_idx);

    void __cdecl Load_Combat_Anims_();

    inline s_ground_unit(&_g_units)[2][40] =
        *reinterpret_cast<s_ground_unit(*)[2][40]>(std::uintptr_t{ 0x0056C490 });

    int16_t __cdecl N_Slots_Left_(uint8_t side);

    inline void __cdecl Add_Initial_Unit_(uint8_t side) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004a4850 });
        fn(side);
    }

    int16_t __cdecl Get_Next_Troop_Type_(uint8_t side);

    inline int16_t __cdecl N_Troop_Type_(int16_t troop_idx, uint8_t side) {
        using Fn = int16_t(__cdecl*)(int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004a4700 });
        return fn(troop_idx, side);
    }

    void __cdecl Resort_Units_(uint8_t side_idx);

    s_animation_header* __cdecl Action_Anim_(int8_t side, uint8_t state, int8_t type);

    void __cdecl Calc_Top_Line_();

    void __cdecl Spatter_Down_Starting_Troops_();

    void __cdecl Set_Up_Troops_();

    void __cdecl Add_Global_Fields_();

    void __cdecl Draw_Colony_Combat_Screen_();

    void __cdecl Draw_Summary_Box_ATK_();

    void __cdecl Draw_Summary_Box_DEF_();

    void __cdecl Draw_Summary_Boxes_();

    void __cdecl Draw_Summary_();

    inline void __cdecl Verify_Legal_Totals_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004a3580 });
        fn();
    }

    inline void __cdecl Do_Unit_Turn_(s_ground_unit* unit) {
        using Fn = void(__cdecl*)(s_ground_unit*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004a3b40 });
        fn(unit);
    }

    inline void __cdecl Force_Firing_Pairs_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004a3610 });
        fn();
    }

    inline void __cdecl Reinforce_Units_(uint8_t side) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004a45f0 });
        fn(side);
    }

    void __cdecl Draw_Troops_();

    inline void __cdecl Print_Data_(char* name, uint16_t bonus, int16_t* param_2, s_invasion_side_info* param_1) {
        using Fn = void(__cdecl*)(char*, uint16_t, int16_t*, s_invasion_side_info*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a4e10});
        fn(name, bonus, param_2, param_1);
    }

    void __cdecl Print_Best_Summary_(s_invasion_side_info* atk_ptr, int16_t* row);

    inline void __cdecl Print_Race_Summary_(s_invasion_side_info* atk_ptr, int16_t* row) {
        using Fn = void(__cdecl*)(s_invasion_side_info*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a4ec0})(atk_ptr, row);
    }

    void __cdecl Print_Troop_Totals_(int16_t x, int16_t y, s_invasion_side_info* side_info);

    void __cdecl Set_Help_List_();

    void __cdecl Colony_Combat_Screen_(s_invasion_side_info* attacker, s_invasion_side_info* defender, uint32_t seed, int16_t param_4);
}
#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colony_combat_popup_data;
struct s_combat_bomb_result;
struct s_invasion_side_info;

namespace MAINPUPS {
    inline char*& _g_msg_ptr =
        *reinterpret_cast<char**>(std::uintptr_t{0x00556D18});

    inline s_animation_header* (&_anims)[38] =
        *reinterpret_cast<s_animation_header*(*)[38]>(std::uintptr_t{0x00565D90});

    inline uint8_t& _on_officer_screen_flag =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D38});

    inline int16_t& _g_officer =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D3C});

    inline char*& _g_str_ptr =
        *reinterpret_cast<char**>(std::uintptr_t{0x00565E28});

    inline int16_t& _last_scanned_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565D8C});
    inline int16_t& _g_state = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E6C});
    inline uint8_t& _selection_state_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D44});
    inline uint8_t& _g_using_colony_ship = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D48});
    inline int16_t& _g_max = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E70});
    inline int16_t& _g_min = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E74});
    inline char*& _g_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00565E78});
    inline int16_t& _g_var = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E7C});
    inline int16_t& _g_temp_tax_rate = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E30});
    inline int16_t& _field_state = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E34});

    inline uint8_t& _g_special = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D0C});
    inline int16_t& _g_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565D84});
    inline int16_t& _g_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565D88});
    inline int16_t& _g_star_n = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D08});
    inline int16_t& _g_player_n = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D14});
    inline int16_t& _field_mode = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D4C});
    inline uint8_t& _no_flicker_between_mainpups_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D1C});
    inline int16_t& _autofunction_ctr = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D2C});
    inline int16_t& _num_field_saves = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D30});
    inline int16_t(&_stats_array)[27] =
        *reinterpret_cast<int16_t(*)[27]>(std::uintptr_t{0x005A6E20});
    inline uint8_t& _explode_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D34});
    inline int16_t& _g_refresh_mode = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565D80});
    inline void*& _eric_saved_autofunction2 = *reinterpret_cast<void**>(std::uintptr_t{0x005A6E58});
    inline void*& _eric_saved_autofunction = *reinterpret_cast<void**>(std::uintptr_t{0x005A6E5C});

    inline void*& _temp_af =
        *reinterpret_cast<void**>(std::uintptr_t{0x00565E2C});

    inline s_colony_combat_popup_data*& _g_data_ptr =
        *reinterpret_cast<s_colony_combat_popup_data**>(std::uintptr_t{0x005A6E60});

    inline const char* s_format_string = reinterpret_cast<const char*>(std::uintptr_t{0x00551130});
    inline const char* s_SI_COMBATTE_NEL_SISTEMA = reinterpret_cast<const char*>(std::uintptr_t{0x00556D58});

    void __cdecl Load_Tax_Rate_Popup_();
    void __cdecl Get_Picture_Draw_XY_(s_animation_header* anim);

    inline void __cdecl Load_Colony_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x0045bdf0)();
    }

    inline void __cdecl Set_G_Special_Value_(int16_t param_1) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(0x0045c420)(param_1);
    }

    inline void __cdecl Update_Selection_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x0045be20)();
    }

    inline void __cdecl Draw_Selection_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x0045bea0)();
    }

    inline int16_t __cdecl Evaluate_Selection_Input_(int16_t* input, int16_t* scan_input, int16_t* done) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t*, int16_t*);
        return reinterpret_cast<Fn>(0x0045c050)(input, scan_input, done);
    }

    inline void __cdecl Load_SLIDER_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x0045cd40)();
    }

    void __cdecl Do_Popup_Setup_(int16_t param_1, s_animation_header* param_2);

    void __cdecl Add_Tax_Rate_Fields_();
    void __cdecl Eric_Pop_Fields_();

    inline void __cdecl Load_New_Officer_Popup_For_Officer_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045B350});
        fn();
    }

    inline void __cdecl Load_New_Officer_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045B2B0});
        fn();
    }

    inline void __cdecl Update_Random_New_Officer_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045AE40});
        fn();
    }

    inline void __cdecl Draw_Random_New_Officer_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045B040});
        fn();
    }

    inline int16_t (&_mp_fields)[26] =
        *reinterpret_cast<int16_t(*)[26]>(std::uintptr_t{0x00565E38});

    inline void __cdecl Eric_Push_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045a230});
        fn();
    }

    void __cdecl Draw_Tax_Rate_Popup_();

    inline void __cdecl Draw_Slider_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x0045cfa0)();
    }

    inline void __cdecl Load_Slider_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x0045cde0)();
    }

    inline void __cdecl Evaluate_Slider_Input_(int16_t* input, int16_t* scanned_field, int16_t* done) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t*);
        reinterpret_cast<Fn>(0x0045d1d0)(input, scanned_field, done);
    }

    inline void __cdecl Mainpups_Tested_Execute_Autofunction_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045B4A0});
        fn();
    }

    void __cdecl Evaluate_Tax_Rate_Input_(int16_t* input_val, int16_t* scan_val, int16_t* exit_flag);

    void __cdecl Do_Popup_Cleanup_();

    inline void __cdecl Load_Colony_Combat_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045E380});
        fn();
    }

    inline void __cdecl Draw_Colony_Combat_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045EA10});
        fn();
    }

    inline void __cdecl Update_Colony_Combat_Fields_(int16_t state, int16_t* var_1C) {
        using Fn = void(__cdecl*)(int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045E510});
        fn(state, var_1C);
    }

    inline void __cdecl Setup_To_Apply_Bomb_Hits_(s_combat_bomb_result* bomb, int16_t* bomb_hit_val1, int16_t* bomb_hit_val2) {
        using Fn = void(__cdecl*)(s_combat_bomb_result*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045DB10});
        fn(bomb, bomb_hit_val1, bomb_hit_val2);
    }

    inline void __cdecl Apply_Next_Bomb_Hit_(s_combat_bomb_result* bomb, int16_t* var_1C, int16_t* bomb_hit_val1, int16_t* bomb_hit_val2) {
        using Fn = void(__cdecl*)(s_combat_bomb_result*, int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045DB60});
        fn(bomb, var_1C, bomb_hit_val1, bomb_hit_val2);
    }

    inline int16_t __cdecl Evaluate_Colony_Combat_Input_(int16_t* input_val, int16_t* scan_val, int16_t* done, int16_t* var_1C) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045F170});
        return fn(input_val, scan_val, done, var_1C);
    }

    inline void __cdecl Load_System_Discovery_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0045ba20})();
    }

    void __cdecl Clear_System_Special_Slots_();

    inline void __cdecl Update_Discovery_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0045ba70})();
    }

    inline void __cdecl Draw_Discovery_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0045bae0})();
    }

    inline void __cdecl Evaluate_Discovery_Input_(int16_t* input_val, int16_t* scanned_val) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0045bbe0})(input_val, scanned_val);
    }

    inline void __cdecl Marooned_Officer_Fails_Popup_(int16_t star_idx, int16_t leader_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0045b470})(star_idx, leader_idx);
    }

    inline void __cdecl Marooned_Officer_Joins_Popup_(int16_t star_idx, int16_t leader_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0045b440})(star_idx, leader_idx);
    }

    void __cdecl Build_Generic_Planet_Or_Colony_Description_String_Formatted_(int16_t planet_idx, char* buffer);

    int16_t __cdecl Dragon_New_Star_Popup_(int16_t player_idx, int16_t star_idx);

    inline void __cdecl Load_Dragon_New_Star_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045f9b0});
        fn();
    }

    inline void __cdecl Draw_Dragon_New_Star_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045faf0});
        fn();
    }

    inline void __cdecl Update_Dragon_New_Star_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045fa20});
        fn();
    }

    inline int16_t __cdecl Evaluate_Dragon_New_Star_Input_(int16_t* input_val, int16_t* scanned_field, int16_t* flag, int16_t param_2) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t*, int16_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045fcf0});
        return fn(input_val, scanned_field, flag, param_2);
    }

    int16_t __cdecl Defense_Colony_Selection_Popup_(int16_t star_idx, int16_t player_idx);

    inline void __cdecl Load_Colony_Defense_Selection_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045c5c0});
        fn();
    }

    inline void __cdecl Draw_Defense_Selection_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045c650});
        fn();
    }

    inline void __cdecl Update_Colony_Defense_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045c610});
        fn();
    }

    inline int16_t __cdecl Evaluate_Defense_Selection_Input_(int16_t* input_event, int16_t* scanned_field, int16_t* leave_popup) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045c710});
        return fn(input_event, scanned_field, leave_popup);
    }

    void __cdecl Player_Attacked_Popup_(int16_t attacker_idx, int16_t defender_idx, int16_t star_idx, int16_t colony_idx);

    int16_t __cdecl Slider_Popup_(int16_t min, int16_t max, int16_t var, char* msg, int16_t param_5);

    void __cdecl Clear_New_Officer_Slots_();
    inline void __cdecl Load_Officer_Gains_Level_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045B7F0});
        fn();
    }
    inline void __cdecl Update_Officer_Gains_Level_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045B610});
        fn();
    }
    inline void __cdecl Draw_Officer_Gains_Level_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045B650});
        fn();
    }

    int16_t __cdecl New_Colony_Selection_Popup_(int16_t star_idx, uint8_t using_colony_ship);
    int16_t __cdecl New_Outpost_Selection_Popup_(int16_t star_id);

    int16_t __cdecl Drop_Troops_Selection_Popup_(int16_t target_idx);

    bool __cdecl Random_New_Officer_Popup_(int16_t player_idx);

    void __cdecl New_System_Discovery_Popup_(int16_t star_idx, uint8_t special_flag);

    void __cdecl Officer_Gains_Level_Popup_(int16_t star_handle, int16_t officer_idx);

    const char* __cdecl System_Special_Desc_(int16_t desc_id);

    s_animation_header* __cdecl System_Special_Anim_(int16_t star_idx, int16_t anim_type);

    void __cdecl Draw_Special_Info_Popup_(int16_t x, int16_t y, s_animation_header* anim, const char* name, const char* desc);

    inline int16_t __cdecl Get_Colony_Best_Shield_Bldg_(int16_t colony_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045da60});
        return fn(colony_idx);
    }

    int16_t __cdecl Colony_Selection_Popup_(
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombardment_result,
        int8_t has_stellar_converter
    );

    void __cdecl Update_CCP_Display_Data_Invasion_(
        s_colony_combat_popup_data* ccp_data,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombardment_result,
        int8_t has_stellar_converter
    );

    void __cdecl Update_CCP_Display_Data_Bombing_(
        s_colony_combat_popup_data* popup_data,
        s_combat_bomb_result* bomb_result,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombing_result,
        int8_t has_stellar_converter
    );

    int16_t __cdecl Colony_Combat_Popup_(s_colony_combat_popup_data* data, int16_t flag);

    int16_t __cdecl Colony_Bombing_Popup_(
        s_combat_bomb_result* bomb,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombing_result,
        int8_t has_stellar_converter);
}
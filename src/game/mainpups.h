#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colony_combat_popup_data;
struct s_combat_bomb_result;
struct s_invasion_side_info;
struct s_potential_colony_info;

namespace MAINPUPS {
    inline char*& _g_msg_ptr =
        *reinterpret_cast<char**>(std::uintptr_t{0x00556D18});

    inline s_animation_header* (&_anims)[38] =
        *reinterpret_cast<s_animation_header*(*)[38]>(std::uintptr_t{0x00565D90});

    inline s_animation_header*& _anim_10 =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00565DB8});

    inline s_animation_header*& _anim_11 =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00565DBC});

    inline uint8_t& _on_officer_screen_flag =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D38});

    inline int16_t& _g_officer =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D3C});

    inline uint16_t& _officer_gains_level_state =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00556D40});

    inline char*& _g_str_ptr =
        *reinterpret_cast<char**>(std::uintptr_t{0x00565E28});

    inline int16_t& _last_scanned_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565D8C});
    inline int16_t& _g_state = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E6C});
    inline int8_t& _selection_state_flag = *reinterpret_cast<int8_t*>(std::uintptr_t{0x00556D44});
    inline uint8_t& _g_using_colony_ship = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D48});
    inline int16_t& _g_max = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E70});
    inline int16_t& _g_min = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E74});
    inline char*& _g_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00565E78});
    inline int16_t& _g_var = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E7C});
    inline int16_t& _g_temp_tax_rate = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E30});
    inline int16_t& _field_state = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E34});
    inline s_colors& _main_scr_font_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00556D20});

    inline uint8_t& _g_special = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00556D0C});
    inline int16_t& _g_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565D84});
    inline int16_t& _g_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565D88});
    inline int16_t& _g_star_n = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D08});
    inline int16_t& _g_player_n = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D14});
    inline int16_t& _field_mode = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D4C});
    inline int16_t& _g_scanned_star = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D50});
    inline int16_t& _g_old_scanned_star = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556D54});
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

    void __cdecl Load_Colony_Fields_();

    inline void __cdecl Set_G_Special_Value_(int16_t param_1) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(0x0045c420)(param_1);
    }

    void __cdecl Update_Selection_Fields_();

    void __cdecl Draw_Selection_Popup_();

    int16_t __cdecl Evaluate_Selection_Input_(int16_t* selected_field, int16_t* field_data, int16_t* refresh_flag);

    int16_t __cdecl Check_System_Display_Fields_Selection_(int16_t* x, int16_t* y, int16_t* selection_confirmed);

    int8_t __cdecl New_Colony_Confirmation_Box_(int16_t planet_idx);

    void __cdecl Load_SLIDER_Popup_();
    void __cdecl Load_Slider_Fields_();
    void __cdecl Add_Slider_Fields_(int16_t val, int16_t padding, int16_t* ptr, int16_t limit);

    void __cdecl Do_Popup_Setup_(int16_t param_1, s_animation_header* param_2);

    void __cdecl Add_Tax_Rate_Fields_();
    void __cdecl Eric_Pop_Fields_();

    void __cdecl Load_New_Officer_Popup_For_Officer_Screen_();

    void __cdecl Load_New_Officer_Popup_();

    void __cdecl Update_Random_New_Officer_Fields_();

    void __cdecl Draw_Random_New_Officer_Popup_();

    inline int16_t (&_mp_fields)[26] =
        *reinterpret_cast<int16_t(*)[26]>(std::uintptr_t{0x00565E38});

    inline int16_t(&_mp_fields_2)[26] =
        *reinterpret_cast<int16_t(*)[26]>(std::uintptr_t{0x00565E3C});

    inline int16_t(&_mp_fields_3)[26] =
        *reinterpret_cast<int16_t(*)[26]>(std::uintptr_t{0x00565E3E});

    inline int16_t(&_mp_fields_9)[26] =
        *reinterpret_cast<int16_t(*)[26]>(std::uintptr_t{0x00565E4A});

    inline int16_t(&_mp_fields_10)[26] =
        *reinterpret_cast<int16_t(*)[26]>(std::uintptr_t{0x00565E4C});

    void __cdecl Eric_Push_Fields_();

    void __cdecl Add_Selection_Fields_();

    void __cdecl Draw_Tax_Rate_Popup_();

    void __cdecl Draw_Slider_Popup_();

    void __cdecl Evaluate_Slider_Input_(int16_t* input, int16_t* scanned_field, int16_t* done);

    void __cdecl Mainpups_Tested_Execute_Autofunction_();

    void __cdecl Leader_Technology_String_(int16_t player_idx, int16_t leader_idx, char* output_buf);

    void __cdecl Evaluate_Tax_Rate_Input_(int16_t* input_val, int16_t* scan_val, int16_t* exit_flag);

    void __cdecl Do_Popup_Cleanup_();

    void __cdecl Load_Colony_Combat_Popup_();

    inline void __cdecl Draw_Colony_Combat_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045EA10});
        fn();
    }

    void __cdecl Update_Colony_Combat_Fields_(int16_t planet_idx, int16_t* fields_ptr);

    inline void __cdecl Add_Colony_Combat_Fields_(int16_t planet_idx, int16_t* fields_ptr) {
        using Fn = void(__cdecl*)(int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045e530});
        fn(planet_idx, fields_ptr);
    }

    void __cdecl Setup_To_Apply_Bomb_Hits_(s_combat_bomb_result* result, int16_t* p_bomb_hits_remaining, int16_t* p_bio_hits_remaining);

    inline void __cdecl Apply_Next_Bomb_Hit_(s_combat_bomb_result* bomb, int16_t* var_1C, int16_t* bomb_hit_val1, int16_t* bomb_hit_val2) {
        using Fn = void(__cdecl*)(s_combat_bomb_result*, int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045DB60});
        fn(bomb, var_1C, bomb_hit_val1, bomb_hit_val2);
    }

    int16_t __cdecl Evaluate_Colony_Combat_Input_(int16_t* field_id_ptr, int16_t* unused, int16_t* close_popup_flag, int16_t* selection_idx_ptr);

    void __cdecl Load_Combat_Colony_Picture_To_Screen_Seg_(int16_t colony_idx);

    void __cdecl Load_System_Discovery_Popup_();

    void __cdecl Clear_System_Special_Slots_();

    void __cdecl Update_Discovery_Fields_();

    void __cdecl Draw_Discovery_Popup_();

    void __cdecl Do_Evaluate_Discovery_Input_(int16_t* mouse_x, int16_t* mouse_y);

    void __cdecl Evaluate_Discovery_Input_(int16_t* input_1, int16_t* input_2);

    void __cdecl Marooned_Officer_Fails_Popup_(int16_t star_idx, int16_t officer_idx);

    void __cdecl Marooned_Officer_Joins_Popup_(int16_t star_idx, int16_t officer_idx);

    void __cdecl Build_Generic_Planet_Or_Colony_Description_String_Formatted_(int16_t planet_idx, char* buffer);

    int16_t __cdecl Dragon_New_Star_Popup_(int16_t player_idx, int16_t star_idx);

    void __cdecl Load_Dragon_New_Star_Popup_();

    void __cdecl Draw_Dragon_New_Star_Popup_();

    void __cdecl Update_Dragon_New_Star_Fields_();

    int16_t __cdecl Evaluate_Dragon_New_Star_Input_(int16_t* field_idx_ptr, int16_t* mouse_pos_ptr, int16_t* status_ptr, int16_t target_star_val);

    int16_t __cdecl Defense_Colony_Selection_Popup_(int16_t star_idx, int16_t player_idx);

    void __cdecl Load_Colony_Defense_Selection_Popup_();

    void __cdecl Draw_Defense_Selection_Popup_();

    void __cdecl Update_Colony_Defense_Fields_();

    int16_t __cdecl Evaluate_Defense_Selection_Input_(int16_t* field_idx, int16_t* mouse_status, int16_t* selection_made);

    inline int16_t __cdecl Check_System_Display_Fields_Defense_Selection_(int16_t* field_idx, int16_t* mouse_status, int16_t* selection_made) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0045C740});
        return fn(field_idx, mouse_status, selection_made);
    }

    void __cdecl Player_Attacked_Popup_(int16_t attacker_idx, int16_t defender_idx, int16_t star_idx, int16_t colony_idx);

    int16_t __cdecl Slider_Popup_(int16_t min, int16_t max, int16_t var, char* msg, int16_t param_5);

    void __cdecl Clear_New_Officer_Slots_();

    void __cdecl Add_Officer_Gains_Level_Fields_();

    void __cdecl Load_Officer_Gains_Level_Popup_();

    void __cdecl Update_Officer_Gains_Level_Fields_();

    void __cdecl Draw_Officer_Gains_Level_Popup_();

    int16_t __cdecl New_Colony_Selection_Popup_(int16_t star_idx, uint8_t using_colony_ship);
    int16_t __cdecl New_Outpost_Selection_Popup_(int16_t star_id);

    int16_t __cdecl Drop_Troops_Selection_Popup_(int16_t target_idx);

    bool __cdecl Random_New_Officer_Popup_(int16_t player_idx);

    void __cdecl New_System_Discovery_Popup_(int16_t star_idx, uint8_t special_flag);

    void __cdecl Officer_Gains_Level_Popup_(int16_t star_handle, int16_t officer_idx);

    const char* __cdecl System_Special_Desc_(int16_t desc_id);

    s_animation_header* __cdecl System_Special_Anim_(int16_t star_idx, int16_t anim_type);

    void __cdecl Draw_Special_Info_Popup_(int16_t x, int16_t y, s_animation_header* anim, const char* name, const char* desc);

    int16_t __cdecl Get_Colony_Best_Shield_Bldg_(int16_t colony_idx);

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
        s_colony_combat_popup_data* ccp_data,
        s_combat_bomb_result* bomb,
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

    int8_t __cdecl Player_Has_Colony_Or_Outpost_At_Star_(int16_t player_idx, int16_t star_idx);
}
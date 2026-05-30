#pragma once

namespace COLGCBT {
    extern s_help_box _colony_ground_combat_screen_help[3];
    extern s_mouse_list _mouse_list_blank;
    extern uint8_t _N_DEF_UNIT_ROWS;
    extern uint8_t _N_ATK_UNIT_ROWS;
    extern uint8_t _title_changed_flag;
    extern int16_t _old_ctr;
    extern int16_t _g_ctr;
    extern s_invasion_side_info* _g_attack_ptr;
    extern const char s_t_T0_124_500_550_600_620__00558468[27];
    extern s_animation_header* _explosion_anim;
    extern uint16_t _display_fast;
    extern s_animation_header* _heart_anim;
    extern uint16_t _start_combat;
    extern s_animation_header* _frame_anim;
    extern s_invasion_side_info* _g_defend_ptr;
    extern int16_t _fields[2];

    extern int16_t _top_line;

    extern s_animation_header* _anims[40];

    extern uint8_t _player_color_array[64];
    extern uint8_t _short_player_color_array[64];

    void __cdecl Replace_Colgcbt_Color_With_Player_Colors_(s_animation_header* anim, int16_t player_idx);

    void __cdecl Load_Combat_Anims_();

    extern s_ground_unit _g_units[2][40];

    int16_t __cdecl N_Slots_Left_(uint8_t side);

    s_ground_unit* __cdecl Get_First_Unit_(uint8_t side);

    void __cdecl Add_Unit_(uint8_t side);

    void __cdecl Add_Initial_Unit_(uint8_t side);

    int16_t __cdecl Get_Next_Troop_Type_(uint8_t side);

    int16_t __cdecl N_Troop_Type_(int16_t unit_type, uint8_t side_idx);

    void __cdecl Resort_Units_(uint8_t side_idx);

    s_animation_header* __cdecl Action_Anim_(uint8_t side, uint8_t state, int8_t type);

    int16_t __cdecl Top_Line_Y_();

    int16_t __cdecl Top_Line_X_(uint8_t index);

    void __cdecl Calc_Top_Line_();

    void __cdecl Spatter_Down_Starting_Troops_();

    void __cdecl Set_Up_Troops_();

    void __cdecl Add_Global_Fields_();

    void __cdecl Draw_Colony_Combat_Screen_();

    void __cdecl Draw_Summary_Box_ATK_();

    void __cdecl Draw_Summary_Box_DEF_();

    void __cdecl Draw_Summary_Boxes_();

    void __cdecl Draw_Summary_();

    void __cdecl Verify_Legal_Totals_();

    void __cdecl Do_Unit_Turn_(s_ground_unit* unit);

    int16_t __cdecl N_Left_Active_To_Process_(s_ground_unit* start_unit);

    int16_t __cdecl N_Left_Of_Me_To_Process_(s_ground_unit* unit);

    s_ground_unit* __cdecl Pick_Target_(uint8_t side);

    s_ground_unit* __cdecl Find_Unshot_Target_(uint8_t side);

    s_ground_unit* __cdecl Find_Lowest_Framed_Unit_(uint8_t player_idx, int16_t state, uint8_t require_no_target);

    void __cdecl Draw_Unit_Beam_(s_ground_unit* unit);

    void __cdecl Get_Firepoint_Offsets_(s_ground_unit *unit, int16_t *x_off, int16_t *y_off);

    void __cdecl Get_Target_Offsets_(s_ground_unit* unit, int16_t* out_x, int16_t* out_y);

    int16_t __cdecl Get_CGCBT_Beam_Len_(uint8_t side_idx);

    void __cdecl Draw_CGCBT_Beam_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t side);

    void __cdecl Play_Unit_Beam_Sound_(s_ground_unit* unit);

    void __cdecl Pick_Next_Action_(s_ground_unit* unit);

    int8_t __cdecl Move_Is_Legal_(s_ground_unit* unit);

    void __cdecl Do_Legal_Move_Check_(s_ground_unit* unit);

    void __cdecl Do_Delta_Y_(s_ground_unit* unit);

    void __cdecl Force_Firing_Pairs_();

    void __cdecl Reinforce_Units_(uint8_t side);

    void __cdecl Draw_Troops_();

    void __cdecl Print_Data_(char* str, int16_t value, int16_t* line_idx, s_invasion_side_info* side);

    void __cdecl Print_Best_Summary_(s_invasion_side_info* atk_ptr, int16_t* row);

    void __cdecl Print_Race_Summary_(s_invasion_side_info* side_info, int16_t* y_pos);

    void __cdecl Print_Troop_Totals_(int16_t x, int16_t y, s_invasion_side_info* side_info);

    void __cdecl Set_Help_List_();

    void __cdecl Colony_Combat_Screen_(s_invasion_side_info* attacker, s_invasion_side_info* defender, uint32_t seed, int16_t param_4);
}
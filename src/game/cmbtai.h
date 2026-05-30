#pragma once
#include <cstdint>

namespace CMBTAI {
    int8_t __cdecl Get_Player_Mode_(int16_t player_idx);

    int16_t __cdecl Retreat_Check_(int16_t player_idx);

    int16_t __cdecl Get_Effective_Weapon_Damage_(int16_t ship_idx, int16_t slot_idx);

    int16_t __cdecl Get_Total_Hits_(int16_t ship_idx);

    int16_t __cdecl Get_Effective_Missile_Strength_(int16_t player_idx, int16_t* beam_strength);

    int16_t __cdecl Average_Missile_Kills_(int16_t ship_idx, int16_t weapon_slot_idx, int16_t msl_count, int16_t msl_type, int16_t* remaining_pd_count, int16_t param6);

    int16_t __cdecl Get_Long_Range_Beam_Strength_(int16_t player_idx);

    int16_t __cdecl Get_Effective_Beam_Strength_(int16_t player_idx);

    void __cdecl Toggle_Auto_Combat_Flags_();

    void __cdecl Do_Auto_Ship_Turn_(void);

    int16_t __cdecl Is_Ship_Out_Of_Ammo_(int16_t ship_idx);

    int16_t __cdecl Check_For_Launched_Fighters_(int16_t source_unit_idx);

    void __cdecl Release_Smallest_Ship_From_Stasis_(int16_t owner_idx);

    int16_t __cdecl Ai_Stasis_Check_(int16_t player_idx);

    int16_t __cdecl Select_Ship_To_Target_(int16_t ship_idx);

    int32_t __cdecl Target_Ship_Value_(int16_t attacker_idx, int16_t target_idx, int16_t distance_flag);

    int16_t __cdecl Missile_Overkill_Check_(int16_t target_idx);

    int16_t __cdecl Choose_Target_(int16_t ship_idx);

    int8_t __cdecl Combat_Ship_Class_(int16_t ship_idx, int16_t target_idx);

    int16_t __cdecl Auto_Move_Ship_(int16_t ship_param, int16_t target_idx);

    bool __cdecl Does_Ship_Have_Missiles_Incoming_(int16_t ship_idx);
    
    int16_t __cdecl Get_Current_Target_(int16_t ship_idx, int16_t param_2);

    int16_t __cdecl Boarding_Action_Type_(int16_t attacker_idx, int16_t target_idx);

    void __cdecl Resolve_Ai_Boarding_(int16_t attacker_idx, int16_t target_idx, int16_t action_type);

    int16_t __cdecl Is_Ship_Targeted_By_Missile_(int16_t ship_idx);

    void __cdecl Crystal_Control_(int16_t ship_idx);

    int8_t __cdecl Ai_Self_Destruct_Check_(int16_t ship_idx);

    int16_t __cdecl Range_To_Ship_(int16_t ship1_idx, int16_t ship2_idx);

    int16_t __cdecl Range_To_XY_(int16_t ship_idx, int16_t target_x_px, int16_t target_y_px);

    int16_t __cdecl Range_To_Missile_(int16_t unit_idx, int16_t missile_idx);

    int16_t __cdecl Will_Missile_Do_Damage_(int16_t target_idx, int16_t missile_idx);

    bool __cdecl Safe_To_Fire_Sphere_Weapon_(int16_t source_ship_idx);

    int32_t __cdecl Target_Missile_Value_(int16_t unit_idx, int32_t missile_idx, int16_t param_3);

    extern int16_t _fleet_target;

    int16_t __cdecl Get_Current_Missile_Target_(int16_t unit_idx, int32_t* value_out);

    int16_t __cdecl Get_Current_Ship_Target_(int16_t current_ship_idx, int16_t reference_target_idx, int32_t* out_value);

    int32_t __cdecl Expected_Beam_Bomb_Damage_(int16_t attacker_idx, int16_t weapon_slot, int16_t target_idx, int16_t count, int16_t distance, int16_t target_type_flag);

    int16_t __cdecl Expected_Missile_Damage_(int16_t target_ship_idx, int16_t weapon_id, int16_t weapon_count, uint16_t weapon_specials, int16_t attacker_player_idx);

    int16_t __cdecl Expected_Weapon_Damage_(int16_t source_idx, int16_t weapon_slot, int16_t target_idx, int16_t target_type, int16_t range);

    int16_t __cdecl Evaluate_Possible_Move_(int16_t score, int16_t val1, int16_t val2, int16_t move_type, int16_t* p_count, int16_t* best_moves_val1, int16_t* best_moves_val2, int16_t best_score);

    int16_t __cdecl Convert_Ordinal_To_Msl_Type_(int16_t ordinal);
}
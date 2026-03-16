#pragma once
#include <cstdint>

namespace CMBTAI {
    int8_t __cdecl Get_Player_Mode_(int16_t player_idx);

    int16_t __cdecl Retreat_Check_(int16_t player_idx);

    inline int16_t __cdecl Get_Effective_Weapon_Damage_(int16_t ship_idx, int16_t slot_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044f860});
        return fn(ship_idx, slot_idx);
    }

    int16_t __cdecl Get_Total_Hits_(int16_t ship_idx);

    inline int16_t __cdecl Get_Effective_Missile_Strength_(int16_t player_idx, int16_t* out_val) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044F010});
        return fn(player_idx, out_val);
    }

    int16_t __cdecl Get_Long_Range_Beam_Strength_(int16_t player_idx);

    int16_t __cdecl Get_Effective_Beam_Strength_(int16_t player_idx);

    void __cdecl Toggle_Auto_Combat_Flags_();

    void __cdecl Do_Auto_Ship_Turn_(void);

    int16_t __cdecl Is_Ship_Out_Of_Ammo_(int16_t ship_idx);

    int16_t __cdecl Check_For_Launched_Fighters_(int16_t source_unit_idx);

    void __cdecl Release_Smallest_Ship_From_Stasis_(int16_t owner_idx);

    int16_t __cdecl Ai_Stasis_Check_(int16_t player_idx);

    int16_t __cdecl Select_Ship_To_Target_(int16_t ship_idx);

    int16_t __cdecl Target_Ship_Value_(int16_t attacker_idx, int16_t target_idx, int16_t distance_flag);

    inline int16_t __cdecl Missile_Overkill_Check_(int16_t target) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044d0b0});
        return fn(target);
    }

    int16_t __cdecl Choose_Target_(int16_t ship_idx);

    int8_t __cdecl Combat_Ship_Class_(int16_t ship_idx, int16_t target_idx);

    int16_t __cdecl Auto_Move_Ship_(int16_t ship_param, int16_t target_idx);

    bool __cdecl Does_Ship_Have_Missiles_Incoming_(int16_t ship_idx);
    
    // TODO: Figure out why decompiled version does not select any targets
    inline int16_t __cdecl Get_Current_Target_(int16_t ship_idx, int16_t param_2) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x0044ef10 })(ship_idx, param_2);
    }

    int16_t __cdecl Boarding_Action_Type_(int16_t attacker_idx, int16_t target_idx);

    void __cdecl Resolve_Ai_Boarding_(int16_t attacker_idx, int16_t target_idx, int16_t action_type);

    int16_t __cdecl Is_Ship_Targeted_By_Missile_(int16_t ship_idx);

    void __cdecl Crystal_Control_(int16_t ship_idx);

    inline int8_t __cdecl Ai_Self_Destruct_Check_(int16_t ship_idx) {
        using Fn = int8_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0044bc60})(ship_idx);
    }

    int16_t __cdecl Range_To_Ship_(int16_t ship1_idx, int16_t ship2_idx);

    int16_t __cdecl Range_To_XY_(int16_t ship_idx, int16_t target_x_px, int16_t target_y_px);

    int16_t __cdecl Range_To_Missile_(int16_t unit_idx, int16_t missile_idx);

    int16_t __cdecl Will_Missile_Do_Damage_(int16_t target_idx, int16_t missile_idx);

    bool __cdecl Safe_To_Fire_Sphere_Weapon_(int16_t source_ship_idx);

    inline int16_t __cdecl Get_Current_Ship_Target_(int16_t ship_idx, int32_t* score_out) {
        using Fn = int16_t(__cdecl*)(int16_t, int32_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0044ee10})(ship_idx, score_out);
    }

    inline int16_t __cdecl Get_Current_Missile_Target_(int16_t ship_idx, int16_t last_target, int32_t* score_out) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int32_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0044eaf0})(ship_idx, last_target, score_out);
    }

    inline int32_t __cdecl Expected_Beam_Bomb_Damage_(int16_t ship_idx, int16_t slot_idx, int16_t target_idx, int16_t shots, int16_t range_tactic, int16_t is_missile) {
        using Fn = int32_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0044CD80})(ship_idx, slot_idx, target_idx, shots, range_tactic, is_missile);
    }

    inline int16_t __cdecl Expected_Missile_Damage_(int16_t target_idx, uint16_t weapon_id, int16_t shots, uint16_t specials, int16_t owner) {
        using Fn = int16_t(__cdecl*)(int16_t, uint16_t, int16_t, uint16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0044CB10})(target_idx, weapon_id, shots, specials, owner);
    }

    int16_t __cdecl Expected_Weapon_Damage_(int16_t source_idx, int16_t weapon_slot, int16_t target_idx, int16_t target_type, int16_t range);

    int16_t __cdecl Evaluate_Possible_Move_(int16_t score, int16_t val1, int16_t val2, int16_t move_type, int16_t* p_count, int16_t* best_moves_val1, int16_t* best_moves_val2, int16_t best_score);
}
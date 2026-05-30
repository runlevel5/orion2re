#pragma once
#include <cstdint>

namespace COMBAT {
    int16_t __cdecl Strategic_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, char* summary, char* details);

    void __cdecl Get_Leader_Combat_Bonuses_(int16_t* ship_indices, int16_t count, int16_t* helmsman_max, 
                                            int16_t* ordnance_max, int16_t* engineer_max, 
                                            int16_t* weaponry_max, int16_t* lore_max);

    void __cdecl Qload_Ships_(int16_t* ship_indices, int16_t num_ships, int16_t param_3, int16_t param_4, int16_t param_5, int16_t param_6);

    int16_t __cdecl Get_Colony_Hits_();

    void __cdecl QLoad_Antaran_Defense_Fleet_();

    void __cdecl QLoad_Combat_Antaran_Ship_(int16_t q_idx, int16_t size_idx);

    int16_t __cdecl QGet_Target_(int16_t ship_idx);

    int16_t __cdecl QGet_Target_SC_(int16_t attacker_idx);

    void __cdecl Stellar_Converter_Attack_(int16_t target_idx);

    void __cdecl Beam_Attack_(int16_t ship_idx, int16_t target_idx);

    void __cdecl Missile_Attack_(int16_t attacker_idx, int16_t target_idx);

    void __cdecl Strat_Special_Attack_(int16_t attacker_idx, int16_t target_idx);

    void __cdecl Strat_Bomb_Attack_(int16_t attacker_idx);

    void __cdecl Resolve_Strat_Colony_Damage_();

    void __cdecl Build_End_Of_Combat_Message_(char* result_msg, char* stats_msg, int16_t star_idx, int16_t colony_damage_flags, int16_t outcome_type);

    void __cdecl Message_Box_Titled_(char* summary, char* details);

    void __cdecl Apply_Damage_To_Qship_(int16_t qship_idx, int16_t damage);

    int16_t __cdecl Combat_Satelite_Check_(uint8_t type);

    extern const char s_Crystal_00557e24[8];   // "Crystal"
    extern const char s_Eel_00557e2c[4];       // "Eel"
    extern const char s_Hydra_00557e30[8];     // "Hydra"
    extern const char s_Dragon_00557e38[8];    // "Dragon"
    extern const char s_Amoeba_00557e40[8];    // "Amoeba"
    extern const char s_Guardian_00557e48[12]; // "Guardian"
    extern const char s_Antaran_00557e54[8];   // "Antaran"

    void __cdecl Get_Non_Player_Name_(int16_t param_1, char* param_2);

    int16_t __cdecl Strategic_Bombardment_(int16_t* attacker_ship_indices, int16_t attacker_count, int16_t colony_id);

    int __cdecl qsort_ship_array_by_size_(const void* a, const void* b);

    void __cdecl Tactical_Bombardment_(int16_t* ship_array, int16_t ship_count, s_combat_bomb_result* bomb_result);

    void __cdecl Bombardment_(s_combat_bomb_result* bomb_result);
}
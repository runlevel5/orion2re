#pragma once
#include <cstdint>

struct s_ship_data;
struct s_ship_design;

namespace INITSHIP {
    void __cdecl Repair_Ships_At_Colonies_();

    void __cdecl Repair_Ship_Full_(int16_t ship_idx);

    void __cdecl Init_Ship_Designs_(int16_t player_idx);
    
    void __cdecl Get_Ship_Combat_Bonuses_(int16_t ship_idx, int16_t* ship_defense, int16_t* ship_missile_defense, int16_t* ship_attack);

    int16_t __cdecl Best_Ship_Fuel_(int16_t player_idx);

    void __cdecl Update_Player_Ship_Designs_(int16_t player_idx);

    void __cdecl Update_Player_Strategic_Ship_Designs_(int16_t player_idx);

    void __cdecl Update_Automatic_Design_Equipment_(int16_t player_idx);

    void __cdecl Update_Equipment_On_Ships_Being_Built_(int16_t player_idx);

    int16_t __cdecl Best_Warp_Drive_(int16_t player_idx);

    int16_t __cdecl Calc_Player_FTL_Speed_(int16_t player_idx);

    int16_t __cdecl Best_Armor_(int16_t player_idx);

    int16_t __cdecl Best_Computer_(int16_t player_idx);

    int16_t __cdecl Best_Ship_Shield_(int16_t player_idx);

    int16_t __cdecl Best_Shield_Special_(int16_t player_idx);

    int16_t __cdecl Best_Beam_Weapon_(int16_t player_idx);

    int16_t __cdecl Best_Point_Defense_Weapon_By_Size_(int16_t player_idx);

    int16_t __cdecl Point_Defense_Damage_Per_Space_(int16_t weapon_idx, int16_t player_idx);

    int16_t __cdecl Best_Missile_Weapon_(int16_t player_idx);

    int16_t __cdecl Best_Missile_Weapon_NT_(int16_t player_idx);

    int16_t __cdecl Best_Strat_Special_Weapon_(int16_t player_idx, int16_t size);

    int16_t __cdecl Get_FTL_Speed_(int16_t player_idx, int16_t ftl_type);

    int16_t __cdecl Get_Design_Structure_(s_ship_design design);

    int16_t __cdecl Get_Structure_(int16_t hull_index, bool is_fort, int16_t armor_tech);

    int16_t __cdecl Get_Ship_Structure_(int16_t ship_idx);

    int16_t __cdecl Get_Ship_Armor_Hits_(int16_t ship_idx);

    void __cdecl Auto_Strategic_Design_Ship_(int16_t player_idx, int16_t design_idx, s_ship_design* design);

    void __cdecl Load_Small_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Medium_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Large_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Huge_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Titan_Antaran_Design_(s_ship_design* design);

    void __cdecl Load_Antaran_Ship_Design_(s_ship_design* design, int16_t hull_size);

    void __cdecl Load_Small_Antaran_Combat_Ship_(int16_t ship_idx);

    void __cdecl Load_Medium_Antaran_Combat_Ship_(int16_t ship_idx);

    void __cdecl Load_Large_Antaran_Combat_Ship_(int16_t ship_idx);

    void __cdecl Load_Huge_Antaran_Combat_Ship_(int16_t ship_idx);

    void __cdecl Load_Titan_Antaran_Combat_Ship_(int16_t ship_idx);

    void __cdecl Load_Guardian_Ship_Design_(s_ship_design* design);

    void __cdecl Load_Amoeba_Ship_Design_(s_ship_design* design, int16_t is_large);

    void __cdecl Load_Crystal_Ship_Design_(s_ship_design* design, int16_t variant);

    void __cdecl Load_Dragon_Ship_Design_(s_ship_design* design, int16_t is_monster);

    void __cdecl Load_Eel_Ship_Design_(s_ship_design* design, int16_t is_greater_eel);

    void __cdecl Load_Hydra_Ship_Design_(s_ship_design* design, int16_t is_monster);

    void __cdecl Load_Loknar_Ship_Design_(s_ship_design* design);

    inline char* s_Avenger_005576c4 = reinterpret_cast<char*>(std::uintptr_t{0x005576C4});

    void __cdecl Load_Colony_Ship_Design_(int16_t player_idx, s_ship_design* design);

    inline char* s_Colony_Ship_Localized = reinterpret_cast<char*>(std::uintptr_t{0x00558E28});

    void __cdecl Load_Doom_Star_Ship_Design_(int16_t owner_idx, s_ship_design* design);

    void __cdecl Update_Strategic_Design_Weapons_Specials_(s_ship_design* design, int16_t player_idx, int16_t design_idx);

    void __cdecl Update_Strategic_Ship_(int16_t ship_idx);

    int16_t __cdecl Strat_Missile_Weapon_Count_(int16_t hull_idx);

    int16_t __cdecl Strat_Special_Weapon_Count_(int16_t hull_idx);

    int16_t __cdecl Strat_Beam_Weapon_Count_(int16_t player_idx, int16_t hull_idx);

    int16_t __cdecl Strat_Bomb_Weapon_Count_(int16_t hull_idx);

    int16_t __cdecl Best_Beam_To_Hit_Special_(int16_t player_idx);

    int16_t __cdecl Best_Beam_Damage_Special_(int16_t player_idx);

    int16_t __cdecl Best_Beam_Defense_Special_(int16_t player_idx);

    int16_t __cdecl Best_Missile_Defense_Special_(int16_t player_idx);

    int16_t __cdecl Best_Point_Defense_Weapon_(int16_t player_idx);

    int16_t __cdecl Best_Bomb_Weapon_(int16_t player_idx);

    int16_t __cdecl Cost_Of_Strategic_Design_(int16_t govt_type, struct s_ship_design* design);

    void __cdecl Get_Design_Combat_Bonuses_(s_ship_design design, int16_t* attack_bonus_out, int16_t* defense_rating_out, int16_t* missile_defense_out);

    void __cdecl Load_Transport_Ship_Design_(int16_t player_idx, s_ship_design* design);

    inline const char* const s_Transport_00558e88 =
        reinterpret_cast<const char*>(std::uintptr_t{0x00558E88});
}
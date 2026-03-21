#pragma once
#include <cstdint>

namespace aidesign {
    inline int16_t (&_min_weapon_count_table)[8] =
        *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00556F60});

    inline s_spcl (&_bio_weapon_spcl)[5] = *reinterpret_cast<s_spcl(*)[5]>(std::uintptr_t{0x00556DC0});
    inline s_spcl (&_capture_spcl)[5] = *reinterpret_cast<s_spcl(*)[5]>(std::uintptr_t{0x00556DD8});
    inline s_spcl (&_cloaking_spcl)[3] = *reinterpret_cast<s_spcl(*)[3]>(std::uintptr_t{0x00556DF0});
    inline s_spcl (&_beam_defense_spcl)[4] = *reinterpret_cast<s_spcl(*)[4]>(std::uintptr_t{0x00556E00});
    inline s_spcl (&_missile_defense_spcl)[8] = *reinterpret_cast<s_spcl(*)[8]>(std::uintptr_t{0x00556E10});
    inline s_spcl (&_armor_spcl)[3] = *reinterpret_cast<s_spcl(*)[3]>(std::uintptr_t{0x00556E30});
    inline s_spcl (&_shield_spcl)[3] = *reinterpret_cast<s_spcl(*)[3]>(std::uintptr_t{0x00556E40});

    inline s_aidesign_ship_design (&_ship_design_tables)[7][6] = *reinterpret_cast<s_aidesign_ship_design(*)[7][6]>(std::uintptr_t{0x00556F70});
    inline s_spcl (&_beam_offense_spcl)[7] = *reinterpret_cast<s_spcl(*)[7]>(std::uintptr_t{0x00556E50});
    inline s_spcl (&_missile_offense_spcl)[2] = *reinterpret_cast<s_spcl(*)[2]>(std::uintptr_t{0x00556E70});
    inline s_spcl (&_special_weapons_spcl)[5] = *reinterpret_cast<s_spcl(*)[5]>(std::uintptr_t{0x00556E78});
    inline s_spcl (&_special_device_spcl)[12] = *reinterpret_cast<s_spcl(*)[12]>(std::uintptr_t{0x00556E90});
    inline s_spcl (&_defense_specials_f_spcl)[20] = *reinterpret_cast<s_spcl(*)[20]>(std::uintptr_t{0x00556EC0});
    inline s_spcl (&_defense_specials_t_spcl)[20] = *reinterpret_cast<s_spcl(*)[20]>(std::uintptr_t{0x00556F10});

    int16_t __cdecl Add_Specials_To_Design_(struct s_spcl* list, int count, int16_t player_idx, struct s_ship_design* design, int16_t space, int16_t* weapon_index_ptr);

    int16_t __cdecl Add_Theme_Special_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t space, int16_t* weapon_index_ptr);

    int16_t __cdecl Get_Ship_Class_(int16_t player_idx);

    int16_t __cdecl Fighters_Available_(int16_t player_idx);

    int16_t __cdecl Special_Weapons_Available_(int16_t player_idx);

    int16_t __cdecl Special_Devices_Available_(int16_t player_idx);

    uint16_t __cdecl Get_Competition_Player_Tech_Values_(uint32_t player_idx, uint32_t tech_type);

    bool __cdecl Human_Player_Best_Weapons_Are_Beams_();

    int16_t __cdecl Add_Fighters_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_index_ptr, int16_t space);

    int16_t __cdecl Add_Beam_Weapon_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_index_ptr, int16_t space, int16_t weapon_class);

    void __cdecl Determine_Beam_Weapon_Modifications_(int16_t player_idx, int16_t hull_size, int16_t* type0, int16_t* spec0, int16_t* arc0, int16_t* type1, int16_t* spec1, int16_t* arc1, int16_t* type2, int16_t* spec2, int16_t* arc2);

    void __cdecl Determine_Missile_Weapon_Modifications_(
        int16_t player_idx,
        uint16_t* out_weapon_id,
        uint16_t* out_weapon_mods,
        int16_t weapon_count_table_idx,
        int16_t available_space,
        int16_t* out_weapon_count
    );

    int16_t __cdecl Add_Missile_Weapon_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_index_ptr, int16_t space, int16_t ammo);

    void __cdecl Determine_Bomb_Weapon_(int16_t player_idx, int16_t* bomb_type);

    int16_t __cdecl Add_Bomb_Weapon_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_slot_idx, int16_t space_available);

    void __cdecl Auto_Design_Ship_(int16_t player_idx, s_ship_design* design, int16_t hull_idx);

    int16_t __cdecl Special_Legal_For_Design_(s_ship_design* design, int16_t tech_id);
}
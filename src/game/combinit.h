#pragma once

namespace COMBINIT {
    inline int8_t& radiation_shield_hits = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F04D});
    inline int8_t& flux_shield_hits = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F050});
    inline int8_t& barrier_shield_hits = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F053});
    inline const char* s_Antaran_Star_Fortress_005558D4 = reinterpret_cast<const char*>(std::uintptr_t{0x005558D4});
    inline int16_t& _antaran_star_fortress_structure_hp = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005558C0});
    inline int16_t& _antaran_star_fortress_armor_hp = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005558BE});
    inline int8_t& _antaran_star_fortress_marines = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005558BC});
    inline int16_t& _antaran_star_fortress_size = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005558BA});

    void __cdecl Load_Combat_Colony_Picture_(int16_t colony_idx);

    inline int8_t* s__00579491 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579491});
    inline int8_t* s__00579492 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579492});
    inline int8_t* s__00579493 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579493});
    inline int8_t* s__00579591 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579591});
    inline int8_t* s__00579592 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579592});
    inline int8_t* s__00579593 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579593});
    inline int8_t* s__00579611 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579611});
    inline int8_t* s__00579612 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579612});
    inline int8_t* s__00579613 = reinterpret_cast<int8_t*>(std::uintptr_t{0x00579613});

    inline char* s_combat_lbx_00551244 = reinterpret_cast<char*>(std::uintptr_t{0x00551244});
    inline const char* s_monster_lbx_00551250 = reinterpret_cast<const char*>(std::uintptr_t{0x00551250});
    inline const char* s_cmbtshp_lbx_0055125c = reinterpret_cast<const char*>(std::uintptr_t{0x0055125C});
    inline char* s_cmbtmisl_lbx_00551234 = reinterpret_cast<char*>(std::uintptr_t{0x00551234});
    inline char* s_cmbtsfx_lbx_00551228 = reinterpret_cast<char*>(std::uintptr_t{0x00551228});
    inline char* s_sphersfx_lbx_00551218 = reinterpret_cast<char*>(std::uintptr_t{0x00551218});

    void __cdecl Load_Display_Combat_Ship_(int16_t ship_idx);

    void __cdecl Load_Combat_Ship_(int16_t ship_idx, int16_t combat_idx);
    int16_t __cdecl Calc_Combat_Shield_Max_(int16_t ship_idx);
    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t ship_idx, int16_t special_id);
    int8_t __cdecl Colony_Has_Defenses_(int16_t colony_idx);

    void __cdecl Load_Antaran_Defense_Fleet_();

    void __cdecl Load_Combat_Antaran_Ship_(int16_t ship_idx, int16_t ship_size);

    void __cdecl Load_Antaran_Star_Fortress_();
    void __cdecl Load_Antaran_Planet_();
    void __cdecl Load_Colony_Defense_(int16_t colony_idx);

    void __cdecl Load_Tactical_Colony_(int16_t colony_idx, int16_t load_picture);

    int16_t __cdecl Type_Of_Space_Station_(int16_t colony_idx);

    void __cdecl Load_Combat_Satelite_(int16_t station_type);

    void __cdecl Load_Combat_Ship_Palette_(int16_t plr);

    void __cdecl Load_Individual_Ship_Pictures_(int16_t player_idx);

    void __cdecl Init_Special_Devices_(int16_t combat_ship_idx);

    void __cdecl Deploy_Ships_();

    int16_t __cdecl Ship_Size_Offset_(int16_t size);

    int8_t __cdecl Ship_Can_Deploy_At_(int16_t x, int16_t y, int16_t ship_idx);

    int __cdecl qsort_combat_data_by_size_(const void* a, const void* b);

    void __cdecl Snap_Center_Combat_Screen_(int16_t target_x, int16_t target_y);

    void __cdecl Scroll_Combat_Screen_();

    void __cdecl Plasma_Web_Damage_(int16_t owner_idx);
    void __cdecl Increment_Bhg_(int16_t player_idx);
    void __cdecl Recharge_Shields_();
    void __cdecl Repair_All_Combat_Ships_();
    void __cdecl End_Of_Turn_Bookeeping_();
    void __cdecl End_Of_Combat_(int16_t param_1, int16_t param_2, int16_t param_3);

    int16_t __cdecl Tactical_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, int16_t retreat_flag);

    void __cdecl Unload_Combat_Ship_(int16_t combat_ship_idx);

    void __cdecl Apply_Bio_Weapon_Penalty_(int16_t player_idx, int16_t param_2, int16_t param_3);

    inline uint8_t& _real_attack_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8BBC});
    int __cdecl qsort_ship_array_by_size_(const void* param_1, const void* param_2);

    void __cdecl Ship_Center_Offsets_(int16_t ship_idx, int16_t* out_x, int16_t* out_y);
}
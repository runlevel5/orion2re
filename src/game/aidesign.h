#pragma once
#include <cstdint>

namespace aidesign {
    inline struct s_spcl* _bio_weapon_spcl = reinterpret_cast<struct s_spcl*>(std::uintptr_t{0x00556DC0});
    inline struct s_spcl* _capture_spcl = reinterpret_cast<struct s_spcl*>(std::uintptr_t{0x00556DD8});
    inline struct s_spcl* _cloaking_spcl = reinterpret_cast<struct s_spcl*>(std::uintptr_t{0x00556DF0});
    inline struct s_spcl* _beam_defense_spcl = reinterpret_cast<struct s_spcl*>(std::uintptr_t{0x00556E00});
    inline struct s_spcl* _missile_defense_spcl = reinterpret_cast<struct s_spcl*>(std::uintptr_t{0x00556E10});
    inline struct s_spcl* _armor_spcl = reinterpret_cast<struct s_spcl*>(std::uintptr_t{0x00556E30});
    inline struct s_spcl* _shield_spcl = reinterpret_cast<struct s_spcl*>(std::uintptr_t{0x00556E40});

    inline s_aidesign_ship_design (&_ship_design_tables)[7][6] = *reinterpret_cast<s_aidesign_ship_design(*)[7][6]>(std::uintptr_t{0x00556F70});
    inline s_spcl (&_beam_offense_spcl)[7] = *reinterpret_cast<s_spcl(*)[7]>(std::uintptr_t{0x00556E50});
    inline s_spcl (&_missile_offense_spcl)[2] = *reinterpret_cast<s_spcl(*)[2]>(std::uintptr_t{0x00556E70});
    inline s_spcl (&_special_weapons_spcl)[5] = *reinterpret_cast<s_spcl(*)[5]>(std::uintptr_t{0x00556E78});
    inline s_spcl (&_special_device_spcl)[12] = *reinterpret_cast<s_spcl(*)[12]>(std::uintptr_t{0x00556E90});
    inline s_spcl (&_defense_specials_f_spcl)[20] = *reinterpret_cast<s_spcl(*)[20]>(std::uintptr_t{0x00556EC0});
    inline s_spcl (&_defense_specials_t_spcl)[20] = *reinterpret_cast<s_spcl(*)[20]>(std::uintptr_t{0x00556F10});

    inline int16_t __cdecl Add_Specials_To_Design_(struct s_spcl* list, int count, int16_t player_idx, struct s_ship_design* design, int16_t space, int16_t* weapon_index_ptr) {
        using Fn = int16_t(__cdecl*)(struct s_spcl*, int, int16_t, struct s_ship_design*, int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046C4F0});
        return fn(list, count, player_idx, design, space, weapon_index_ptr);
    }

    inline int16_t __cdecl Add_Theme_Special_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t space, int16_t* weapon_index_ptr) {
        using Fn = int16_t(__cdecl*)(int16_t, struct s_ship_design*, int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0046c7f0 });
        return fn(player_idx, design, space, weapon_index_ptr);
    }

    inline int16_t __cdecl Get_Ship_Class_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0046BE70})(player_idx);
    }

    inline bool __cdecl Human_Player_Best_Weapons_Are_Beams_() {
        using Fn = bool(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x0046BC90})();
    }

    inline int16_t __cdecl Add_Fighters_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_index_ptr, int16_t space) {
        using Fn = int16_t(__cdecl*)(int16_t, struct s_ship_design*, int16_t*, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0046C9E0})(player_idx, design, weapon_index_ptr, space);
    }

    inline int16_t __cdecl Add_Beam_Weapon_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_index_ptr, int16_t space, int16_t weapon_class) {
        using Fn = int16_t(__cdecl*)(int16_t, struct s_ship_design*, int16_t*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0046DF10})(player_idx, design, weapon_index_ptr, space, weapon_class);
    }

    inline int16_t __cdecl Add_Missile_Weapon_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_index_ptr, int16_t space, int16_t ammo) {
        using Fn = int16_t(__cdecl*)(int16_t, struct s_ship_design*, int16_t*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0046D010})(player_idx, design, weapon_index_ptr, space, ammo);
    }

    inline int16_t __cdecl Add_Bomb_Weapon_To_Design_(int16_t player_idx, struct s_ship_design* design, int16_t* weapon_index_ptr, int16_t space) {
        using Fn = int16_t(__cdecl*)(int16_t, struct s_ship_design*, int16_t*, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0046D3B0})(player_idx, design, weapon_index_ptr, space);
    }

    void __cdecl Auto_Design_Ship_(int16_t player_idx, s_ship_design* design, int16_t hull_idx);

    int16_t __cdecl Cost_Reduction_For_Govt_Hull_Type(int16_t govt_type, struct s_ship_design* design);
}
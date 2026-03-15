#pragma once

#include <cstdint>

namespace CMBTFIR2 {
    inline int16_t* _artemis_net_chance = reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055A538 });

    int16_t __cdecl Helmsman_Bonus_(int16_t leader_idx);

    int16_t __cdecl Weaponry_Bonus_(int16_t leader_idx);

    int16_t __cdecl Engineer_Bonus_(int16_t leader_idx);

    int16_t __cdecl Galactic_Lore_Bonus_(int16_t leader_idx);

    void __cdecl Set_Combat_Ordnance_Values_();

    int16_t __cdecl Leader_Ordnance_Bonus_(int16_t leader_idx);

    int16_t __cdecl Get_Fleet_Ordnance_Bonus_(int16_t owner);

    extern int16_t* _artemis_net_chance;
    bool __cdecl Apply_Combat_Artemis_Net_(char strategic);

    int16_t __cdecl Used_Teleporter_(int16_t ship_idx);

    void __cdecl Set_Teleporter_Used_(int16_t ship_idx);

    void __cdecl Set_Boarded_Flag_(int16_t ship_idx);

    inline void __cdecl Resolve_Raid_(int16_t ship, int16_t a, int16_t* b) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e7120})(ship, a, b);
    }

    inline void __cdecl Resolve_Capture_(int16_t ship, int16_t a, int16_t b, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e7ea0})(ship, a, b, x, y);
    }

    int16_t __cdecl Already_Boarded_(int16_t param_1);

    void __cdecl Set_Combat_Ship_Done_(int16_t ship_idx);

    int16_t __cdecl Max_Defensive_Fire_Range_(int16_t ship_idx);

    void __cdecl Init_Internal_Space_(s_combat_data* ship);

    short __cdecl Max_Total_Internal_Space_(short ship_idx);

    int16_t __cdecl Crew_Defense_Bonus_(int16_t combat_idx);

    short __cdecl Fleet_Defensive_Bonus_(short player_idx, short mode, short* bonus_val_ptr);

    short __cdecl Fleet_Offensive_Bonus_(short player_idx);

    inline int16_t __cdecl Get_Fleet_Pilot_Bonus_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e5b20});
        return fn(player_idx);
    }

    int16_t __cdecl Ship_Specials_Defensive_Bonus_(int16_t ship_idx, int16_t target_type, int16_t* field_bonus_out);

    int16_t __cdecl Ship_Specials_Offensive_Bonus_(int16_t ship_idx);

    int16_t __cdecl Defensive_Combat_Bonus_(int16_t combat_unit_idx, int16_t target_type);

    int16_t __cdecl Offensive_Combat_Bonus_(int16_t ship_idx);

    int16_t __cdecl Computer_Bonus_(int16_t ship_idx);

    int16_t __cdecl Crew_Attack_Bonus_(int16_t ship_idx);

    int16_t __cdecl Get_Fleet_Engineer_Bonus_(int16_t player_idx);

    void __cdecl Repair_Combat_Ship_(int16_t ship_idx, int16_t amount);

    void __cdecl Clear_Internal_Space_(s_combat_data* combat_data);

    int16_t __cdecl Being_Captured_(int16_t ship_idx);

    void __cdecl Set_Capturing_Flag_(int16_t unit_idx);

    inline int16_t __cdecl Init_Boarding_Action_(int16_t ship_idx, int16_t* marines_sent) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004E70C0});
        return fn(ship_idx, marines_sent);
    }

    void __cdecl Defensive_Fire_Check_(int16_t target_idx, int16_t target_x, int16_t target_y, int16_t dest_x, int16_t dest_y);

    void __cdecl Final_Fire_(int16_t ship_idx, int16_t target_x, int16_t target_y, int16_t missile_idx);

    void __cdecl Kill_Crew_(int16_t combat_data_idx, int16_t kill_count);

    inline void __cdecl Select_Internal_(
        int16_t target_ship_idx,
        int16_t* out_sys_index,
        int16_t* out_count,
        int16_t param_8,
        int16_t armor_piercing_flag,
        int16_t ignore_armor_flag,
        int16_t param_9
    ) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e65d0});
        fn(target_ship_idx, out_sys_index, out_count, param_8, armor_piercing_flag, ignore_armor_flag, param_9);
    }

    inline void __cdecl Apply_Internal_Damage_(
        int16_t target_ship_idx,
        int16_t* remaining_damage,
        int16_t sys_index,
        int16_t count,
        int16_t flag
    ) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e4e60});
        fn(target_ship_idx, remaining_damage, sys_index, count, flag);
    }
}
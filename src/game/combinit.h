#pragma once

namespace COMBINIT {
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
    inline char* s_cmbtmisl_lbx_00551234 = reinterpret_cast<char*>(std::uintptr_t{0x00551234});
    inline char* s_cmbtsfx_lbx_00551228 = reinterpret_cast<char*>(std::uintptr_t{0x00551228});
    inline char* s_sphersfx_lbx_00551218 = reinterpret_cast<char*>(std::uintptr_t{0x00551218});

    inline void __cdecl Load_Display_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040f660});
        fn(ship_idx);
    }

    void __cdecl Load_Combat_Ship_(int16_t ship_idx, int16_t combat_idx);
    int16_t __cdecl Calc_Combat_Shield_Max_(int16_t ship_idx);
    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t ship_idx, int16_t special_id);
    int8_t __cdecl Colony_Has_Defenses_(int16_t colony_idx);
    inline void* __cdecl Allocate_Glass_Remap_Next_Block_(void* seg) {
        using Fn = void*(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b6f0});
        return fn(seg);
    }
    inline void __cdecl Save_Glass_Remap_Block_(void* block) {
        using Fn = void(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b710});
        fn(block);
    }
    inline void __cdecl Load_Antaran_Defense_Fleet_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040fcf0});
        fn();
    }
    inline void __cdecl Load_Antaran_Planet_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c9b0});
        fn();
    }
    inline void __cdecl Load_Colony_Defense_(int16_t col) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040cb30});
        fn(col);
    }
    inline uint8_t __cdecl Does_Combat_Planet_Have_Defenses_() {
        using Fn = uint8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ae1e0});
        return fn();
    }
    inline void __cdecl Load_Combat_Ship_Palette_(int16_t plr) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c030});
        fn(plr);
    }
    inline void __cdecl Create_Ship_Darken_Map_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d6380});
        fn();
    }
    inline void __cdecl Load_Combat_Beam_Stuff_(int16_t flag) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420b70});
        fn(flag);
    }
    inline void __cdecl Set_Glass_Remap_Color_(int16_t idx, int16_t r, int16_t g, int16_t b, int16_t c) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b8f0});
        fn(idx, r, g, b, c);
    }
    inline void __cdecl Set_Combat_Ordnance_Values_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e5ba0});
        fn();
    }
    inline void __cdecl Load_Individual_Ship_Pictures_(int16_t plr) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c230});
        fn(plr);
    }
    inline void __cdecl Restore_Glass_Remap_Block_(void* block) {
        using Fn = void(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b740});
        fn(block);
    }
    inline void __cdecl Init_Special_Devices_(int16_t count) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040f500});
        fn(count);
    }

    inline void __cdecl Deploy_Ships_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040af90});
        fn();
    }

    inline void __cdecl Snap_Center_Combat_Screen_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0040bcc0})(x, y);
    }

    inline void __cdecl Scroll_Combat_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0040b9e0})();
    }
    inline int16_t __cdecl Check_For_Winner_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004695d0});
        return fn();
    }
    inline int16_t __cdecl Calc_Current_Speed_(int16_t count) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004693d0});
        return fn(count);
    }
    inline int16_t __cdecl Offensive_Combat_Bonus_(int16_t count) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e6410});
        return fn(count);
    }
    inline int16_t __cdecl Defensive_Combat_Bonus_(int16_t count, int16_t flag) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e5920});
        return fn(count, flag);
    }
    inline void __cdecl Plasma_Web_Damage_(int16_t plr) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c690});
        fn(plr);
    }
    inline void __cdecl Increment_Bhg_(int16_t plr) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c570});
        fn(plr);
    }
    inline void __cdecl End_Of_Turn_Bookeeping_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040c620});
        fn();
    }
    inline void __cdecl End_Of_Combat_(int16_t colony_idx, int16_t pop, int16_t winner) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040d4a0});
        fn(colony_idx, pop, winner);
    }

    int16_t __cdecl Tactical_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, int32_t retreat_flag);
    int __cdecl qsort_ship_array_by_size_(const int16_t* param_1, const int16_t* param_2);
}
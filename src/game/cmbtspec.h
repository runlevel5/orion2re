#pragma once
#include <cstdint>

namespace CMBTSPEC {
    void __cdecl Draw_Cloak_(int16_t ship_idx, int16_t cloak_type);

    inline void __cdecl Draw_Continuing_Tractor_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004A6B20})(ship_idx);
    }

    inline void __cdecl Start_Ship_Engine_SFX_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004a7e20)();
    }

    inline void __cdecl End_Ship_Engine_SFX_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004a7e50)();
    }

    inline void __cdecl Draw_Moving_Tractor_(int16_t tractor_idx, int16_t cur_pixel_x, int16_t cur_pixel_y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(0x004a6cb0)(tractor_idx, cur_pixel_x, cur_pixel_y);
    }

    inline void __cdecl Lightning_Field_(int16_t target, int16_t missile_idx, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a6190})(target, missile_idx, x, y);
    }

    inline void __cdecl Destroy_Ship_FX_(int16_t ship_idx, int16_t p2, int16_t p3, int16_t p4) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004A9400})(ship_idx, p2, p3, p4);
    }

    inline void __cdecl Draw_Stasis_Ball_(int16_t param_1, int16_t size, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a6390});
        fn(param_1, size, x, y);
    }

    inline void __cdecl Draw_Plasma_Web_(int16_t param_1, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a75e0});
        fn(param_1, x, y);
    }

    inline void __cdecl Do_Spherical_SFX_(int16_t ship_idx, int16_t x, int16_t y, int16_t subtype, int16_t shots_left) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a7f70})(ship_idx, x, y, subtype, shots_left);
    }

    inline void __cdecl Draw_Stasis_Beam_(int16_t ship_idx, uint16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t, uint16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a6610})(ship_idx, target_idx);
    }

    inline void __cdecl Anti_Missile_Rockets_(int16_t ship_idx, int16_t x, int16_t y, uint16_t target_idx, int16_t weapon_slot_idx, int16_t range) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, uint16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a5590})(ship_idx, x, y, target_idx, weapon_slot_idx, range);
    }

    inline void __cdecl Gyro_Destablizer_(int16_t ship_idx, uint16_t target_idx, int16_t shots_left) {
        using Fn = void(__cdecl*)(int16_t, uint16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a5a20})(ship_idx, target_idx, shots_left);
    }

    inline void __cdecl Plasma_Web_(int16_t ship_idx, uint16_t target_idx, int16_t weapon_slot_idx) {
        using Fn = void(__cdecl*)(int16_t, uint16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a6eb0})(ship_idx, target_idx, weapon_slot_idx);
    }

    inline void __cdecl Stellar_Converter_(int16_t ship_idx, uint16_t target_idx, int16_t shots_left) {
        using Fn = void(__cdecl*)(int16_t, uint16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a7a20})(ship_idx, target_idx, shots_left);
    }

    inline void __cdecl Tractor_Beam_(int16_t ship_idx, uint16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t, uint16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a73c0})(ship_idx, target_idx);
    }

    inline void __cdecl Draw_Transporter_Bomb_Beam_(int16_t ship_idx, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(0x004a7830)(ship_idx, x, y);
    }
}
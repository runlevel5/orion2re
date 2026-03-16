#pragma once
#include <cstdint>

struct s_animation_header;

namespace BEAMS {
    inline s_animation_header*& _mauler_ball_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00564D64});

    inline void __cdecl Init_Beams_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420ca0});
        fn();
    }

    inline void __cdecl Load_Ship_Burst_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420bc0});
        fn();
    }

    inline void __cdecl Load_Ship_Explosion_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420c50});
        fn();
    }

    inline void __cdecl Load_Shield_Hit_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004230e0});
        fn();
    }

    s_animation_header* __cdecl Reflection_Field_Anims_(int32_t p1, int32_t p2);
    void __cdecl Load_Reflection_Field_Hit_(int8_t should_load);

    void __cdecl Load_Combat_Beam_Stuff_(int8_t reflection_hit_param);

    inline void __cdecl Add_Damage_Indicator_For_Ship_(int32_t dx, int32_t dy, int16_t ship_idx, int32_t flag, int16_t total_damage, int16_t struct_damage) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int16_t, int32_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00422D20});
        fn(dx, dy, ship_idx, flag, total_damage, struct_damage);
    }

    inline void __cdecl Draw_Shield_Hit_Once_Through_(int16_t target, int16_t facing, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00423210})(target, facing, x, y);
    }

    inline int8_t __cdecl Done_Drawing_Damage_Message_Queue_() {
        using Fn = int8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00422a60})();
    }

    inline void __cdecl Draw_Damage_Message_Queue_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004229d0})();
    }

    inline void __cdecl Draw_Damage_Message_Queue_Until_Done_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00422E70});
        fn();
    }

    inline uint8_t __cdecl Draw_Fighter_Beam_(int16_t* beams, int16_t count, int16_t target_x, int16_t target_y, int16_t beam_type, int16_t frame) {
        using Fn = uint8_t(__cdecl*)(int16_t*, int16_t, int16_t, int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00422770})(beams, count, target_x, target_y, beam_type, frame);
    }

    inline void __cdecl Add_Damage_Indicator_For_Missile_(int16_t x, int16_t y, int16_t flag, int16_t damage) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00422DC0})(x, y, flag, damage);
    }

    inline void __cdecl Add_Damage_Indicator_For_Biological_(int16_t x, int16_t y, int16_t is_critical, int16_t damage) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00422e00})(x, y, is_critical, damage);
    }

    inline void __cdecl Beam_SFX_(int16_t source, int16_t sx, int16_t sy, int16_t target, int16_t tx, int16_t ty, s_beam_fx_entry* fx_entries, int16_t flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, s_beam_fx_entry*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00420D60})(source, sx, sy, target, tx, ty, fx_entries, flag);
    }
}
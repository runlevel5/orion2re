#pragma once

namespace INITGAME {
    inline uint8_t& DAT_005974f0 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005974F0 });
    inline uint8_t& DAT_00597662 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597662 });

    void Set_Default_Game_Settings_();

    inline void Init_New_Game_(int16_t difficulty, int16_t galaxy_size, int16_t galaxy_age, int16_t num_players, uint8_t strategic_flag, int16_t starting_level) {
        using Fn = void(*)(int16_t, int16_t, int16_t, int16_t, uint32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00478820 });
        fn(difficulty, galaxy_size, galaxy_age, num_players, strategic_flag, starting_level);
    }

    inline void __stdcall Write_Game_Settings_() {
        using Fn = void(__stdcall*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00478FB0 });
        fn();
    }
}
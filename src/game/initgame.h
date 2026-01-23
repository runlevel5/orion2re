#pragma once

namespace INITGAME {
    inline uint8_t& DAT_005974f0 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005974F0 });
    inline uint8_t& DAT_00597662 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597662 });

    void Set_Default_Game_Settings_();

    void Init_Ships_Array_();

    void Init_Colonies_Array_();

    // Suggested name: Init_Leaders
    inline void __cdecl Init_Leaders_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00479580 })();
    }

    // Suggested name: Init_Players
    inline void __cdecl Init_Players_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00479000 })();
    }

    // Suggested name: Make_System_Monsters_Into_Ships
    inline void __cdecl Make_System_Monsters_Into_Ships_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00478BC0 })();
    }

    // Suggested name: Set_Initial_Colony_Auto_Selects_
    inline void __cdecl Set_Initial_Colony_Auto_Selects_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00478D10 })();
    }

    // Suggested name: FUN_00443c60
    inline void __cdecl FUN_00443c60() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00443C60 })();
    }

    void __cdecl Init_New_Game_(int16_t game_difficulty, int16_t galaxy_size, int32_t galaxy_age, int16_t num_players,
                                int16_t strategic_combat_flag, int16_t starting_civ_level);

    void Write_Game_Settings_();
}
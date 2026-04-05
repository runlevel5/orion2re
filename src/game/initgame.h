#pragma once

namespace INITGAME {
    inline uint8_t& DAT_005974f0 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005974F0 });
    inline uint8_t& DAT_00597662 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597662 });

    void Set_Default_Game_Settings_();

    void Init_Ships_Array_();

    void Init_Colonies_Array_();

    void __cdecl Init_Leaders_();

    void __cdecl Init_Players_();

    void __cdecl Make_System_Monsters_Into_Ships_();

    void __cdecl Init_Colony_(int colony_idx, int planet_idx, int player_num);

    void __cdecl Set_Initial_Colony_Auto_Selects_();

    void __cdecl Init_New_Game_(int16_t game_difficulty, int16_t galaxy_size, int16_t galaxy_age, int16_t num_players,
                                int16_t strategic_combat_flag, int16_t starting_civ_level);

    void Write_Game_Settings_();
}
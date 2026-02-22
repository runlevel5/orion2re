#pragma once
#include <cstdint>

namespace AIBUILD {
    inline int8_t (&_difficulty_intel_bias)[8] =
        *reinterpret_cast<int8_t (*)[8]>(std::uintptr_t{0x0054F060});
    inline int8_t* _player_ship_size_table = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F068});
    inline int (&_player_ship_sizes)[8][14] = *reinterpret_cast<int (*)[8][14]>(std::uintptr_t{0x00566088});
    inline int (&_player_total_strength_vs_colonies)[8] = *reinterpret_cast<int (*)[8]>(std::uintptr_t{0x00566248});
    inline int8_t (&_player_n_transports)[8][2] = *reinterpret_cast<int8_t (*)[8][2]>(std::uintptr_t{0x00566268});
    inline int8_t (&_player_n_freighter_groups_wanted)[8] =
        *reinterpret_cast<int8_t (*)[8]>(std::uintptr_t{0x00566278});
    inline int8_t (&_player_n_spies)[8][2] = *reinterpret_cast<int8_t (*)[8][2]>(std::uintptr_t{0x00566280});
    inline int8_t (&_player_n_colony_ships)[8][3] = *reinterpret_cast<int8_t (*)[8][3]>(std::uintptr_t{0x00566290});
    inline int (&_player_total_strength_vs_ships)[8][3] = *reinterpret_cast<int (*)[8][3]>(std::uintptr_t{0x005A63F0});

    int16_t __cdecl Refit_Cost_(int16_t player_idx, struct s_ship_design* old_design, struct s_ship_design* new_design);

    int16_t __cdecl Mean_Ship_Strength_(struct s_ship_data* ship);

    void __cdecl Compute_Needs_(int16_t player_idx);

    void __cdecl Compute_Empire_Building_Needs_();
    int16_t __cdecl Player_War_Level_(int16_t player_idx);
    void __cdecl Compute_Colony_Ship_Need_(int16_t player_num);
    void __cdecl Compute_Freighter_Need_(int16_t player_idx);
}
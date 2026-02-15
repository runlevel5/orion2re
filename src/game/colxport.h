#pragma once
#include <cstdint>
#include "types.h"

namespace COLXPORT {
    inline uint8_t& _cheezy_hack_col_count_state =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005560C8});

    inline int16_t (&_cheezy_hack_col_count)[8] =
        *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00564DE0});

    short __cdecl N_Colonies_(short player_idx);

    inline int16_t*& _g_colony_list_ptr = *reinterpret_cast<int16_t**>(std::uintptr_t{0x005A7B74});

    inline void __cdecl Build_Global_Colony_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042AD70});
        fn();
    }

    inline void __cdecl Load_Xport_Anims_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0042ACB0})();
    }
}
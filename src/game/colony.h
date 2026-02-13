#pragma once
#include <cstdint>

namespace COLONY {
    inline void __cdecl Init_Colonies_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004dbfd0});
        fn();
    }

    inline void __cdecl Colony_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004dfc10})();
    }

    inline int16_t& _orbit_temp =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B22});

    inline uint8_t& _using_colony_screen_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E24});

    inline uint8_t& _full_draw = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E18});
    inline int32_t& _colony_handle = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595E50});
    inline int32_t& _planet_handle = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595FD8});
    inline int32_t& _star_handle = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595E58});
    inline int32_t& _orbit_n = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595E78});
    inline uint8_t& _cheats = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559E1C});
    inline int32_t& _sys_disp_star_handle = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00595FD4});

    inline void __cdecl Load_Colony_Screen_Seg_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004DC8D0})();
    }
    inline void __cdecl Load_Planet_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004DCA30})();
    }
    inline void __cdecl Update_Colony_Palette_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004DF780})();
    }
    inline void __cdecl Make_Bldg_Array_For_Colony_(int32_t colony_handle, uint8_t flag) {
        using Fn = void(__cdecl*)(int32_t, uint8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004DCEF0})(colony_handle, flag);
    }
    inline void __cdecl Reset_Mouse_Picture_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004E1CD0})();
    }

    char* __cdecl Real_Building_Name_(int16_t building_id);
}
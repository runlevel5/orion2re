#pragma once
#include <cstdint>

namespace NEWGAME {
    inline s_colors& _color_ken = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005A6260});
    inline int16_t& _civ_button_variable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566384});
    inline int16_t* _fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x005663B0});
    inline char* DAT_0059532e = reinterpret_cast<char*>(std::uintptr_t{0x0059532E});

    inline void __cdecl Reload_Newgame_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x00486390)();
    }

    inline void __cdecl Globals_To_Temps_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x00486fd0)();
    }

    inline void __cdecl Update_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x00486ac0)();
    }

    inline void __cdecl Draw_Newgame_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004865a0)();
    }

    inline void __cdecl Temps_To_Globals_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x00486f60)();
    }

    inline int16_t __cdecl Race_Selection_Screen_(s_player* player) {
        using Fn = int16_t(__cdecl*)(s_player*);
        return reinterpret_cast<Fn>(0x00498B40)(player);
    }

    int16_t __cdecl Newgame_Screen_();
}
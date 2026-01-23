#pragma once

namespace FILEDEF {
    void Load_Game_Settings_();
    bool Save_Game_Settings_(void);
    void Update_Game_Settings_Version_Number_();

    inline void __cdecl Save_Game_(int slot) {
        using Fn = void(__cdecl*)(int);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0043C840 })(slot);
    }

    inline void __cdecl Save_Moo_Custom_Player_Data_(s_player* player) {
        using Fn = void(__cdecl*)(s_player*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0043D1B0 })(player);
    }
}

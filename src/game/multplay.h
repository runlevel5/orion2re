#pragma once

namespace MULTPLAY {
    // Suggested name: Set_Default_Modem_Settings
    inline void __cdecl Set_Default_Modem_Settings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00504C70 });
        fn();
    }

    inline void __cdecl Multi_Player_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x005056f0})();
    }
    inline void __cdecl Hotseat_Select_Player_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00503a50})();
    }
}
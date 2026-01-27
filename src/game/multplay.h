#pragma once

namespace MULTPLAY {
    // Suggested name: Set_Default_Modem_Settings
    inline void __cdecl Set_Default_Modem_Settings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00504C70 });
        fn();
    }
}
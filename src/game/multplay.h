#pragma once

namespace MULTPLAY {
    // Suggested name: Set_Default_Modem_Settings
    inline void __stdcall Set_Default_Modem_Settings_() {
        using Fn = void(__stdcall*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00504C70 });
        fn();
    }
}
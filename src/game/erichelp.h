#pragma once

namespace ERICHELP {
    inline void __cdecl Set_Tax_Rate_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004123F0});
        fn();
    }
}
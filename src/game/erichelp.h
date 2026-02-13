#pragma once

namespace ERICHELP {
    inline void __cdecl Set_Tax_Rate_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004123F0});
        fn();
    }

    inline void __cdecl Set_Building_Queue_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00412350})();
    }
}
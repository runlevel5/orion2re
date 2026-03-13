#pragma once

namespace FIREPTS {
    inline void __cdecl Load_FP_Data_From_LBX_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00508480});
        fn();
    }
}
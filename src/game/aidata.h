#pragma once

namespace aidata {
    inline s_ai_tech_info*& _aitech = *reinterpret_cast<s_ai_tech_info**>(std::uintptr_t{0x005953CC});

    inline void __cdecl Compute_AI_Data_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004edf80 });
        fn();
    }

    inline void __cdecl Deallocate_AI_Data_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004f05a0 });
        fn();
    }
}
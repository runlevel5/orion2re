#pragma once
#include <cstdint>

namespace COMBFIND {
    inline void __cdecl Search_For_Battles_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004bcf70 });
        fn();
    }

    inline char* __cdecl Load_Combfind_Strings_(char* data) {
        using Fn = char*(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BC0D0});
        return fn(data);
    }
}
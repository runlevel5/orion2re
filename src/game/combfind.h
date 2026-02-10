#pragma once
#include <cstdint>

namespace COMBFIND {
    inline char* __cdecl Load_Combfind_Strings_(char* data) {
        using Fn = char*(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BC0D0});
        return fn(data);
    }
}
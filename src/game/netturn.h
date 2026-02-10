#pragma once
#include <cstdint>

namespace NETTURN {
    inline void __cdecl Net_Next_Turn_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0047d5b0})();
    }

    inline char* __cdecl Load_Netturn_Strings_(char* data) {
        using Fn = char*(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047D570});
        return fn(data);
    }
}
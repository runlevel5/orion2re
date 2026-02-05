#pragma once
#include <cstdint>

namespace NETMOX {
    // Suggested name: NETMOX::Humans_Requesting_Diplomacy
    inline uint8_t __cdecl Humans_Requesting_Diplomacy_() {
        using Fn = uint8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00519c40})();
    }

    inline void __cdecl Human_Abdicates_(int32_t player_num) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519D50});
        fn(player_num);
    }
}
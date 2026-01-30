#pragma once
#include <cstdint>

namespace NETMOX {
    // Suggested name: NETMOX::Humans_Requesting_Diplomacy
    inline uint8_t __cdecl Humans_Requesting_Diplomacy_() {
        using Fn = uint8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00519c40})();
    }
}
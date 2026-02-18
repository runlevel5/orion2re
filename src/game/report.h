#pragma once
#include <cstdint>

namespace REPORT {
    inline uint16_t& _n_reports = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A4EAA});
    inline uint8_t (&_reports)[950] = *reinterpret_cast<uint8_t(*)[950]>(std::uintptr_t{0x005A4EC0});
    inline uint8_t& _done_player_reports = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4EB8});
    inline uint32_t (&_something_interesting_happened)[2] = *reinterpret_cast<uint32_t(*)[2]>(std::uintptr_t{0x005A4EB0});
    inline uint8_t (&_done_colonize_check)[9] = *reinterpret_cast<uint8_t(*)[9]>(std::uintptr_t{0x005A4EA0});
    inline uint8_t& _displayed_msgs = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A5276});
    inline uint8_t& _displayed_gnn = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4EAC});

    inline void __cdecl Initialize_Reports_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004aa050})();
    }

    inline void __cdecl Load_Report_Strings_(char* data) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004A9D80});
        fn(data);
    }
}
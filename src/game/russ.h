#pragma once
#include <cstdint>

namespace RUSS {
    inline auto PFMT_ = reinterpret_cast<void(__cdecl*)(int32_t, int32_t, const char*, ...)>(std::uintptr_t{0x004D7850});

    int16_t __cdecl Get_Weighted_Choice_Char_(int8_t* weights, int16_t count);

    int16_t __cdecl Get_Weighted_Choice_Short_(const int16_t* weights, int16_t count);

    inline void Mox_Sync_Update_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00483C80 });
        fn();
    }

    inline void __cdecl Mox_Update_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004d7820 });
        fn();
    }

    inline void __cdecl Assert_Settings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004d7810 });
        fn();
    }

    void __cdecl Release_All_Blocks_(void* seg);

    inline void Load_Russ_Strings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d7ed0});
        fn();
    }

    inline const char* s_Seed_ld = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575BC }); // "Seed: %ld"
    inline const char* s_Map_d = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575C8 });   // "Map: %d"
}
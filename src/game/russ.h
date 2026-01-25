#pragma once

namespace RUSS {
    inline uint16_t __cdecl Get_Weighted_Choice_Short_(uint16_t* weights, int count) {
        using Fn = uint16_t(__cdecl*)(uint16_t*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D78E0});
        return fn(weights, count);
    }

    inline uint8_t __cdecl Get_Weighted_Choice_Char_(uint8_t* weights, int32_t count) {
        using Fn = uint8_t(__cdecl*)(uint8_t*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d7860});
        return fn(weights, count);
    }

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

    template<typename... Args>
    inline void __cdecl PFMT_(int32_t type, int32_t priority, const char* format, Args... args) {
        using Fn = void(__cdecl*)(int32_t, int32_t, const char*, ...);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004D7850 });
        fn(type, priority, format, args...);
    }

    inline const char* s_Seed_ld = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575BC }); // "Seed: %ld"
    inline const char* s_Map_d = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575C8 });   // "Map: %d"
}
#pragma once
#include <cstdint>

namespace EVENTS {
    inline s_event_data (&_event_data)[36] = *reinterpret_cast<s_event_data(*)[36]>(std::uintptr_t{0x005A6460});
    inline uint32_t& _last_event_year = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A65A8});
    inline uint16_t& _event_delay_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A65AE});

    inline int16_t __cdecl Get_Antaran_Victim_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00477070})();
    }
    void Init_Events_();

    inline int16_t __cdecl Event_Check_Comet_(int16_t colony_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(0x00477750)(colony_idx);
    }

    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t colony_idx);
    int16_t __cdecl Event_Check_Space_Anomaly_At_Star_(int16_t star_idx);

    int16_t __cdecl Event_Check_Population_Boom_(int16_t colony_idx);
    int16_t __cdecl Event_Check_Plague_(int16_t colony_idx);

    int16_t __cdecl Event_Check_Pirate_Activity_(uint8_t player_idx);
    int16_t __cdecl Event_Check_Colony_Researching_(int16_t colony_idx);

    int16_t __cdecl Event_Check_Hyperspace_Flux_();

    inline void __cdecl Event_Surrender_(int8_t actor_player, int16_t target_player) {
        using Fn = void(__cdecl*)(int8_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004775f0});
        fn(actor_player, target_player);
    }

    int16_t __cdecl Colony_Has_Event_(int16_t colony_idx);

    inline void __cdecl Determine_Event_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004761c0 });
        fn();
    }

    inline void __cdecl Event_Twiddle_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004742a0 });
        fn();
    }

    inline void __cdecl Event_Orion_Discovered_(int16_t owner) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004775D0});
        fn(owner);
    }

    inline void __cdecl Event_Race_Eliminated_(int8_t player_idx) {
        using Fn = void(__cdecl*)(int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004775b0});
        fn(player_idx);
    }
}
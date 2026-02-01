#pragma once
#include <cstdint>

namespace EVENTS {
#pragma pack(push, 1)
    struct s_event_data {
        int8_t field0_0x0;
        int8_t field1_0x1;
        uint8_t field2_0x2;
        int16_t event_type;
        int16_t field4_0x5;
        int16_t field5_0x7;
    };
#pragma pack(pop)

    inline s_event_data (&_event_data)[36] = *reinterpret_cast<s_event_data(*)[36]>(std::uintptr_t{0x005A6460});
    inline uint32_t& _last_event_year = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A65A8});
    inline uint16_t& _event_delay_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A65AE});

    void Init_Events_();
    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t colony_idx);

    inline int16_t __cdecl Event_Check_Population_Boom_(int16_t colony_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00477720})(colony_idx);
    }

    inline int16_t __cdecl Event_Check_Plague_(int16_t colony_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004776D0})(colony_idx);
    }

    int16_t __cdecl Event_Check_Pirate_Activity_(uint8_t player_idx);

    inline int16_t __cdecl Event_Check_Hyperspace_Flux_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00477610})();
    }
}
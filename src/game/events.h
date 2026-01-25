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
}
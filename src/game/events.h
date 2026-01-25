#pragma once
#include <cstdint>

namespace EVENTS {
#pragma pack(push, 1)
    struct s_event_data {
        uint8_t field0_0x0;
        uint8_t field1_0x1;
        undefined field2_0x2;
        short event_type;
        short field4_0x5;
        short field5_0x7;
    };
#pragma pack(pop)

    inline s_event_data (&_event_data)[36] = *reinterpret_cast<s_event_data(*)[36]>(std::uintptr_t{0x005A6460});
    inline uint32_t& _last_event_year = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A65A8});
    inline uint16_t& _event_delay_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A65AE});

    void Init_Events_();
}
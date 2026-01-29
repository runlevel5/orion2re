#pragma once
#include <cstdint>

namespace MSG_ {
    #pragma pack(push, 1)
    struct s_MSG {
        uint32_t field0_0x0;
        uint32_t field1_0x4;
        uint32_t field2_0x8;
        uint32_t field3_0xc;
        uint16_t field4_0x10;
    };
    #pragma pack(pop)

    inline uint8_t (&_first_msg)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A66F0});
    inline uint8_t (&_n_msgs)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A66F8});
    inline s_MSG (&_msgs)[100] = *reinterpret_cast<s_MSG(*)[100]>(std::uintptr_t{0x005A6700});
    inline uint8_t (&_max_msgs)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A6E08});

    void __cdecl Allocate_Msg_Slots_();
    void __cdecl Add_Message(uint32_t header, uint32_t p2, uint32_t p3, uint32_t p4, uint16_t p5, int16_t buffer_idx);
}
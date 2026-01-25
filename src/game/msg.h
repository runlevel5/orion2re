#pragma once
#include <cstdint>

namespace MSG_ {
    inline uint8_t (&_msgs)[1800] = *reinterpret_cast<uint8_t(*)[1800]>(std::uintptr_t{0x005A6700});
    inline uint32_t& _first_msg = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A66F0});
    inline uint32_t& _n_msgs = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A66F8});
    inline uint32_t& _max_msgs = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A6E08});

    #pragma pack(push, 1)
    struct Msg_ColonyBaseRemoved {
        uint16_t pad1;
        uint8_t planet_idx;
        uint8_t pad2;
        uint16_t building_id;
        uint16_t refund_amount;
        uint32_t pad3;
        uint32_t pad4;
        uint32_t pad5;
    };
    #pragma pack(pop)

    inline void __cdecl Allocate_Msg_Slots_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004726E0})();
    }

    inline void __cdecl Add_Msg_(Msg_ColonyBaseRemoved m, int16_t player_id) {
        using Fn = void(__cdecl*)(Msg_ColonyBaseRemoved, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00473810});
        fn(m, player_id);
    }
}
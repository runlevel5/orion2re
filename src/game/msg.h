#pragma once
#include <cstdint>

namespace MSG_ {
    inline void __cdecl Add_Msg_(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5, int16_t player_idx) {
        using Fn = void(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00473810});
        fn(arg1, arg2, arg3, arg4, arg5, player_idx);
    }

    inline uint8_t (&_first_msg)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A66F0});
    inline uint8_t (&_n_msgs)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A66F8});
    inline s_MSG (&_msgs)[100] = *reinterpret_cast<s_MSG(*)[100]>(std::uintptr_t{0x005A6700});
    inline uint8_t (&_max_msgs)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A6E08});

    void __cdecl Allocate_Msg_Slots_();
    void __cdecl Add_Message(uint32_t header, uint32_t p2, uint32_t p3, uint32_t p4, uint16_t p5, int16_t buffer_idx);
    inline char* __cdecl Load_Msg_Strings_(char* data) {
        using Fn = char*(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00472040});
        return fn(data);
    }

    inline void __cdecl Add_Ongoing_Event_Msgs_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00473b50 });
        fn();
    }

    inline void __cdecl Add_Peace_Ended_Msg_(int player_i, int player_j) {
        using Fn = void(__cdecl*)(int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00473B30});
        fn(player_i, player_j);
    }
}
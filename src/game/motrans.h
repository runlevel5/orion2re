#pragma once

#include <cstdint>

namespace motrans {
    inline uint8_t& _mtp_receiving =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056085C});

    uint32_t __cdecl Modem_Status_();

    inline void __cdecl Modem_Update_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005394f0});
        fn();
    }

    inline void __cdecl Modem_Send_Message_(int32_t target_id, int16_t msg_type, uint8_t* data, uint32_t size) {
        using Fn = void(__cdecl*)(int32_t, int16_t, uint8_t*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00539260});
        fn(target_id, msg_type, data, size);
    }
}
#pragma once

#include <cstdint>

namespace motrans {
    inline uint8_t& _modemTransportInitialized =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00560858});

    inline uint8_t& _mtp_receiving =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056085C});

    inline uint32_t& _modem_receive_buffer_full =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560860});

    inline uint8_t*& _modem_receive_buffer =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00591E88});

    inline uint32_t& _modem_amount_received =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00591E9C});

    inline uint32_t& _mtp_user_type =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00591EC8});

    uint32_t __cdecl Modem_Status_();

    void __cdecl Modem_Update_();

    inline void __cdecl Receive_Modem_Packet_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00539570});
        fn();
    }

    void __cdecl Modem_Send_Message_(int32_t target_id, int16_t msg_type, uint8_t* data, uint32_t size);

    inline void __cdecl Send_Modem_Message_(int32_t param_1, void* param_2, uint32_t param_3) {
        using Fn = void(__cdecl*)(int32_t, void*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005392B0});
        fn(param_1, param_2, param_3);
    }
}
#pragma once
#include <cstdint>

struct s_network;

namespace simmodem {
    using recv_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    inline uint8_t*& _open_game_port =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x0055C050});

    inline uint8_t& _modemInitialized =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055C054});

    inline int8_t& _connected_to_id =
        *reinterpret_cast<int8_t*>(std::uintptr_t{0x00589A18});

    inline recv_fn& _modemUserRecFunc =
        *reinterpret_cast<recv_fn*>(std::uintptr_t{0x00589A14});

    inline void __cdecl Uninit_Modem_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535570});
        fn();
    }

    inline int32_t __cdecl Init_Modem_(s_network* net, recv_fn recv_fn_ptr) {
        using Fn = int32_t(__cdecl*)(s_network*, recv_fn);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535530});
        return fn(net, recv_fn_ptr);
    }

    inline void __cdecl Modem_Reset_(char* reset_cmd) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005357a0});
        fn(reset_cmd);
    }

    inline int8_t __cdecl Modem_Answer_() {
        using Fn = int8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535590});
        return fn();
    }

    inline int32_t __cdecl Modem_Connected_() {
        using Fn = int32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535780});
        return fn();
    }

    inline bool __cdecl Modem_Dial_Number_(char* phone_number) {
        using Fn = bool(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005355C0});
        return fn(phone_number);
    }

    inline void __cdecl Get_Modem_Error_(char* error_buffer) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535660});
        fn(error_buffer);
    }
}
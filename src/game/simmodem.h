#pragma once
#include <cstdint>

struct s_network;

namespace simmodem {
    using recv_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    extern uint8_t* _open_game_port;

    extern uint8_t _modemInitialized;

    extern int8_t _connected_to_id;

    extern recv_fn _modemUserRecFunc;

    void __cdecl Uninit_Modem_();

    int32_t __cdecl Init_Modem_(s_network* net, recv_fn recv_fn_ptr);

    void __cdecl Modem_Reset_(char* reset_cmd);

    int8_t __cdecl Modem_Answer_();

    int32_t __cdecl Modem_Connected_();

    bool __cdecl Modem_Dial_Number_(char* phone_number);

    void __cdecl Get_Modem_Error_(char* error_buffer);
}
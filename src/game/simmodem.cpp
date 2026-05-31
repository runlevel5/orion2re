#include "pch.h"

#include "simmodem.h"

namespace simmodem {

    uint8_t* _open_game_port;
    uint8_t _modemInitialized;
    int8_t _connected_to_id;
    recv_fn _modemUserRecFunc;

    int32_t __cdecl Init_Modem_(s_network* net, recv_fn recv_fn_ptr) {
        (void)net;
        _open_game_port = nullptr;
        _modemInitialized = 0;
        _connected_to_id = 0;
        _modemUserRecFunc = recv_fn_ptr;

        // Modem support is intentionally short-circuited in this build.
        // Report initialization failure so modem networking is disabled cleanly.
        return 0;
    }

    void __cdecl Uninit_Modem_() {
        _open_game_port = nullptr;
        _modemInitialized = 0;
        _connected_to_id = 0;
        _modemUserRecFunc = nullptr;
    }

    void __cdecl Modem_Reset_(char* reset_cmd) {
        (void)reset_cmd;
    }

    int8_t __cdecl Modem_Answer_() {
        return 0;
    }

    int32_t __cdecl Modem_Connected_() {
        return 0;
    }

    bool __cdecl Modem_Dial_Number_(char* phone_number) {
        (void)phone_number;
        return false;
    }

    void __cdecl Get_Modem_Error_(char* error_buffer) {
        if (error_buffer != nullptr) {
            error_buffer[0] = '\0';
        }
    }

}
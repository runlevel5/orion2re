#pragma once

#include <cstdint>

namespace motrans {
    extern uint8_t _modemTransportInitialized;

    extern uint8_t _mtp_receiving;

    extern uint32_t _modem_receive_buffer_full;

    extern uint8_t* _modem_receive_buffer;

    extern uint32_t _modem_amount_received;

    extern uint32_t _mtp_user_type;

    uint32_t __cdecl Modem_Status_();

    void __cdecl Modem_Update_();

    inline void __cdecl Receive_Modem_Packet_() {
    }

    void __cdecl Modem_Send_Message_(int32_t target_id, int16_t msg_type, uint8_t* data, uint32_t size);

    inline void __cdecl Send_Modem_Message_(int32_t param_1, void* param_2, uint32_t param_3) {
        (void)param_1;
        (void)param_2;
        (void)param_3;
    }
}
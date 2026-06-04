#pragma once

#include <cstdint>

namespace motrans {
    extern uint8_t _modemTransportInitialized;

    extern uint8_t _mtp_receiving;

    extern uint32_t _modem_receive_buffer_full;

    extern uint8_t* _modem_receive_buffer;

    extern uint32_t _modem_amount_received;

    extern uint32_t _mtp_user_type;

    // Returns the modem transport status code for the current receive state.
    uint32_t __cdecl Modem_Status_();

    // Polls the modem transport and dispatches a received packet when one is ready.
    void __cdecl Modem_Update_();

    // Placeholder receive hook; currently performs no work.
    inline void __cdecl Receive_Modem_Packet_() {
    }

    // Forwards a modem message when the modem layer is initialized.
    // target_id: Destination peer identifier.
    // msg_type: Message type identifier.
    // data: Payload buffer.
    // size: Payload size in bytes.
    void __cdecl Modem_Send_Message_(int32_t target_id, int16_t msg_type, uint8_t* data, uint32_t size);

    // Placeholder send hook; currently ignores the provided message parameters.
    // message_type: Message type identifier.
    // data: Payload buffer.
    // size: Payload size in bytes.
    inline void __cdecl Send_Modem_Message_(int32_t message_type, void* data, uint32_t size) {
        (void)message_type;
        (void)data;
        (void)size;
    }
}
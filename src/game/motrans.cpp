#include "pch.h"

namespace motrans {

void __cdecl Modem_Update_() {
    if (simmodem::_modemInitialized == 0) {
        return;
    }

    if (_modemTransportInitialized == 0) {
        return;
    }

    if (netcode::_connection_type == 1) {
        if (simmodem::Modem_Connected_() == 0) {
            return;
        }
    }

    using PortFunc = uint32_t(__thiscall*)(void*);
    auto get_port_status = *reinterpret_cast<PortFunc*>(simmodem::_open_game_port + 0x50);
    uint32_t status = get_port_status(simmodem::_open_game_port);

    if (status >= 24) {
        Receive_Modem_Packet_();

        if (_modem_receive_buffer_full == 1) {
            simmodem::_modemUserRecFunc(
                (int16_t)simmodem::_connected_to_id,
                _mtp_user_type,
                _modem_amount_received,
                _modem_receive_buffer
            );

            _modem_receive_buffer_full = 0;
        }
    }
}

uint32_t __cdecl Modem_Status_() {
    if (_mtp_receiving != 0) {
        return 6;
    }
    return 4;
}

void __cdecl Modem_Send_Message_(int32_t target_id, int16_t msg_type, uint8_t* data, uint32_t size) {
    if (simmodem::_modemInitialized != 0) {
        Send_Modem_Message_(msg_type, data, size & 0xFFFF);
    }
}

}
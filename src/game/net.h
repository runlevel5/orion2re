#pragma once
#include <cstdint>

namespace net {
    inline uint32_t& _thisHost =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00591EFC});

    // Suggested name: net::modem_settings_cmd_reset
    inline char* modem_settings_cmd_reset =
        reinterpret_cast<char*>(std::uintptr_t{0x00595181});

    // Suggested name: net::modem_settings_cmd_init
    inline char* modem_settings_cmd_init =
        reinterpret_cast<char*>(std::uintptr_t{0x0059519F});

    // Suggested name: net::modem_settings_cmd_hangup
    inline char* modem_settings_cmd_hangup =
        reinterpret_cast<char*>(std::uintptr_t{0x005951BD});

    // Suggested name: net::modem_settings_dial_number
    inline char* modem_settings_dial_number =
        reinterpret_cast<char*>(std::uintptr_t{0x005951DB});

    int32_t __cdecl NetPoll_(s_comcore_poll_buf* req);
}
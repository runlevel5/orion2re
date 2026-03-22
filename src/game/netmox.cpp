#include "pch.h"

namespace NETMOX {
    int8_t __cdecl Net_May_Begin_AI_Diplomacy_() {
        int8_t uninitialized_ret;

        if (NETMOX::_net.packet_type >= 0x25) {
            if (NETMOX::_net.packet_type <= 0x26) {
                return 0;
            }

            if (NETMOX::_net.packet_type == 0x28) {
                return 1;
            }
        }

        // Returns an uninitialized stack value, matching assembly at 0x005195b9
        return uninitialized_ret;
    }
}
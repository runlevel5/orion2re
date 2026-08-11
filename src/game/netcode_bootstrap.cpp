#include "pch.h"

#include "game/netcode_bootstrap.h"

namespace netcode {

    netadapter::e_bootstrap_mode __cdecl Get_Router_Bootstrap_Mode_(uint32_t mode, uint8_t sync_state) {
        if (mode == 0) {
            if (sync_state == NET_SYNC_STATE_HOST_STARTUP || sync_state == NET_SYNC_STATE_LOAD_GAME_HOST_SETUP) {
                return netadapter::BOOTSTRAP_EMBEDDED_LAN_HOST;
            }
            return netadapter::BOOTSTRAP_LAN_DISCOVERY;
        }

        return netadapter::BOOTSTRAP_DIRECT_ENDPOINT;
    }
}
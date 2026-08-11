#pragma once

#include "game/netadapter.h"

namespace netcode {

    netadapter::e_bootstrap_mode __cdecl Get_Router_Bootstrap_Mode_(uint32_t mode, uint8_t sync_state);
}
#pragma once
#include <cstdint>

struct s_colony;

namespace BOMB {
    int16_t __cdecl Handle_Leader_At_Lost_Colony_(s_colony* colony);

    void __cdecl Destroy_Colony_(s_colony* colony, int32_t capturing_player);
}
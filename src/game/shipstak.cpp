#include "pch.h"

namespace SHIPSTAK {
    short __cdecl Ship_Stack_Star_Id_(short node_idx) {
        return MOX::_ship[MOX::_ship_node[node_idx].ship_idx].location;
    }
}
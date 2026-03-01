#include "pch.h"

namespace SHIPS {

    void __cdecl Kill_Ship_(int16_t ship_idx) {
        HACCESS::Ship_Owner_(ship_idx);

        bool officer_relocated = false;
        for (int16_t leader_idx = 0; leader_idx < 67; ++leader_idx) {
            if (officer_relocated) {
                break;
            }

            if (MOX::_leaders[leader_idx].type == 0 && 
                MOX::_leaders[leader_idx].location == ship_idx && 
                MOX::_leaders[leader_idx].eta > 0) {
                
                OFFICER::Move_Officer_To_Pool_(leader_idx);
                officer_relocated = true;
            }
        }

        MOX::_ship[ship_idx].location = -1;
        MOX::_ship[ship_idx].status = 5;
    }

} // namespace SHIPS
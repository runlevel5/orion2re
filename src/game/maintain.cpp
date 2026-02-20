#include "deps.h"
#include "maintain.h"

namespace maintain {
    // Suggested name: MAINTAIN::Make_Scrap_Ships_Dead_
    void __cdecl Make_Scrap_Ships_Dead_() {
        int num_ships = (int)MOX::_NUM_SHIPS;
        s_ship_data* ships = MOX::_ship;

        for (int i = 0; i < num_ships; ++i) {
            if (ships[i].status == 7 || ships[i].status == 8) {
                SHIPS::Kill_Ship_(static_cast<int16_t>(i));
            }
        }
    }
}
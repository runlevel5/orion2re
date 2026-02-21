#include "deps.h"

namespace lostship {
    void __cdecl Lose_Out_Of_Range_Ships_() {
        int8_t reserve_range_array[72];
        int8_t fuel_range_array[72];

        if (0 < MOX::_NUM_PLAYERS) {
            for (int16_t player_idx = 0; player_idx < static_cast<int16_t>(MOX::_NUM_PLAYERS); ++player_idx) {
                lostship::Compute_Stars_In_Range_Of_Player_(reserve_range_array, fuel_range_array, player_idx);
                lostship::Player_Lose_Out_Of_Range_Ships_(reserve_range_array, fuel_range_array, player_idx);
                COMBFIND::Process_Retreating_Ships_();
            }
        }
    }

    void __cdecl Player_Lose_Out_Of_Range_Ships_(int8_t* reserve_range_array, int8_t* fuel_range_array, int16_t player_id) {
        int32_t ship_count = static_cast<int32_t>(MOX::_NUM_SHIPS);
        s_ship_data* ship_array = MOX::_ship;

        for (int32_t i = 0; i < ship_count; ++i) {
            s_ship_data* ship = &ship_array[i];

            // If ship is active and belongs to the specified player
            if (ship->status == 0 && static_cast<int16_t>(ship->owner) == player_id) {
                bool is_out_of_range = false;

                // Check basic fuel range
                if (fuel_range_array[ship->location] == 0) {
                    is_out_of_range = true;
                }
                // Check reserve fuel range if ship does not have extended range capabilities
                else if (reserve_range_array[ship->location] == 0) {
                    if (!struct_::Test_Bit_Field_(ship->d.special_device_flags, static_cast<uint16_t>(SPECIAL_EXTENDED_FUEL_TANKS))) {
                        is_out_of_range = true;
                    }
                }

                if (is_out_of_range) {
                    ship->status = 9;
                }
            }
        }
    }
}
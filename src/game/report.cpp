#include "deps.h"

namespace REPORT {

    s_colony* __cdecl Player_Colony_Base_At_Star_(short player_idx, short star_idx) {
        for (int i = 0; i < 5; ++i) {
            short planet_idx = MOX::_star[star_idx].planet_index[i];
            
            if (planet_idx != -1) {
                short colony_idx = MOX::_planet[planet_idx].colony_index;
                
                if (colony_idx != -1) {
                    s_colony* colony = &MOX::_colony[colony_idx];
                    
                    if (colony->owner == player_idx && colony->buildings[11] > 0) {
                        return colony;
                    }
                }
            }
        }

        return nullptr;
    }

    s_ship_data* __cdecl Player_Ship_Type_At_Star_(short owner, short ship_type, short location, char include_just_built) {
        s_ship_data* ship = MOX::_ship;
        int num_ships = (int)MOX::_NUM_SHIPS;
        s_ship_data* end = &ship[num_ships];

        while (ship < end) {
            if ((short)(int8_t)ship->d.ship_type == ship_type &&
                (short)ship->owner == owner &&
                ship->location == location &&
                ship->status < 5) {

                if (include_just_built != 0 || ship->just_built == 0) {
                    return ship;
                }
            }
            ship++;
        }

        return nullptr;
    }

    bool __cdecl Has_Msg_(int16_t player_idx, int8_t show_summary) {
#pragma pack(push, 1)
        struct s_MSG_internal {
            int8_t msg_type;     // 0x00
            int8_t field1_0x1;   // 0x01
            int16_t field2_0x2;  // 0x02
            int8_t val;          // 0x04
            int8_t field5_0x5;   // 0x05
            int8_t field6_0x6;   // 0x06
            int8_t field7_0x7;   // 0x07
            int32_t field2_0x8;  // 0x08
            int32_t field3_0xc;  // 0x0c
            int16_t field4_0x10; // 0x10
        };
#pragma pack(pop)

        uint8_t result = 0;

        // Check if the summary for this player hasn't been displayed yet
        if (!((REPORT::_displayed_msgs >> (player_idx & 0x1F)) & 1)) {
            int16_t num_msgs = MSG_::N_Msgs_For_Player_(player_idx);

            if (num_msgs > 0 && MOX::_settings.end_of_turn_summary != 0) {
                int16_t first_idx = MSG_::First_Msg_For_Player_(player_idx);
                bool should_show = false;

                // Scan messages backwards from newest to oldest
                for (int i = num_msgs - 1; i >= 0; --i) {
                    s_MSG_internal* msg = reinterpret_cast<s_MSG_internal*>(&MSG_::_msgs[first_idx + i]);

                    switch (msg->msg_type) {
                    case 0:
                        switch (msg->val) {
                        case 4:
                        case 9:
                            should_show = true;
                            break;
                        case 6:
                        case 7:
                        case 8:
                            should_show = false;
                            break;
                        default:
                            // Serious reports check (caseD_1)
                            should_show = (MOX::_settings.only_show_serious_turn_summary_reports == 0);
                            break;
                        }
                        break;

                    case 4:
                    case 5:
                    case 7:
                        should_show = true;
                        break;

                    case 8:
                    case 16:
                        should_show = false;
                        break;

                    case 9:
                        // Check specific criteria for message type 9
                        if (msg->field2_0x2 != -1 && msg->field7_0x7 != 0) {
                            should_show = true;
                        }
                        else {
                            should_show = false;
                        }
                        break;

                    case 17:
                        if (msg->val == (int8_t)MOX::_PLAYER_NUM) {
                            should_show = false;
                        }
                        else {
                            // Serious reports check (caseD_1)
                            should_show = (MOX::_settings.only_show_serious_turn_summary_reports == 0);
                        }
                        break;

                    default:
                        // Serious reports check (caseD_1)
                        should_show = (MOX::_settings.only_show_serious_turn_summary_reports == 0);
                        break;
                    }

                    if (should_show) {
                        break;
                    }
                }

                if (should_show) {
                    result = 4;
                    if (show_summary != 0) {
                        MOX::_current_screen = 0x28 /* SCREEN_TURN_SUMMARY */;
                        MOX::_return_screen = 0x27 /* SCREEN_REPORTS */;
                        MOX::_global_esc = 1;
                        REPORT::_displayed_msgs |= (1 << (player_idx & 0x1F));
                    }
                }
            }
        }

        return (bool)result;
    }

}
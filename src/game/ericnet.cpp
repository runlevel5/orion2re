#include "pch.h"

namespace ERICNET {
    void __cdecl Init_Homeworld_Colony_(int16_t colony_idx, int16_t player_idx) {
        Init_Homeworld_Colony2_(colony_idx, player_idx, 1);
    }

    void __cdecl Init_Non_Homeworld_Colony_(int16_t colony_idx, int16_t player_idx) {
        Init_Homeworld_Colony2_(colony_idx, player_idx, 0);
    }

    int __cdecl Mox_Play_Sound_(int16_t sound_id) {
        if (MOX::_settings.sound_fx_on != 0) {
            return sound::Play_Sound_(sound_id);
        }

        return 0;
    }

    void __cdecl Do_All_Ships_XP_Check_() {
        int16_t instructor_bonuses[8];
        memset(instructor_bonuses, 0, sizeof(instructor_bonuses));

        Best_Instructor_Bonus_(instructor_bonuses);

        for (int16_t ship_idx = 0; ship_idx < 500; ++ship_idx) {
            s_ship_data& ship = MOX::_ship[ship_idx];

            if (ship.status < 5 && static_cast<uint8_t>(ship.owner) < 8) {
                int16_t owner_id = HACCESS::Ship_Owner_(ship_idx);
                int16_t bonus = instructor_bonuses[owner_id];
                Do_XP_For_Ship_(ship_idx, bonus);
            }
        }
    }

    void __cdecl Calc_Ship_Level_(int16_t ship_idx) {
        s_ship_data* ship = &MOX::_ship[ship_idx];
        uint16_t level = 0;

        // Iterate through experience thresholds to determine ship level
        for (int i = 0; i < 4; ++i) {
            if (ship->crew_experience >= MOX::_crew_data[level + 1].experience) {
                level++;
            }
        }

        // Add level bonus for Warlord trait
        s_player* owner = &MOX::_player[static_cast<uint8_t>(ship->owner)];
        if (owner->traits[TRAIT_WARLORD] != 0) {
            level++;
        }

        // Final level is capped at 5
        if (level > 5) {
            level = 5;
        }

        ship->crew_quality = static_cast<int8_t>(level);
    }

    void __cdecl Do_XP_For_Ship_(int16_t ship_index, int16_t xp_gain) {
        Do_Academy_At_Location_Check_(ship_index);

        int32_t total_xp = (int32_t)MOX::_ship[ship_index].crew_experience + (int32_t)xp_gain + 1;
        int32_t max_xp = (int16_t)MOX::_crew_data[3].experience;

        if (total_xp > max_xp) {
            total_xp = max_xp;
        }

        MOX::_ship[ship_index].crew_experience = (int16_t)total_xp;
        Calc_Ship_Level_(ship_index);
    }

    void __cdecl Do_Academy_At_Location_Check_(int16_t ship_index) {
        s_ship_data& ship = MOX::_ship[ship_index];

        // Only active ships gain experience
        if (ship.status == 0) {
            int16_t star_idx = ship.location;
            int8_t ship_owner = ship.owner;

            for (int i = 0; i < 5; ++i) {
                int16_t planet_idx = MOX::_star[star_idx].planet_index[i];
                if (planet_idx > -1) {
                    int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
                    if (colony_idx > -1) {
                        s_colony& colony = MOX::_colony[colony_idx];
                        // If colony is owned by the same player and has a Space Academy
                        if (colony.owner == ship_owner && colony.buildings[BUILDING_SPACE_ACADEMY] != 0) {
                            ship.crew_experience++;
                        }
                    }
                }
            }
        }
    }
}
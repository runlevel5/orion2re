#include "pch.h"
#include <cstring>

namespace ERICNET {
    void __cdecl Init_Homeworld_Colony2_(int16_t planet_idx, int16_t player_idx, uint8_t param_flag) {
        int16_t colony_idx = MOX::_NUM_COLONIES;
        MOX::_NUM_COLONIES++;

        s_colony& colony = MOX::_colony[colony_idx];
        memset(&colony, 0, sizeof(s_colony));

        if (COLONY::_debug_flag) {
            RUSS::PFMT_(0, 0, "INITING HOMEWORLD ph:%d pl:%d num:%d", (int)planet_idx, (int)player_idx, (int)MOX::_NUM_COLONIES);
            assert::hgetch_();
        }

        INITGAME::Init_Colony_(colony_idx, planet_idx, (uint8_t)player_idx);
        COLONY::Set_Colony_Globals_(colony_idx);

        int8_t star_idx = MOX::_planet[planet_idx].star_index;
        MOX::_star[star_idx].visited |= (1 << player_idx);

        colony.climate = MOX::_planet[planet_idx].climate;

        int16_t limit = COLCALC::Colony_Race_Pop_Limit_(&colony, player_idx);

        if (MOX::_settings.starting_civilization_level == 2 /* Post-War */) {
            int32_t growth_mod = MOX::_player[player_idx].traits[TRAIT_POP_GROWTH_BONUS];
            colony.n_pops = (int8_t)(((growth_mod + 300) * (int32_t)limit * 6) / 3000);

            if (limit < 9) {
                colony.n_pops = (int8_t)(((int32_t)(uint8_t)colony.n_pops + (int32_t)limit) / 2);
            }

            if (colony.n_pops == 0) {
                colony.n_pops = 1;
            }

            if (param_flag != 0 && (uint8_t)colony.n_pops < 8) {
                colony.n_pops = 8;
            }
        } else {
            colony.n_pops = 8;
        }

        for (int i = 0; i < (int)(uint8_t)colony.n_pops; ++i) {
            uint32_t job_bits;
            if (MOX::_planet[planet_idx].food_per_farmer == 0 || MOX::_player[player_idx].traits[TRAIT_EATS_MINERALS] != 0) {
                job_bits = ((i % 2) + 1) << 7;
            } else {
                job_bits = (i % 3) << 7;
            }

            uint32_t pop_val = colony.pop[i];
            pop_val = (pop_val & ~0x180) | job_bits;
            pop_val = (pop_val & ~0x00F) | (player_idx & 0x00F);
            pop_val = (pop_val & ~0x070) | ((uint32_t)(player_idx << 4) & 0x070);
            pop_val &= ~0x400;
            pop_val |= 0x200;
            colony.pop[i] = pop_val;
        }

        memset(colony.buildings, 0, 49);

        if (MOX::_player[colony.owner].traits[TRAIT_CURRENT_GOVERNMENT] < 6 && param_flag != 0) {
            Add_Building_(colony_idx, BUILDING_CAPITOL);
        }

        int16_t added_buildings_count = 0;
        for (int j = 0; ERICNET::_buildlist[j] != 0; ++j) {
            if (added_buildings_count >= ERICNET::_start_civ_level[MOX::_settings.starting_civilization_level]) {
                break;
            }

            if ((uint8_t)colony.n_pops <= (added_buildings_count * 3) / 2) {
                break;
            }

            int16_t building_id = ERICNET::_buildlist[j];
            if (COLCALC::Colony_Can_Build_Product_(&colony, building_id)) {
                bool can_add = true;
                switch (building_id) {
                    case 5:
                    case 13:
                    case 32:
                        can_add = (MOX::_player[player_idx].traits[TRAIT_ENVIRONMENT_IMMUNE] == 0);
                        break;
                    case 10:
                    case 25:
                        can_add = ((int32_t)(uint8_t)colony.n_pops < (int32_t)limit);
                        break;
                    case 20:
                    case 31:
                        can_add = (MOX::_player[player_idx].traits[TRAIT_CURRENT_GOVERNMENT] / 2 != 3);
                        break;
                    case 21:
                    case 37:
                    case 43:
                        can_add = (MOX::_player[player_idx].traits[TRAIT_EATS_MINERALS] == 0);
                        break;
                    default:
                        can_add = true;
                        break;
                }

                if (can_add) {
                    Add_Building_(colony_idx, building_id);
                    added_buildings_count++;
                }
            }
        }

        if (MOX::_settings.starting_civilization_level == 1 /* Average */ && param_flag != 0) {
            MOX::_ship[MOX::_NUM_SHIPS].owner = (int8_t)player_idx;
            COLBLDG::Create_Ship_(player_idx, 0, colony_idx);

            MOX::_ship[MOX::_NUM_SHIPS].owner = (int8_t)player_idx;
            COLBLDG::Create_Ship_(player_idx, 0, colony_idx);

            MOX::_ship[MOX::_NUM_SHIPS].owner = (int8_t)player_idx;
            INITSHIP::Load_Colony_Ship_Design_(player_idx, &MOX::_player[player_idx].ship_designs[5]);
            COLBLDG::Create_Ship_(player_idx, 5, colony_idx);
        }

        colony.food2_per_farmer = MOX::_planet[colony.planet].food_per_farmer;
        colony.max_farms = MOX::_planet[colony.planet].max_farms;
        colony.climate = MOX::_planet[colony.planet].climate;
        colony.military[0] = 3; // Ground Defense Units

        colony.production[0] = 0;
        colony.production[1] = 0;
        colony.production[2] = 0;
        colony.production[3] = 0;
        
        colony.maintenance[0] = 0;
        colony.maintenance[1] = 0;
        colony.maintenance[2] = 0;
        colony.maintenance[3] = 0;
    }

    void __cdecl Init_Homeworld_Colony_(int16_t planet_idx, int16_t player_idx) {
        Init_Homeworld_Colony2_(planet_idx, player_idx, 1);
    }

    void __cdecl Init_Non_Homeworld_Colony_(int16_t planet_idx, int16_t player_idx) {
        Init_Homeworld_Colony2_(planet_idx, player_idx, 0);
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

    void __cdecl Remove_Building_(int16_t colony_idx, int16_t building_id) {
        MOX::_colony[colony_idx].buildings[building_id] = 0;

        switch (building_id) {
            case 8:  /* BLDG_SUBTERRANEAN_FARMS */
            case 40: /* BLDG_GAIA_CONTROLLER */
            case 41: /* BLDG_EVOLUTIONARY_MUTATION */
                for (int16_t i = 0; i < 7; ++i) {
                    int16_t prod_val = MOX::_colony[colony_idx].producing[i];
                    if (prod_val < -99) {
                        int16_t ship_idx = -100 - prod_val;
                        if (MOX::_ship[ship_idx].d.size > 1) {
                            COLBLDG::Delete_From_Colony_Queue_(colony_idx, i);
                            i--;
                        }
                    }
                }
                // Fallthrough
            case 3:  /* BLDG_STAR_BASE */
            case 14: /* BLDG_BATTLESTATION */
            case 45: /* BLDG_STAR_FORTRESS */
                COLCALC::Compute_Star_Colony_Stuff_(&MOX::_star[MOX::_planet[MOX::_colony[colony_idx].planet].star_index]);
                if (building_id == 45 /* BLDG_STAR_FORTRESS */) {
                    SHIPMOVE::Update_Ship_ETAs_(static_cast<int32_t>(MOX::_colony[colony_idx].owner));
                    return;
                }
                break;

            case 9: /* BLDG_CAPITOL */
                invasion::Assign_New_Home_Planet_(static_cast<int8_t>(MOX::_colony[colony_idx].owner), colony_idx);
                return;

            case 22: /* BLDG_ANDROIDS */
                for (int16_t i = 0; i < 7; ++i) {
                    int16_t prod_val = MOX::_colony[colony_idx].producing[i];
                    if (prod_val < -99) {
                        int16_t ship_idx = -100 - prod_val;
                        if (MOX::_ship[ship_idx].d.ship_type == 2 /* SHIP_TYPE_COLONY_SHIP */) {
                            COLBLDG::Delete_From_Colony_Queue_(colony_idx, i);
                            i--;
                        }
                    }
                }
                return;

            case 23: /* BLDG_ATMOSPHERIC_RENEWER */
            case 24: /* BLDG_WEATHER_CONTROLLER */
            case 28: /* BLDG_CORE_WASTE_DUMP */
                if (MOX::_planet[MOX::_colony[colony_idx].planet].climate < 2) {
                    COLBLDG::Delete_Prod_From_Colony_Queue_(colony_idx, 44 /* PROD_RECENTERING */);
                }
                // Fallthrough
            case 15: /* BLDG_POLLUTION_PROCESSOR */
                COLCALC::Colony_Environmental_Stuff_(&MOX::_colony[colony_idx]);
                invasion::Enforce_Population_Limits_At_Colony_(&MOX::_colony[colony_idx]);
                COLCALC::Colony_Calculation_(colony_idx);
                break;

            default:
                break;
        }
    }
}
#include "pch.h"

namespace antarans {
    int32_t __cdecl Delta_Stardate_() {
        return MOX::_stardate - 35000;
    }

    int16_t __cdecl Delay_() {
        int8_t level = static_cast<int8_t>(MOX::_settings.starting_civilization_level);

        switch (level) {
        case 0:
            return 200;
        case 1:
            return 100;
        case 2:
        default:
            return 0;
        }
    }

    // Suggested name: Antarans_Defensive_Resource_Bonus
    int16_t __cdecl Antarans_Defensive_Resource_Bonus_() {
        if (MOX::_settings.game_difficulty == 3) {
            return 150;
        }

        if (MOX::_settings.game_difficulty == 4) {
            return 200;
        }

        return 100;
    }

    int16_t __cdecl Antarans_Offensive_Resource_Bonus_() {
        if (MOX::_settings.game_difficulty == 3) {
            return 150;
        }

        if (MOX::_settings.game_difficulty == 4) {
            return 200;
        }

        return 100;
    }

    int16_t __cdecl No_Small_Ships_After_N_Turns_() {
        int16_t bonus = Antarans_Defensive_Resource_Bonus_();

        if (MOX::_settings.game_difficulty < 3 /* HARD */) {
            return 100;
        }

        // The turn threshold is 12500 divided by the Antaran resource bonus.
        // Higher bonus = earlier appearance of large ships.
        return (int16_t)(12500 / (int32_t)bonus);
    }

    int16_t __cdecl No_Medium_Ships_After_N_Turns_() {
        int16_t bonus = Antarans_Defensive_Resource_Bonus_();

        if (MOX::_settings.game_difficulty < 3) {
            return 199;
        }

        if (bonus == 0) {
            return 0;
        }

        return (int16_t)(20000 / bonus);
    }

    int16_t __cdecl Next_Antaran_Ship_Size_() {
        int16_t result = -1;

        for (int16_t i = 8; i >= 0; --i) {
            if (result != -1) {
                break;
            }

            int32_t available = (int32_t)MOX::_antarans.offensive_ships[i] -
                                (int32_t)MOX::_antarans.offensive_deployed[i];

            if (available > 0) {
                result = i;
            }
        }

        return result;
    }

    bool __cdecl Antaran_Defensive_Force_At_Maximum_() {
        for (int i = 0; i < 9; ++i) {
            uint8_t max_val = static_cast<uint8_t>(_n_max_antaran_def_ships[i]);

            // If a ship type has a maximum threshold specified, check if current count meets it
            if (max_val != 0) {
                if (MOX::_antarans.defensive_ships[i] < static_cast<int16_t>(max_val)) {
                    return false;
                }
            }
        }
        return true;
    }

    void __cdecl Increment_Antaran_Resource_Level_() {
        int32_t delta_stardate = Delta_Stardate_();
        int16_t antaran_delay = Delay_();
        int32_t diff_stardate = delta_stardate - (int32_t)antaran_delay;

        // Perform resource increment every 25 stardates after the delay period
        if (diff_stardate > 0 && (diff_stardate % 25 == 0)) {
            int32_t intervals = diff_stardate / 25;
            int16_t def_bonus_base = Antarans_Defensive_Resource_Bonus_();
            int16_t off_bonus_base = Antarans_Offensive_Resource_Bonus_();

            // Calculate increases using a rounding-up approach (add 99 then divide by 100)
            int16_t def_inc = (int16_t)((def_bonus_base * intervals + 99) / 100);
            int16_t off_inc = (int16_t)((off_bonus_base * intervals + 99) / 100);

            // Offensive resource always gains the defensive bonus
            MOX::_antarans.resource_offensive += def_inc;

            if (Antaran_Defensive_Force_At_Maximum_()) {
                // If defensive resource is capped, offensive resource gains the offensive bonus instead
                MOX::_antarans.resource_defensive += off_inc;
            } else {
                // Otherwise, defensive resource gains its corresponding bonus
                MOX::_antarans.attack_pressure += off_inc;
            }
        }
    }

    void __cdecl Build_Antaran_Ships_(short *resources, short *ships, int8_t *max_ships, char is_offensive) {
        int32_t stardate_delta = Delta_Stardate_();
        int32_t antaran_delay = (int32_t)Delay_();
        int32_t time_ref = stardate_delta - antaran_delay;

        bool debug_headers_printed = false;

        while (true) {
            // Check if we can afford the smallest possible ship
            if (*resources < (short)(uint8_t)_antaran_ship_cost[0]) {
                return;
            }

            int32_t chosen_size = -1;
            uint16_t starting_size = (MOX::_settings.starting_civilization_level == 2);

            for (uint16_t size = starting_size; size < 9; ++size) {
                bool is_allowed_by_time = false;
                if (size == 0) {
                    if ((int32_t)No_Small_Ships_After_N_Turns_() >= time_ref) {
                        is_allowed_by_time = true;
                    }
                } else if (size == 1) {
                    if ((int32_t)No_Medium_Ships_After_N_Turns_() >= time_ref) {
                        is_allowed_by_time = true;
                    }
                } else {
                    is_allowed_by_time = true;
                }

                if (is_allowed_by_time) {
                    int32_t cost = (uint8_t)_antaran_ship_cost[size];
                    if (cost != 0 && *resources >= (short)cost && (short)(uint8_t)max_ships[size] > ships[size]) {
                        if (!debug_headers_printed) {
                            debug_headers_printed = true;
                            const char* side = is_offensive ? "Offensive" : "Defensive";
                            _sprintf(MOX::_temp_string, "\n(%4d.%d) Building Antaran %s ships:",
                                    (int)MOX::_stardate / 10, (int)MOX::_stardate % 10, side);

                            if (!is_offensive) {
                                strcat(MOX::_temp_string, ">> current def ships (by size) : ");
                                for (int i = 0; i < 9; ++i) {
                                    char buf[20];
                                    _sprintf(buf, "%2d ", MOX::_antarans.defensive_ships[i]);
                                    strcat(MOX::_temp_string, buf);
                                }
                            } else {
                                strcat(MOX::_temp_string, ">> current off ships (by size) : ");
                                for (int i = 0; i < 9; ++i) {
                                    char buf[20];
                                    _sprintf(buf, "%2d ", MOX::_antarans.offensive_ships[i]);
                                    strcat(MOX::_temp_string, buf);
                                }
                                strcat(MOX::_temp_string, ">> off ships deployed (by size): ");
                                for (int i = 0; i < 9; ++i) {
                                    char buf[20];
                                    _sprintf(buf, "%2d ", MOX::_antarans.offensive_deployed[i]);
                                    strcat(MOX::_temp_string, buf);
                                }
                            }
                        }

                        char build_info[512];
                        _sprintf(build_info, "-------- building size %d, resources = %d, n_ships[%d] = %d",
                                (int)size, (int)*resources, (int)size, (int)ships[size]);
                        strcat(MOX::_temp_string, build_info);

                        _sprintf(build_info, "-------- n_max_ships[%d] = %d, cost = %d",
                                (int)size, (int)(uint8_t)max_ships[size], (int)(uint8_t)_antaran_ship_cost[size]);
                        strcat(MOX::_temp_string, build_info);

                        chosen_size = (int32_t)size;
                        break;
                    }
                }
            }

            if (chosen_size == -1) {
                return;
            }

            // Perform construction
            short current_count = ships[chosen_size];
            ships[chosen_size] = current_count + 1;

            // If small ships hit their cap, reset the cap
            if (chosen_size <= 2 && _n_max_antaran_off_ships[chosen_size] == current_count + 1) {
                _n_max_antaran_off_ships[chosen_size] = 0;
            }

            short cost = _antaran_ship_cost[chosen_size];
            *resources -= cost;

            char build_final[128];
            _sprintf(build_final, "-------- after build: resources = %3d, n_ships[%d] = %2d",
                    (int)*resources, (int)chosen_size, (int)ships[chosen_size]);
            strcat(MOX::_temp_string, build_final);

            // Special Titan logic (size 4) on Hard/Impossible difficulty
            if (chosen_size == 4 && _offensive_titan_ship_built == 0 && MOX::_settings.game_difficulty > 2) {
                _offensive_titan_ship_built = 1;
                for (int i = 0; i < 9; ++i) {
                    _antaran_ship_cost[i] = (int8_t)(( (int)_antaran_ship_cost[i] * 90) / 100);
                }
            }
        }
    }

    void __cdecl Build_Antaran_Defensive_Ships_() {
        strcpy(MOX::_temp_string, s_defensive_0055858c);

        Build_Antaran_Ships_(
            &MOX::_antarans.resource_defensive,
            MOX::_antarans.defensive_ships,
            _n_max_antaran_def_ships,
            0
        );
    }

    void __cdecl Build_Antaran_Offensive_Ships_() {
        strcpy(MOX::_temp_string, s_offensive_00558598);

        Build_Antaran_Ships_(
            &MOX::_antarans.resource_offensive,
            MOX::_antarans.offensive_ships,
            _n_max_antaran_off_ships,
            1
        );
    }

    void __cdecl Build_List_Of_Stars_By_Player_(short player_idx, short* star_list, short* star_count) {
        *star_count = 0;

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            if (HACCESS::N_Colonies_At_Star_(star_idx, player_idx)) {
                star_list[*star_count] = star_idx;
                *star_count += 1;
            }
        }

        int16_t shuffle_limit = *star_count / 2;
        for (int16_t i = 0; i < shuffle_limit; ++i) {
            int16_t idx1 = static_cast<int16_t>(random::Random_(*star_count)) - 1;
            int16_t idx2 = static_cast<int16_t>(random::Random_(*star_count)) - 1;

            if (idx1 != idx2) {
                int16_t temp = star_list[idx1];
                star_list[idx1] = star_list[idx2];
                star_list[idx2] = temp;
            }
        }
    }

    short __cdecl Get_Antaran_Target_Star_(short player_idx) {
        short star_count;
        short star_indices[72];
        short target_star_idx = -1;

        Build_List_Of_Stars_By_Player_(player_idx, star_indices, &star_count);

        if (star_count > 0) {
            for (short i = 0; i < star_count; ++i) {
                // Reservoir sampling: pick the i-th star with 1/(i+1) probability
                if (random::Random_(static_cast<int>(i) + 1) == 1) {
                    target_star_idx = star_indices[i];
                }
            }
        }

        return target_star_idx;
    }

    void __cdecl Antaran_Invasion_Check_() {
        if (Delta_Stardate_() == 0) {
            _offensive_titan_ship_built = 0;
        }

        Increment_Antaran_Resource_Level_();
        Build_Antaran_Offensive_Ships_();
        Build_Antaran_Defensive_Ships_();

        if (Antarans_Should_Attack_()) {
            MOX::_antarans.attack_pressure = 0;

            if (Deploy_Antaran_Ships_()) {
                MOX::_antarans.invasion_stardate = (int)MOX::_stardate;
            }
        }
    }

    int16_t __cdecl Antaran_Defensive_Force_() {
        int16_t total_force = 0;
        for (int32_t i = 0; i < 9; ++i) {
            total_force += (int16_t)(MOX::_antarans.defensive_ships[i] * (uint16_t)(uint8_t)_antaran_ship_cost[i]);
        }
        return total_force;
    }

    int16_t __cdecl Antaran_Total_Resources_() {
        int16_t total_force;

        total_force = Antaran_Defensive_Force_();
        total_force += Antaran_Offensive_Force_();
        total_force += MOX::_antarans.resource_offensive;
        total_force += MOX::_antarans.resource_defensive;

        return total_force;
    }

    bool __cdecl Deploy_Antaran_Ships_() {
        int16_t target_star = Get_Antaran_Target_Star_(EVENTS::Get_Antaran_Victim_());

        _sprintf(MOX::_temp_string, s_ships_by_size);
        for (int i = 0; i < 9; ++i) {
            char buf[16];
            _sprintf(buf, s_2d_fmt, (int)MOX::_antarans.offensive_ships[i]);
            strcat(MOX::_temp_string, buf);
        }

        _sprintf(MOX::_temp_string, s_ships_deployed_by_size);
        for (int i = 0; i < 9; ++i) {
            char buf[16];
            _sprintf(buf, s_2d_fmt, (int)MOX::_antarans.offensive_deployed[i]);
            strcat(MOX::_temp_string, buf);
        }

        int16_t ship_idx = -1;
        int16_t x = 0, y = 0;
        int ship_count = 0;
        bool finished = false;

        while (!finished) {
            int16_t ship_list[2] = { -1, -1 };
            int16_t size_idx = Next_Antaran_Ship_Size_();

            if (size_idx < 0) {
                finished = true;
                continue;
            }

            s_ship_design design;
            INITSHIP::Load_Antaran_Ship_Design_(&design, size_idx);

            if (ship_count == 0) {
                ship_idx = SHIPS::Set_Nonplayer_Ship_XY_(target_star, 8 /* Antarans */, &design, 1);
                if (ship_idx == -1) {
                    return false;
                }
                x = MOX::_ship[ship_idx].x;
                y = MOX::_ship[ship_idx].y;
            } else {
                ship_idx = SHIPMOVE::Create_Nonplayer_Ship_(8 /* Antarans */, &design, x, y);
            }

            if (ship_idx == -1) {
                finished = true;
            } else {
                ship_list[0] = ship_idx;
                MOX::_antarans.offensive_deployed[size_idx]++;

                s_ship_move_info move_info;
                SHIPMOVE::Ships_Try_To_Move_To_(ship_list, target_star, &move_info);

                if (move_info.moving != 0) {
                    SHIPMOVE::Make_Ships_Move_To_(ship_list, target_star, &move_info);
                    s_ship_data* ship = &MOX::_ship[ship_idx];
                    ship->status = 1;
                    ship->location = target_star + 500;
                    ship->crew_quality = 2;
                    ship->crew_experience = MOX::_crew_data[2].experience + 1;
                }
            }

            if (ship_count >= 5 || ship_idx == -1) {
                finished = true;
            }
            ship_count++;
        }

        return true;
    }

    int16_t __cdecl Antaran_Offensive_Force_() {
        int16_t total_force = 0;

        for (int i = 0; i < 9; ++i) {
            // Ship cost is treated as an unsigned byte value (0-255) in the calculation.
            uint8_t ship_cost = static_cast<uint8_t>(_antaran_ship_cost[i]);
            int16_t ship_count = MOX::_antarans.offensive_ships[i];

            total_force += (int16_t)(ship_count * ship_cost);
        }

        return total_force;
    }

    void __cdecl Kill_Antaran_Defensive_Ship_(int16_t ship_idx) {
        int32_t defensive_count = (int32_t)MOX::_antarans.defensive_ships[ship_idx] - 1;

        if (defensive_count < 1) {
            defensive_count = 0;
        }

        MOX::_antarans.defensive_ships[ship_idx] = (int16_t)defensive_count;

        if (MOX::_antarans.offensive_ships[ship_idx] > 0) {
            int32_t offensive_count = (int32_t)MOX::_antarans.offensive_ships[ship_idx] - 1;

            if (offensive_count < 1) {
                offensive_count = 0;
            }

            MOX::_antarans.offensive_ships[ship_idx] = (int16_t)offensive_count;
            MOX::_antarans.defensive_ships[ship_idx]++;
        }
    }

    void __cdecl Kill_Antaran_Offensive_Ship_(int16_t ship_idx) {
        int32_t count = (int32_t)MOX::_antarans.offensive_ships[ship_idx] - 1;

        if (count < 1) {
            count = 0;
        }

        MOX::_antarans.offensive_ships[ship_idx] = (int16_t)count;

        Restock_Antaran_Ship_(ship_idx);
    }

    void __cdecl Restock_Antaran_Ship_(int16_t ship_type_idx) {
        int16_t count = MOX::_antarans.offensive_deployed[ship_type_idx] - 1;

        if (count < 1) {
            count = 0;
        }

        MOX::_antarans.offensive_deployed[ship_type_idx] = count;
    }

    bool __cdecl Antarans_Should_Attack_() {
        Delta_Stardate_();

        bool should_attack = false;

        int16_t defensive_force = Antaran_Total_Resources_();
        int16_t offensive_force = Antaran_Offensive_Force_();

        // Offensive force must be at least 1/4 of defensive force to consider an attack
        if ((int32_t)offensive_force * 4 < (int32_t)defensive_force) {
            return false;
        }

        bool has_undeployed_ships = false;
        for (int i = 0; i < 9; ++i) {
            if (MOX::_antarans.offensive_ships[i] > MOX::_antarans.offensive_deployed[i]) {
                has_undeployed_ships = true;
                break;
            }
        }

        // If Antarans have ships that aren't already deployed elsewhere, build up pressure
        if (has_undeployed_ships) {
            MOX::_antarans.attack_pressure++;
        }

        // If there's built-up pressure, perform a random check to decide on the attack
        if (MOX::_antarans.attack_pressure > 0) {
            int16_t random_val = (int16_t)random::Random_(200) - 1;
            if ((int32_t)MOX::_antarans.attack_pressure + 1 > (int32_t)random_val) {
                should_attack = true;
            }
        }

        return should_attack;
    }
}
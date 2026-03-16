#include "pch.h"
#include "bomb.h"
#include "mox.h"
#include "events.h"
#include "msg.h"
#include "invasion.h"
#include "colbldg.h"
#include "colcalc.h"
#include "officer.h"
#include <cstring>

namespace BOMB {

    int16_t __cdecl Handle_Leader_At_Lost_Colony_(s_colony* colony) {
        int owner_idx = (int)colony->owner;
        int star_idx = (int)MOX::_planet[colony->planet].star_index;
        int16_t killed_leader_idx = -1;
        int16_t leader_idx = (int16_t)MOX::_star[star_idx].officer_index[owner_idx];

        if (leader_idx != -1) {
            bool other_colony_found = false;
            int16_t current_colony_idx = (int16_t)(colony - MOX::_colony);

            for (int i = 0; i < 5; ++i) {
                int16_t p_idx = MOX::_star[star_idx].planet_index[i];
                if (p_idx != -1) {
                    int16_t c_idx = MOX::_planet[p_idx].colony_index;
                    if (c_idx != -1 && MOX::_colony[c_idx].owner == owner_idx && c_idx != current_colony_idx) {
                        other_colony_found = true;
                        break;
                    }
                }
            }

            if (!other_colony_found) {
                if (MOX::_leaders[leader_idx].eta > 0) {
                    OFFICER::Move_Officer_To_Pool_(leader_idx);
                } else {
                    OFFICER::Kill_Officer_(leader_idx);
                    killed_leader_idx = leader_idx;
                }
                MOX::_star[star_idx].officer_index[owner_idx] = -1;
            }
        }

        return killed_leader_idx;
    }

    void __cdecl Destroy_Colony_(s_colony* colony, int32_t capturing_player) {
        s_star_data* star;
        int16_t colony_idx;
        int16_t leader_idx;
        s_MSG msg;

        star = &MOX::_star[MOX::_planet[colony->planet].star_index];
        colony_idx = (int16_t)(colony - MOX::_colony);

        EVENTS::Kill_All_Events_At_Colony_(colony_idx);

        leader_idx = BOMB::Handle_Leader_At_Lost_Colony_(colony);

        if (capturing_player != -1) {
            memset(&msg, 0, sizeof(s_MSG));
            msg.msg_type = 7 /* MSG_TYPE_LOST_COLONY */;
            msg.u.lost_colony.planet_idx = colony->planet;
            // TYPE MISMATCH: using 'reason' from orion2.h for 'player_idx'
            msg.u.lost_colony.reason = (int16_t)capturing_player;
            msg.u.lost_colony.leader_idx = leader_idx;
            // TYPE MISMATCH: using 'colony_or_outpost' from orion2.h for 'outpost_flag'
            msg.u.lost_colony.colony_or_outpost = colony->outpost_flag;
            MSG_::Add_Msg_(msg, colony->owner);
        }

        if (MOX::_player[(uint8_t)colony->owner].home_planet_id == colony->planet) {
            invasion::Assign_New_Home_Planet_(colony->owner, colony_idx);
        }

        COLBLDG::Clear_Colony_Queue_(colony_idx);

        MOX::_planet[colony->planet].colony_index = -1;

        memset(colony, 0, sizeof(s_colony));
        colony->planet = -1;
        colony->owner = -1;

        COLCALC::Compute_Star_Colony_Stuff_(star);
    }

    /**
     * @address 0047e8e0
     */
    int __cdecl Resolve_Bomb_Hit_(s_colony* colony, s_combat_bomb_result* result, int bomb_power) {
        int target_building_count = 0;
        int8_t target_building_indices[BUILDING_COUNT];
        s_player* owner_player = &MOX::_player[(uint8_t)colony->owner];

        for (int i = 0; i < BUILDING_COUNT; ++i) {
            if (colony->buildings[i] != 0 &&
                i != BUILDING_CAPITOL &&
                i != BUILDING_MISSILE_BASE &&
                i != BUILDING_GROUND_BATTERIES &&
                i != BUILDING_STELLAR_CONVERTER &&
                i != BUILDING_FIGHTER_GARRISON &&
                i != BUILDING_BATTLESTATION &&
                i != BUILDING_STAR_BASE &&
                i != BUILDING_STAR_FORTRESS) {
                target_building_indices[target_building_count] = (int8_t)i;
                target_building_count++;
            }
        }

        int total_targets = (int)colony->military[0] + (int)colony->military[1] + target_building_count;
        if (colony->production_spent != 0) {
            total_targets++;
        }

        int result_code = 0;
        if (colony->n_pops == 1) {
            if (total_targets == 0) {
                int pop_idx = colony->pop[0] & 0xF;
                if (colony->pop_roundoff[pop_idx] <= 100) {
                    result->population_killed++;
                    colony->pop_roundoff[pop_idx] = 0;
                    colony->n_pops = 0;

                    if (colony->buildings[BUILDING_CAPITOL] != 0) {
                        int16_t colony_idx = (int16_t)(colony - MOX::_colony);
                        ERICNET::Remove_Building_(colony_idx, BUILDING_CAPITOL);
                    }

                    result->colony_wiped_out = 1;
                    owner_player->colony_war_losses[(uint8_t)result->attacker_idx] += 10;
                } else {
                    colony->pop_roundoff[pop_idx] -= 100;
                }
                result_code = 1;
                result->damage_occurred = 1;
            }
        } else {
            total_targets += (uint8_t)colony->n_pops;
        }

        if (result_code == 0) {
            if (total_targets == 0) {
                return 0;
            }

            int hit_val = random::Random_(total_targets);
            int hit_idx = hit_val - 1;

            if (hit_idx < target_building_count) {
                int8_t building_id = target_building_indices[hit_idx];
                int16_t colony_idx = (int16_t)(colony - MOX::_colony);
                ERICNET::Remove_Building_(colony_idx, (int16_t)building_id);
                result->buildings_destroyed[building_id] = 1;
                result->damage_occurred = 1;
                return 1;
            }

            hit_idx -= target_building_count;
            if (hit_idx < (int)colony->military[0]) {
                int needed_strength = 1;
                if (owner_player->tech_applications[TECH_APP_POWERED_ARMOR] == 3) {
                    needed_strength = 2;
                }
                if (owner_player->traits[TRAIT_HEAVY_G_WORLD] != 0) {
                    needed_strength++;
                }

                if (bomb_power < needed_strength) {
                    return 0;
                }

                colony->military[0]--;
                result->damage_occurred = 1;
                result->infantry_killed++;
                return needed_strength;
            }

            hit_idx -= (int)colony->military[0];
            if (hit_idx < (int)colony->military[1]) {
                int needed_strength = 2;
                if (owner_player->tech_applications[TECH_APP_BATTLEOIDS] == 3) {
                    needed_strength = 3;
                }
                if (owner_player->traits[TRAIT_HEAVY_G_WORLD] != 0) {
                    needed_strength++;
                }

                if (bomb_power < needed_strength) {
                    return 0;
                }

                colony->military[1]--;
                result->damage_occurred = 1;
                result->tanks_killed++;
                return needed_strength;
            }

            hit_idx -= (int)colony->military[1];
            if (hit_idx < (int)colony->production_spent) {
                result->production_lost = colony->production_spent;
                colony->production_spent = 0;
            } else {
                result->population_killed++;
                colony->n_pops--;
                colony->pop[hit_idx] = colony->pop[colony->n_pops];
                owner_player->colony_war_losses[(uint8_t)result->attacker_idx]++;
            }
            result_code = 1;
            result->damage_occurred = 1;
        }

        return result_code;
    }

    /**
     * @address 0047ebd0
     */
    void __cdecl Resolve_Biological_Hit_(s_colony* colony, s_combat_bomb_result* result) {
        int target_pop_idx = -1;
        int non_android_count = 0;

        for (int i = 0; i < (int)(uint8_t)colony->n_pops; ++i) {
            // Check race index; 8 is RACE_ANDROID
            if ((colony->pop[i] & 0xf) != 8) {
                non_android_count++;
                if (random::Random_(non_android_count) == 1) {
                    target_pop_idx = i;
                }
            }
        }

        if (target_pop_idx < 0) {
            return;
        }

        result->damage_occurred = 1;

        if ((uint8_t)colony->n_pops >= 2) {
            int chance_sum = (int)colony->military[0] + (int)colony->military[1] + (int)(uint8_t)colony->n_pops;
            if (random::Random_(chance_sum) <= (int)(uint8_t)colony->n_pops) {
                result->population_killed++;
                result->bio_pop_killed++;

                colony->n_pops--;
                colony->pop[target_pop_idx] = colony->pop[(uint8_t)colony->n_pops];

                MOX::_player[(uint8_t)colony->owner].colony_war_losses[(uint8_t)result->attacker_idx]++;
                return;
            }

            if (colony->military[0] != 0) {
                if (random::Random_((int)colony->military[1] + (int)colony->military[0]) <= (int)colony->military[0]) {
                    colony->military[0]--;
                    result->infantry_killed++;
                    return;
                }
            }

            colony->military[1]--;
            result->tanks_killed++;
            return;
        }

        // Single population colony logic
        if (colony->military[0] != 0) {
            if (random::Random_((int)colony->military[1] + (int)colony->military[0]) <= (int)colony->military[0]) {
                colony->military[0]--;
                result->infantry_killed++;
                return;
            }
        }

        if (colony->military[1] != 0) {
            colony->military[1]--;
            result->tanks_killed++;
            return;
        }

        int race_idx = (int)(colony->pop[0] & 0xf);
        // BUG: colony->pop_roundoff has 10 elements in orion2.h but 16 are possible here
        if (colony->pop_roundoff[race_idx] <= 100) {
            result->population_killed++;
            result->bio_pop_killed++;

            colony->pop_roundoff[race_idx] = 0;
            colony->n_pops = 0;

            bool has_buildings = false;
            for (int i = 0; i < 49; ++i) {
                if (colony->buildings[i] != 0) {
                    has_buildings = true;
                    break;
                }
            }

            if (!has_buildings) {
                result->colony_wiped_out = 1;
                MOX::_player[(uint8_t)colony->owner].colony_war_losses[(uint8_t)result->attacker_idx] += 10;
            }
        } else {
            race_idx = (int)(colony->pop[target_pop_idx] & 0xf);
            colony->pop_roundoff[race_idx] -= 100;
        }
    }

    /**
     * @address 0047edc0
     */
    void __cdecl Apply_Bomb_Hits_(s_combat_bomb_result* result) {
        int32_t bio_hits = result->bio_hits_remaining;
        int32_t bomb_hits = result->bomb_hits_remaining;
        s_colony* colony = &MOX::_colony[result->colony_idx];

        while (bio_hits + bomb_hits > 0) {
            if (bio_hits != 0) {
                bio_hits--;
                BOMB::Resolve_Biological_Hit_(colony, result);
            } else {
                int hits_dealt = BOMB::Resolve_Bomb_Hit_(colony, result, (int16_t)bomb_hits);
                if (hits_dealt == 0) {
                    bomb_hits = 0;
                } else {
                    bomb_hits -= hits_dealt;
                    result->total_hits_processed += hits_dealt;
                }
            }
        }
    }

} // namespace BOMB
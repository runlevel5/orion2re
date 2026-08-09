#include "pch.h"

namespace COLCALC {
    int32_t __cdecl Production_Cost_To_Buy_(int32_t product_cost, int32_t production_spent) {
        if (production_spent >= product_cost) {
            return 0;
        }

        const int32_t half_cost = product_cost / 2;
        const int32_t tenth_cost = product_cost / 10;
        if (production_spent >= half_cost) {
            return (product_cost - production_spent) * 2;
        }

        int32_t buy_cost = (product_cost - half_cost) * 2;
        if (production_spent >= tenth_cost) {
            return buy_cost + (half_cost - production_spent) * 5;
        }

        buy_cost += (half_cost - tenth_cost) * 5;
        return buy_cost + (tenth_cost - production_spent) * 10;
    }

    void __cdecl Resolve_Research_Completion_(s_player* player, bool keep_points, int32_t points_spent) {
        player->research_breakthrough = 1;

        if (!keep_points) {
            player->research_accumulated = 0;
            return;
        }

        player->research_accumulated -= points_spent;
        if (player->research_accumulated < 0) {
            player->research_accumulated = 0;
        }
    }

    int16_t __cdecl Colony_Empire_Base_Food2_Produced_(const s_colony* colony, int16_t owner_idx, int16_t player_idx, s_colony_job_production* out) {
        int32_t idx = (int32_t)player_idx;
        int16_t food_per_farmer;

        if (colony->owner == (int8_t)owner_idx && out == nullptr) {
            food_per_farmer = (int16_t)(uint8_t)colony->food2_per_farmer;
        }
        else {
            food_per_farmer = Colony_Food2_Per_Farmer_(colony, owner_idx, out);
        }

        if (food_per_farmer <= 0) {
            return food_per_farmer;
        }

        int16_t trait_bonus = 0;
        int32_t extra_food = 0;

        if (player_idx < MAX_PLAYERS) {
            s_player* player = &MOX::_player[player_idx];

            trait_bonus = (int16_t)player->traits[TRAIT_FOOD_BONUS];

            if (player->traits[TRAIT_AQUATIC] != 0) {
                int climate = (int)colony->climate;
                switch (climate) {
                case PLANET_CLIMATE_TUNDRA:
                case PLANET_CLIMATE_SWAMP:
                    extra_food = ((int32_t)MOX::_food_per_farmer_table[8] - (int32_t)MOX::_food_per_farmer_table[climate]) * 2;
                    break;
                case PLANET_CLIMATE_OCEAN:
                case PLANET_CLIMATE_TERRAN:
                    extra_food = ((int32_t)MOX::_food_per_farmer_table[9] - (int32_t)MOX::_food_per_farmer_table[climate]) * 2;
                    break;
                default:
                    break;
                }
            }
        }
        else if (player_idx == 8) {
            trait_bonus = 6;
        }
        else if (player_idx == 9) {
            trait_bonus = 4;
        }

        if (out != nullptr) {
            if (idx < 10) {
                out->racial_bonuses[idx] = trait_bonus;
            }
            out->food_bonus = extra_food;
        }

        return food_per_farmer + trait_bonus + (int16_t)extra_food;
    }

    int16_t __cdecl Colony_Empire_Base_Industry_Produced_(const s_colony* colony, int16_t type, int16_t player_idx, s_colony_job_production* out) {
        int16_t base_industry;

        if ((int16_t)colony->owner == (int16_t)type && out == nullptr) {
            base_industry = (int16_t)(uint8_t)colony->industry_per_worker;
        }
        else {
            base_industry = Colony_Industry_Per_Worker_(colony, type, out);
        }

        int32_t idx = (int32_t)player_idx;
        int16_t bonus = 0;
        if (player_idx < MAX_PLAYERS) {
            bonus = MOX::_player[idx].traits[TRAIT_PRODUCTION_BONUS];
        }
        else if (player_idx == 8) {
            bonus = 3;
        }

        if (out != nullptr) {
            if (idx < 10) {
                out->racial_bonuses[idx] = bonus;
            }
        }

        return base_industry + bonus;
    }

    int16_t __cdecl Colony_Empire_Base_Research_Produced_(const s_colony* colony, int16_t research_type, int16_t player_idx, s_colony_job_production* out) {
        int32_t idx = (int32_t)player_idx;
        uint16_t base_research_per_scientist;

        if (colony->owner == (int8_t)research_type && out == nullptr) {
            base_research_per_scientist = (uint8_t)colony->research_per_scientist;
        }
        else {
            base_research_per_scientist = Colony_Research_Per_Scientist_(colony, research_type, out);
        }

        int16_t trait_bonus = 0;
        int32_t intelligence_bonus = 0;

        if (player_idx < MAX_PLAYERS) {
            trait_bonus = (int16_t)MOX::_player[idx].traits[TRAIT_RESEARCH_BONUS];

            if (colony->owner == (int8_t)player_idx && MOX::_player[idx].tech_applications[TECH_APP_HEIGHTENED_INTELLIGENCE] == TECH_RESEARCH_STATUS_RESEARCHED) {
                intelligence_bonus = 1;
            }
        }
        else if (player_idx == 8) {
            trait_bonus = 3;
        }

        if (out != nullptr) {
            if (idx < 10) {
                out->racial_bonuses[idx] = trait_bonus;
            }
            out->intelligence_bonus = intelligence_bonus;
        }

        return (int16_t)base_research_per_scientist + trait_bonus + (int16_t)intelligence_bonus;
    }

    void __cdecl Check_For_Research_Breakthrough_(s_player* player) {
        uint8_t field = (uint8_t)player->current_research_field;

        if (field == 0) {
            player->research_accumulated += (int32_t)player->research_produced;
            return;
        }

        int32_t breakthrough_chance = Chance_For_Research_Breakthrough_(player);
        player->research_accumulated += (int32_t)player->research_produced;

        if ((int32_t)game_random::Random_(100) <= breakthrough_chance) {
            int32_t points_spent = 0;
            if (_keep_research_on_breakthrough != 0) {
                points_spent = Player_Research_Cost_(player, field) * 2;
            }
            Resolve_Research_Completion_(
                player, _keep_research_on_breakthrough != 0, points_spent);
            Give_Player_Field_(player, field, 0);
        }
    }
}
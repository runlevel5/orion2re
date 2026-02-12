#include "deps.h"
#include "advciv.h"

namespace ADVCIV {
    bool __cdecl Not_Star_Owner_(int16_t planet_idx, int16_t star_idx) {
        int8_t owner = _star_colony_owner[star_idx];

        // Checks if the star is owned (owner > -1) and if the owner is not the specified planet
        if (owner > -1 && (int16_t)owner != planet_idx) {
            return true;
        }

        return false;
    }

    void __cdecl Init_Advanced_Civilization_Fleet_(int16_t player_index) {
        return;
    }

    void __cdecl Assign_Advanced_Civilization_Starting_Ships_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            Init_Advanced_Civilization_Fleet_(i);
        }
    }

    int32_t __cdecl Uncolonized_Planet_Worth_To_Player_(s_planet_data* planet, s_player* plr) {
        int32_t worth = 0;

        if (planet->planet_type == PLANET_TYPE_PLANET) {
            int16_t subterranean_bonus = 0;
            if (plr->evolutionary_upgrade_traits[TRAIT_SUBTERRANEAN] != 0) {
                subterranean_bonus = COLCALC::Double_Add_Two_(planet->size);
            }

            int16_t player_idx = static_cast<int16_t>(plr - MOX::_player);
            int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(planet, player_idx);

            int32_t total_pop_sum = max_pop + subterranean_bonus;
            if (plr->tech_applications[TECH_APP_ADVANCED_CITY_PLANNING] == 3) {
                total_pop_sum += 5;
            }

            int8_t mineral_bonus = COLCALC::_minerals_per_mine[planet->mineral_class];
            int32_t scaled_pop = (mineral_bonus + 1) * total_pop_sum / 3;

            int8_t pop_growth_bonus = plr->traits[TRAIT_POP_GROWTH_BONUS];
            if (pop_growth_bonus == -50) {
                worth = 80 * mineral_bonus + scaled_pop;
            } else if (pop_growth_bonus == 0) {
                worth = 75 * mineral_bonus + 2 * scaled_pop;
            } else if (pop_growth_bonus == 50) {
                worth = 70 * mineral_bonus + 3 * scaled_pop;
            } else if (pop_growth_bonus == 100) {
                worth = 65 * mineral_bonus + 4 * scaled_pop;
            }

            int32_t food_per_farmer = planet->food_per_farmer;
            bool has_grav_gen_tech = (plr->tech_applications[TECH_APP_PLANETARY_GRAVITY_GENERATOR] == 3);
            if (food_per_farmer == 0 && plr->tech_applications[TECH_APP_BIOMORPHIC_FUNGI] == 3) {
                food_per_farmer = 1;
            }

            if (plr->evolutionary_upgrade_traits[TRAIT_EATS_MINERALS] == 0) {
                if (plr->evolutionary_upgrade_traits[TRAIT_CYBERNETIC] == 0) {
                    worth += food_per_farmer * 150;
                } else {
                    worth += food_per_farmer * 75;
                }
            }

            switch (planet->planet_special) {
            case SYSTEM_SPECIAL_GOLD_DEPOSITS: worth += 1280; break;
            case SYSTEM_SPECIAL_GEM_DEPOSITS: worth += 2560; break;
            case SYSTEM_SPECIAL_ANCIENT_ARTIFACTS: worth += 640; break;
            case SYSTEM_SPECIAL_ORION_STAR: worth += 1600; break;
            default: break;
            }

            worth = (mineral_bonus + 7) * total_pop_sum * worth / 10;

            bool is_swamp_ocean = (planet->climate == PLANET_CLIMAGE_OCEAN || planet->climate == PLANET_CLIMAGE_SWAMP);
            bool is_aquatic = (plr->evolutionary_upgrade_traits[TRAIT_AQUATIC] != 0);

            if (!is_swamp_ocean || !is_aquatic) {
                int32_t maintenance_reduction = COLCALC::_climate_maintenance_modifier[planet->climate] >> 2;
                worth = worth * (100 - maintenance_reduction) / 100;
                if (is_swamp_ocean) {
                    worth = (worth * 3) / 4;
                }
            }

            int32_t divisor = 1;
            int32_t penalty_factor = has_grav_gen_tech ? 1 : 0;
            bool apply_penalty = false;

            if (planet->gravity_class == PLANET_GRAVITY_LOW_G) {
                if (plr->evolutionary_upgrade_traits[TRAIT_LOW_G_WORLD] == 0) {
                    divisor = 20;
                    penalty_factor += 18;
                    apply_penalty = true;
                }
            } else if (planet->gravity_class == PLANET_GRAVITY_NORMAL_G) {
                if (plr->evolutionary_upgrade_traits[TRAIT_LOW_G_WORLD] != 0) {
                    divisor = 20;
                    penalty_factor += 18;
                    apply_penalty = true;
                }
            } else if (planet->gravity_class == PLANET_GRAVITY_HEAVY_G) {
                if (plr->evolutionary_upgrade_traits[TRAIT_HEAVY_G_WORLD] == 0) {
                    divisor = 6;
                    penalty_factor += 4;
                    apply_penalty = true;
                }
            }

            if (apply_penalty) {
                worth = (worth * penalty_factor) / divisor;
            }
        }

        int32_t final_worth = worth >> 6;
        if (final_worth > 0xFFFE) {
            final_worth = 0xFFFF;
        }
        return final_worth;
    }
}
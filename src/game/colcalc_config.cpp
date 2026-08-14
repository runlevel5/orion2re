#include "pch.h"

namespace COLCALC {
    s_command_points command_points = {
        5,
        1,
        2,
        3,
        1,
        2,
        3,
        2,
    };

    uint8_t ship_command_points_usage[SHIP_SIZE_COUNT] = {
        1, 2, 3, 4, 5, 6,
    };

    s_government_bonus government_bonuses[GOVERNMENT_COUNT] = {
        { { 1, 3 }, -10, 0, 0, 0 },
        { { 2, 3 }, -5, 0, 0, 0 },
        { { 0, 1 }, 0, 0, 0, 0 },
        { { 0, 1 }, 0, 0, 0, 0 },
        { { 0, 1 }, 10, 10, 0, 0 },
        { { 0, 1 }, 15, 15, 0, 0 },
        { { 0, 1 }, 0, 0, 10, 10 },
        { { 0, 1 }, 0, 0, 20, 20 },
    };

    s_lander_ship_cost lander_ship_cost = {
        500,
        100,
        100,
    };

    s_misc_costs misc_costs = {
        50,
        50,
        50,
        50,
    };

    s_growth_rate_bonus growth_rate_bonus = {
        0,
        25,
        50,
        100,
        -200,
    };

    s_trade_goods_ratio trade_goods_ratio = {
        50,
        100,
    };

    uint8_t _pop_bonus_biospheres = 2;
    uint8_t _pop_bonus_subterranean[PLANET_SIZE_COUNT] = {
        2, 4, 6, 8, 10,
    };
    uint8_t weather_control_climate = PLANET_CLIMATE_TOXIC;
    uint8_t _aquatic_treats_tundra_as_swamp = 0;
    uint8_t _federation_no_mixed_races_penalty = 0;
    uint8_t _housing_creates_no_farmers = 0;
    uint8_t _planet_extinction = 0;
    uint8_t _zero_colony_lost = 0;
    uint8_t _feudal_affects_freighters = 0;
    uint8_t _keep_research_on_breakthrough = 0;
    uint8_t _keep_research_on_finds = 0;
    uint8_t scout_lab_research_bonus[SHIP_SIZE_COUNT] = {
        1, 2, 4, 8, 16, 32,
    };
    uint8_t _replicator_food_pp = 2;
    uint8_t _replicator_food_bc = 1;
    uint8_t _freighters_cash_bonus = 0;

    int __cdecl Calc_Player_Climate_(const s_player* player, int climate) {
        if (player != nullptr && player->traits[TRAIT_AQUATIC] != 0) {
            switch (climate) {
                case PLANET_CLIMATE_TUNDRA:
                    return _aquatic_treats_tundra_as_swamp != 0
                        ? PLANET_CLIMATE_SWAMP
                        : PLANET_CLIMATE_TERRAN;
                case PLANET_CLIMATE_SWAMP:
                    return PLANET_CLIMATE_TERRAN;
                case PLANET_CLIMATE_OCEAN:
                case PLANET_CLIMATE_TERRAN:
                    return PLANET_CLIMATE_GAIA;
                default:
                    break;
            }
        }
        return climate;
    }
}
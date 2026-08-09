#include "pch.h"

namespace DESIGN {
    static_assert(MINIATURIZATION_TECHNOLOGY_COUNT == TECH_APP_COUNT);

    uint8_t _missile_x3_shot = 0;

    static const int16_t missile_rack_quantities[2][MISSILE_RACK_COUNT] = {
        { 2, 5, 10, 15, 20 },
        { 2, 3, 5, 10, 20 },
    };
    static const int16_t missile_rack_sizes[MISSILE_RACK_COUNT] = {
        10, 20, 30, 35, 40,
    };

    s_miniaturization_settings miniaturization = {
        { 100, 75, 55, 40, 30, 25 },
        {
            { 1000, 800, 650, 500, 350, 250 },
            { 1000, 800, 700, 600, 500, 400 },
            { 1000, 1000, 1000, 1000, 1000, 1000 },
        },
    };

    uint8_t miniaturization_categories[MINIATURIZATION_TECHNOLOGY_COUNT] = {
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 2, 0, 0, 2, 0, 0, 1,
        0, 0, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 2, 1, 1, 0,
        1, 0, 0, 2, 1, 0, 2, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2,
        0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
        0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0,
        0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
        2, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

    int16_t Missile_Rack_Quantity_(int16_t rack_index) {
        if (rack_index < 0 || rack_index >= MISSILE_RACK_COUNT) {
            return 0;
        }
        return missile_rack_quantities[_missile_x3_shot != 0][rack_index];
    }

    int16_t Missile_Rack_Index_(int16_t quantity) {
        for (int16_t rack_index = 0; rack_index < MISSILE_RACK_COUNT; ++rack_index) {
            if (Missile_Rack_Quantity_(rack_index) == quantity) {
                return rack_index;
            }
        }
        return -1;
    }

    int16_t Missile_Rack_Size_(int16_t quantity) {
        int16_t rack_index = Missile_Rack_Index_(quantity);
        return rack_index >= 0 ? missile_rack_sizes[rack_index] : 0;
    }

    bool Should_Autodesign_Use_X3_Rack_(
        int16_t quantity, int16_t fitting_weapon_count, int16_t minimum_weapon_count) {
        return _missile_x3_shot != 0 && quantity == 5 &&
            fitting_weapon_count <= minimum_weapon_count;
    }

    uint16_t Miniaturization_Cost_Multiplier_(int16_t level) {
        int index = level >= 0 && level < MINIATURIZATION_LEVEL_COUNT - 1
            ? level
            : MINIATURIZATION_LEVEL_COUNT - 1;
        return miniaturization.cost[index];
    }

    uint16_t Miniaturization_Space_Multiplier_(int16_t level, int8_t category) {
        int level_index = level >= 0 && level < MINIATURIZATION_LEVEL_COUNT - 1
            ? level
            : MINIATURIZATION_LEVEL_COUNT - 1;
        int category_index = category >= 0 && category < MINIATURIZATION_CATEGORY_COUNT
            ? category
            : MINIATURIZATION_CATEGORY_GROUP_3;
        return miniaturization.space[category_index][level_index];
    }
}
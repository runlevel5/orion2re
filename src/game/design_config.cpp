#include "pch.h"

namespace DESIGN {
    static_assert(MINIATURIZATION_TECHNOLOGY_COUNT == TECH_APP_COUNT);

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
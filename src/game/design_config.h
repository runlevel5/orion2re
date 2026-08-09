#pragma once

#include <cstdint>

namespace DESIGN {
    enum e_miniaturization_category : uint8_t {
        MINIATURIZATION_CATEGORY_GROUP_1 = 0,
        MINIATURIZATION_CATEGORY_GROUP_2 = 1,
        MINIATURIZATION_CATEGORY_GROUP_3 = 2,
        MINIATURIZATION_CATEGORY_COUNT = 3,
    };

    constexpr int MINIATURIZATION_LEVEL_COUNT = 6;
    constexpr int MINIATURIZATION_TECHNOLOGY_COUNT = 212;
    constexpr int MISSILE_RACK_COUNT = 5;

    struct s_miniaturization_settings {
        uint16_t cost[MINIATURIZATION_LEVEL_COUNT];
        uint16_t space[MINIATURIZATION_CATEGORY_COUNT][MINIATURIZATION_LEVEL_COUNT];
    };

    extern s_miniaturization_settings miniaturization;
    extern uint8_t miniaturization_categories[MINIATURIZATION_TECHNOLOGY_COUNT];
    extern uint8_t _missile_x3_shot;

    uint16_t Miniaturization_Cost_Multiplier_(int16_t level);
    uint16_t Miniaturization_Space_Multiplier_(int16_t level, int8_t category);
    int16_t Missile_Rack_Quantity_(int16_t rack_index);
    int16_t Missile_Rack_Index_(int16_t quantity);
    int16_t Missile_Rack_Size_(int16_t quantity);
    bool Should_Autodesign_Use_X3_Rack_(
        int16_t quantity, int16_t fitting_weapon_count, int16_t minimum_weapon_count);
}
#include "pch.h"

#include "doctest/doctest.h"
#include "game/invasion.h"

namespace test_ground_commando_runtime {
    void Reset_();
    s_colony* Colony_();
    s_leader_data* Leader_(int index);
    s_ship_data* Ship_(int index);
    void Set_Colony_Officer_(int leader_idx);
    void Set_Officer_Level_(int16_t level);
}

DOCTEST_TEST_CASE("colony ground combat info preserves the resolved Commando officer") {
    test_ground_commando_runtime::Reset_();
    test_ground_commando_runtime::Set_Colony_Officer_(1);
    test_ground_commando_runtime::Set_Officer_Level_(1);

    s_colony* colony = test_ground_commando_runtime::Colony_();
    colony->owner = 0;
    colony->officer = 0;
    colony->military[0] = 1;

    s_leader_data* leader = test_ground_commando_runtime::Leader_(1);
    leader->general_skills = LEADER_GENERAL_SKILL_COMMANDO;

    invasion::_ground_commando_defender_x5 = 0;
    s_invasion_side_info side = {};
    invasion::Compute_Colony_Ground_Combat_Info_(colony, &side);

    DOCTEST_CHECK(side.combat_modifiers.officer_bonus == 4);
    DOCTEST_CHECK(side.combat_modifiers.officer_idx == 1);
}

DOCTEST_TEST_CASE("defending ground Commando setting selects the configured multipliers") {
    test_ground_commando_runtime::Reset_();
    test_ground_commando_runtime::Set_Colony_Officer_(0);
    s_colony colony = {};
    colony.officer = 0;
    s_invasion_player_bonuses bonuses = {};

    s_leader_data* leader = test_ground_commando_runtime::Leader_(0);
    leader->general_skills = LEADER_GENERAL_SKILL_COMMANDO;
    test_ground_commando_runtime::Set_Officer_Level_(1);

    invasion::_ground_commando_defender_x5 = 0;
    invasion::Compute_Colony_Officer_Bonus_(&colony, &bonuses);
    DOCTEST_CHECK(bonuses.officer_bonus == 4);

    bonuses = {};
    invasion::_ground_commando_defender_x5 = 1;
    invasion::Compute_Colony_Officer_Bonus_(&colony, &bonuses);
    DOCTEST_CHECK(bonuses.officer_bonus == 10);

    bonuses = {};
    leader->general_skills = LEADER_GENERAL_SKILL_COMMANDO2;
    test_ground_commando_runtime::Set_Officer_Level_(2);
    invasion::_ground_commando_defender_x5 = 0;
    invasion::Compute_Colony_Officer_Bonus_(&colony, &bonuses);
    DOCTEST_CHECK(bonuses.officer_bonus == 9);

    bonuses = {};
    invasion::_ground_commando_defender_x5 = 1;
    invasion::Compute_Colony_Officer_Bonus_(&colony, &bonuses);
    DOCTEST_CHECK(bonuses.officer_bonus == 22);
}

DOCTEST_TEST_CASE("attacking ground Commando setting selects the configured multipliers") {
    test_ground_commando_runtime::Reset_();
    int16_t ship_indexes[] = {0};
    s_invasion_player_bonuses bonuses = {};

    s_ship_data* ship = test_ground_commando_runtime::Ship_(0);
    ship->officer_index = 0;
    s_leader_data* leader = test_ground_commando_runtime::Leader_(0);
    leader->eta = 0;
    leader->general_skills = LEADER_GENERAL_SKILL_COMMANDO;
    test_ground_commando_runtime::Set_Officer_Level_(1);

    invasion::_ground_commando_attacker_x2 = 0;
    invasion::Compute_Attacker_Officer_Bonus_(ship_indexes, 1, &bonuses);
    DOCTEST_CHECK(bonuses.officer_bonus == 10);

    bonuses = {};
    invasion::_ground_commando_attacker_x2 = 1;
    invasion::Compute_Attacker_Officer_Bonus_(ship_indexes, 1, &bonuses);
    DOCTEST_CHECK(bonuses.officer_bonus == 4);

    bonuses = {};
    leader->general_skills = LEADER_GENERAL_SKILL_COMMANDO2;
    test_ground_commando_runtime::Set_Officer_Level_(2);
    invasion::_ground_commando_attacker_x2 = 0;
    invasion::Compute_Attacker_Officer_Bonus_(ship_indexes, 1, &bonuses);
    DOCTEST_CHECK(bonuses.officer_bonus == 23);

    bonuses = {};
    invasion::_ground_commando_attacker_x2 = 1;
    invasion::Compute_Attacker_Officer_Bonus_(ship_indexes, 1, &bonuses);
    DOCTEST_CHECK(bonuses.officer_bonus == 9);
}

DOCTEST_TEST_CASE("leader Warlord bonus applies at every XP tier when enabled") {
    struct s_level_case {
        int16_t xp;
        int16_t base_level;
    };
    const s_level_case cases[] = {
        {0, 0},
        {60, 1},
        {150, 2},
        {300, 3},
        {500, 4},
        {1000, 4},
    };

    OFFICER::_leader_warlord_bonus = 0;
    for (const s_level_case& test_case : cases) {
        DOCTEST_CHECK(OFFICER::Get_Officer_Base_Level_(test_case.xp, 0) ==
                      test_case.base_level);
        DOCTEST_CHECK(OFFICER::Get_Officer_Base_Level_(test_case.xp, 1) ==
                      test_case.base_level + (test_case.xp >= 1000 ? 1 : 0));
    }

    OFFICER::_leader_warlord_bonus = 1;
    for (const s_level_case& test_case : cases) {
        DOCTEST_CHECK(OFFICER::Get_Officer_Base_Level_(test_case.xp, 1) ==
                      test_case.base_level + 1);
    }
}

DOCTEST_TEST_CASE("leader XP advancement ignores the effective Warlord bonus") {
    struct s_advance_case {
        int16_t initial_xp;
        int16_t expected_xp;
    };
    const s_advance_case cases[] = {
        {0, 60},
        {60, 150},
        {150, 300},
        {300, 500},
        {500, 1000},
        {1000, 1000},
    };

    test_ground_commando_runtime::Reset_();
    OFFICER::_leader_warlord_bonus = 1;
    for (const s_advance_case& test_case : cases) {
        s_leader_data* leader = test_ground_commando_runtime::Leader_(0);
        leader->xp = test_case.initial_xp;
        OFFICER::Increase_Officer_Level_By_One_(0);
        DOCTEST_CHECK(leader->xp == test_case.expected_xp);
    }
}
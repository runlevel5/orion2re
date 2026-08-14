#include "pch.h"

#include "doctest/doctest.h"

DOCTEST_TEST_CASE("technology spy bonuses include configured Xeno Psychology") {
    spy::spy_tech_bonus = {10, 0, 5, 10, 10, 10};
    s_player player = {};
    player.tech_applications[TECH_APP_XENO_PSYCHOLOGY] =
        TECH_RESEARCH_STATUS_RESEARCHED;

    DOCTEST_CHECK(spy::Tech_Spy_Bonus_(&player) == 0);

    spy::spy_tech_bonus.xeno_psychology = 7;
    DOCTEST_CHECK(spy::Tech_Spy_Bonus_(&player) == 7);

    spy::spy_tech_bonus = {10, 0, 5, 10, 10, 10};
}

DOCTEST_TEST_CASE("technology spy bonus uses every configured field") {
    spy::spy_tech_bonus = {10, 0, 5, 10, 10, 10};
    s_player player = {};
    player.tech_applications[TECH_APP_NEURAL_SCANNER] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    player.tech_applications[TECH_APP_XENO_PSYCHOLOGY] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    player.tech_applications[TECH_APP_TELEPATHIC_TRAINING] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    player.tech_applications[TECH_APP_CYBER_SECURITY_LINK] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    player.tech_applications[TECH_APP_STEALTH_SUIT] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    player.tech_applications[TECH_APP_PSIONICS] =
        TECH_RESEARCH_STATUS_RESEARCHED;

    DOCTEST_CHECK(spy::Tech_Spy_Bonus_(&player) == 45);

    spy::spy_tech_bonus = {1, 2, 3, 4, 5, 6};
    DOCTEST_CHECK(spy::Tech_Spy_Bonus_(&player) == 21);

    spy::spy_tech_bonus = {10, 0, 5, 10, 10, 10};
}
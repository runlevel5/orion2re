#include "pch.h"

#include "doctest/doctest.h"
#include "game/eric.h"

namespace test_eric_runtime {
    void Reset_();
    s_leader_data* Defender_Leader_();
    int16_t Last_Help_String_();
}

DOCTEST_TEST_CASE("Telepath leaders protect colonies from mind control") {
    const uint32_t telepath_skills[] = {
        LEADER_GENERAL_SKILL_TELEPATH,
        LEADER_GENERAL_SKILL_TELEPATH2,
    };

    for (uint32_t skill : telepath_skills) {
        test_eric_runtime::Reset_();
        test_eric_runtime::Defender_Leader_()->general_skills = skill;

        DOCTEST_CHECK(ERIC::Player_Can_Mind_Control_Colony_(0, 0) == 0);
        DOCTEST_CHECK(ERIC::Player_Can_Mind_Control_Colony_With_Help_(0, 0) == 0);
        DOCTEST_CHECK(test_eric_runtime::Last_Help_String_() == 0x200);
    }
}

DOCTEST_TEST_CASE("Assassin leaders do not protect colonies from mind control") {
    test_eric_runtime::Reset_();
    test_eric_runtime::Defender_Leader_()->general_skills =
        LEADER_GENERAL_SKILL_ASSASSIN;

    DOCTEST_CHECK(ERIC::Player_Can_Mind_Control_Colony_(0, 0) == 1);
    DOCTEST_CHECK(ERIC::Player_Can_Mind_Control_Colony_With_Help_(0, 0) == 1);
    DOCTEST_CHECK(test_eric_runtime::Last_Help_String_() == -1);
}
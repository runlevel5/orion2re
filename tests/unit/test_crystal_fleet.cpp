#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtfire.h"

namespace TEST_CRYSTAL_FLEET {
    void Reset_();
    int16_t Cleanup_Count_();
    int16_t Cleanup_Source_(int16_t index);
}

DOCTEST_TEST_CASE("destroyed Crystal releases captures and invalidates effects") {
    TEST_CRYSTAL_FLEET::Reset_();

    MOX::_combat_data[1].owner = 11;
    MOX::_combat_data[1].previous_owner = 11;

    MOX::_combat_data[2].owner = 11;
    MOX::_combat_data[2].previous_owner = 0;
    MOX::_combat_data[2].ship_image_seg =
        reinterpret_cast<s_animation_header*>(1);

    CMBTFIRE::Destroy_Ship_(1, 0, 0, 1);

    DOCTEST_CHECK(MOX::_combat_data[2].owner == 0);
    DOCTEST_CHECK(MOX::_combat_data[2].ship_image_seg == nullptr);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Count_() == 2);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Source_(0) == 1);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Source_(1) == 2);
}
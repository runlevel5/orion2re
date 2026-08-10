#include "pch.h"

#include "doctest/doctest.h"

namespace TEST_HOMEGEN {
    void Reset_();
    int32_t Satellite_Generation_Count_();
    void Set_Generated_Spectral_Class_(int8_t spectral_class);
    int32_t Non_Black_Hole_Generation_Count_();
}

DOCTEST_TEST_CASE("minimum-distance planet generation preserves existing satellite records") {
    TEST_HOMEGEN::Reset_();
    int16_t home_stars[1] = {0};

    HOMEGEN::Guarantee_A_Planet_At_Min_Dist_(0, 4, home_stars);

    DOCTEST_CHECK(MOX::_planet_count == 2);
    DOCTEST_CHECK(MOX::_star[1].planet_index[2] == 0);
    DOCTEST_CHECK(MOX::_planet[0].star_index == 1);
    DOCTEST_CHECK(MOX::_planet[0].planet_type == PLANET_TYPE_PLANET);
    DOCTEST_CHECK(TEST_HOMEGEN::Satellite_Generation_Count_() == 2);
    DOCTEST_CHECK(TEST_HOMEGEN::Non_Black_Hole_Generation_Count_() == 0);
}

DOCTEST_TEST_CASE("minimum-distance generation converts black holes through galaxy-age spectral generation") {
    TEST_HOMEGEN::Reset_();
    TEST_HOMEGEN::Set_Generated_Spectral_Class_(STAR_CLASS_G);
    MOX::_star[1].spectral_class = STAR_CLASS_BLACK_HOLE;
    int16_t home_stars[1] = {0};

    HOMEGEN::Guarantee_A_Planet_At_Min_Dist_(0, 4, home_stars);

    DOCTEST_CHECK(MOX::_star[1].spectral_class == STAR_CLASS_G);
    DOCTEST_CHECK(MOX::_planet_count == 2);
    DOCTEST_CHECK(TEST_HOMEGEN::Satellite_Generation_Count_() == 2);
    DOCTEST_CHECK(TEST_HOMEGEN::Non_Black_Hole_Generation_Count_() == 1);
}

DOCTEST_TEST_CASE("food-world generation rerolls unsuitable stars without allowing black holes") {
    TEST_HOMEGEN::Reset_();
    TEST_HOMEGEN::Set_Generated_Spectral_Class_(STAR_CLASS_K);
    MOX::_star[0].spectral_class = STAR_CLASS_DWARF;

    HOMEGEN::Make_One_Planet_(0, 1, 0, 1);

    DOCTEST_CHECK(MOX::_star[0].spectral_class == STAR_CLASS_K);
    DOCTEST_CHECK(MOX::_star[0].planet_index[0] == 1);
    DOCTEST_CHECK(TEST_HOMEGEN::Non_Black_Hole_Generation_Count_() == 1);
}
#include "pch.h"

#include "doctest/doctest.h"

DOCTEST_TEST_SUITE("map scale") {

DOCTEST_TEST_CASE("extended scaling starts above the original star capacity") {
    DOCTEST_CHECK_FALSE(MAP_SCALE::Uses_Extended_Scaling_(72, 72));
    DOCTEST_CHECK(MAP_SCALE::Uses_Extended_Scaling_(73, 72));
}

DOCTEST_TEST_CASE("extended zoom scales span the full generated map") {
    DOCTEST_CHECK(MAP_SCALE::Extended_Scale_For_Zoom_Level_(100, 3, 0) == 13);
    DOCTEST_CHECK(MAP_SCALE::Extended_Scale_For_Zoom_Level_(100, 3, 1) == 25);
    DOCTEST_CHECK(MAP_SCALE::Extended_Scale_For_Zoom_Level_(100, 3, 2) == 50);
    DOCTEST_CHECK(MAP_SCALE::Extended_Scale_For_Zoom_Level_(100, 3, 3) == 100);

    DOCTEST_CHECK(MAP_SCALE::Extended_Zoom_Level_For_Scale_(24, 100, 3) == 0);
    DOCTEST_CHECK(MAP_SCALE::Extended_Zoom_Level_For_Scale_(25, 100, 3) == 1);
    DOCTEST_CHECK(MAP_SCALE::Extended_Zoom_Level_For_Scale_(50, 100, 3) == 2);
    DOCTEST_CHECK(MAP_SCALE::Extended_Zoom_Level_For_Scale_(100, 100, 3) == 3);
}

DOCTEST_TEST_CASE("star scaling preserves legacy galaxies and close zoom levels") {
    DOCTEST_CHECK(MAP_SCALE::Star_Scale_Percent_(72, 72, 100) == 100);
    DOCTEST_CHECK(MAP_SCALE::Star_Scale_Percent_(73, 72, 30) == 100);
    DOCTEST_CHECK(MAP_SCALE::Star_Scale_Percent_(73, 72, 60) == 50);
    DOCTEST_CHECK(MAP_SCALE::Star_Scale_Percent_(73, 72, 100) == 30);

    DOCTEST_CHECK(MAP_SCALE::Scale_Star_Dimension_(33, 100) == 33);
    DOCTEST_CHECK(MAP_SCALE::Scale_Star_Dimension_(2, 100) == 2);
    DOCTEST_CHECK(MAP_SCALE::Scale_Star_Dimension_(33, 50) == 16);
    DOCTEST_CHECK(MAP_SCALE::Scale_Star_Dimension_(8, 30) == 3);
}

DOCTEST_TEST_CASE("maximum extended zoom enables the overview and delays black hole animation") {
    DOCTEST_CHECK_FALSE(MAP_SCALE::Is_Extended_Max_Map_View_(72, 72, 100, 100));
    DOCTEST_CHECK_FALSE(MAP_SCALE::Is_Extended_Max_Map_View_(73, 72, 50, 100));
    DOCTEST_CHECK(MAP_SCALE::Is_Extended_Max_Map_View_(73, 72, 100, 100));

    DOCTEST_CHECK(MAP_SCALE::Advance_Black_Hole_Animation_(100));
    DOCTEST_CHECK_FALSE(MAP_SCALE::Advance_Black_Hole_Animation_(99));
}

DOCTEST_TEST_CASE("maximum zoom influence overlay can replace thorium range") {
    DOCTEST_CHECK(MAP_SCALE::Influence_Overlay_Range_(255, 12, true) == 12);
    DOCTEST_CHECK(MAP_SCALE::Influence_Overlay_Range_(9, 12, true) == 9);
    DOCTEST_CHECK(MAP_SCALE::Influence_Overlay_Range_(255, 12, false) == 255);
    DOCTEST_CHECK(MAP_SCALE::Influence_Overlay_Range_(9, 12, false) == 9);
}

}
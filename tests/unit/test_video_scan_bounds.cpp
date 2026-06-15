#include "doctest/doctest.h"

#include "game/video_scan_bounds.h"

DOCTEST_TEST_SUITE("video scan bounds") {

DOCTEST_TEST_CASE("onscreen dot helper accepts only coordinates inside the frame") {
    DOCTEST_CHECK(video::Is_Scan_Dot_Onscreen_(0, 0, 640, 480));
    DOCTEST_CHECK(video::Is_Scan_Dot_Onscreen_(639, 479, 640, 480));
    DOCTEST_CHECK_FALSE(video::Is_Scan_Dot_Onscreen_(-1, 0, 640, 480));
    DOCTEST_CHECK_FALSE(video::Is_Scan_Dot_Onscreen_(640, 0, 640, 480));
    DOCTEST_CHECK_FALSE(video::Is_Scan_Dot_Onscreen_(0, -1, 640, 480));
    DOCTEST_CHECK_FALSE(video::Is_Scan_Dot_Onscreen_(0, 480, 640, 480));
}

DOCTEST_TEST_CASE("scan span normalization clamps to the valid dword range") {
    int32_t min_x = -1;
    int32_t max_x = -1;

    DOCTEST_REQUIRE(video::Normalize_Scan_Span_(-5, 200, 160, &min_x, &max_x));
    DOCTEST_CHECK(min_x == 0);
    DOCTEST_CHECK(max_x == 159);
}

DOCTEST_TEST_CASE("scan span normalization rejects inverted or invalid spans") {
    int32_t min_x = 11;
    int32_t max_x = 22;

    DOCTEST_CHECK_FALSE(video::Normalize_Scan_Span_(200, 10, 160, &min_x, &max_x));
    DOCTEST_CHECK_FALSE(video::Normalize_Scan_Span_(0, 10, 0, &min_x, &max_x));
    DOCTEST_CHECK_FALSE(video::Normalize_Scan_Span_(0, 10, 160, nullptr, &max_x));
    DOCTEST_CHECK(min_x == 11);
    DOCTEST_CHECK(max_x == 22);
}

}
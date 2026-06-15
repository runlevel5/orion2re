#include "doctest/doctest.h"

#include "game/compat.h"
#include "game/video_scan_bounds.h"

namespace video {
    struct s_scan_list_entry {
        int16_t min_x;
        int16_t max_x;
    };

    extern int32_t g_screen_res_x;
    extern int32_t g_screen_res_y;
    extern int32_t screen_refresh_flag;
    extern s_scan_list_entry* _refresh_scan_list;
    extern s_scan_list_entry* _refresh_stencil_list;
}

namespace setvesa {
    void __cdecl Add_Square_To_Scan_List_(int32_t left, int32_t top, int32_t right, int32_t bottom);
}

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

DOCTEST_TEST_CASE("rectangle scan list writer keeps clipped top-left pixel rectangles") {
    video::s_scan_list_entry refresh_rows[2] = {{-1, -1}, {-1, -1}};
    video::s_scan_list_entry stencil_rows[2] = {{-1, -1}, {-1, -1}};

    auto* const saved_refresh_scan_list = video::_refresh_scan_list;
    auto* const saved_refresh_stencil_list = video::_refresh_stencil_list;
    const int32_t saved_screen_res_x = video::g_screen_res_x;
    const int32_t saved_screen_res_y = video::g_screen_res_y;
    const int32_t saved_screen_refresh_flag = video::screen_refresh_flag;

    video::_refresh_scan_list = refresh_rows;
    video::_refresh_stencil_list = stencil_rows;
    video::g_screen_res_x = 640;
    video::g_screen_res_y = 480;
    video::screen_refresh_flag = 0;

    setvesa::Add_Square_To_Scan_List_(-5, -5, 0, 0);

    DOCTEST_CHECK(refresh_rows[0].min_x == 0);
    DOCTEST_CHECK(refresh_rows[0].max_x == 0);
    DOCTEST_CHECK(stencil_rows[0].min_x == 0);
    DOCTEST_CHECK(stencil_rows[0].max_x == 0);

    video::_refresh_scan_list = saved_refresh_scan_list;
    video::_refresh_stencil_list = saved_refresh_stencil_list;
    video::g_screen_res_x = saved_screen_res_x;
    video::g_screen_res_y = saved_screen_res_y;
    video::screen_refresh_flag = saved_screen_refresh_flag;
}

DOCTEST_TEST_CASE("rectangle scan list writer ignores inverted clipped spans") {
    video::s_scan_list_entry refresh_rows[2] = {{-1, -1}, {-1, -1}};
    video::s_scan_list_entry stencil_rows[2] = {{-1, -1}, {-1, -1}};

    auto* const saved_refresh_scan_list = video::_refresh_scan_list;
    auto* const saved_refresh_stencil_list = video::_refresh_stencil_list;
    const int32_t saved_screen_res_x = video::g_screen_res_x;
    const int32_t saved_screen_res_y = video::g_screen_res_y;
    const int32_t saved_screen_refresh_flag = video::screen_refresh_flag;

    video::_refresh_scan_list = refresh_rows;
    video::_refresh_stencil_list = stencil_rows;
    video::g_screen_res_x = 640;
    video::g_screen_res_y = 480;
    video::screen_refresh_flag = 0;

    setvesa::Add_Square_To_Scan_List_(12, 0, 8, 0);

    DOCTEST_CHECK(refresh_rows[0].min_x == -1);
    DOCTEST_CHECK(refresh_rows[0].max_x == -1);
    DOCTEST_CHECK(stencil_rows[0].min_x == -1);
    DOCTEST_CHECK(stencil_rows[0].max_x == -1);

    video::_refresh_scan_list = saved_refresh_scan_list;
    video::_refresh_stencil_list = saved_refresh_stencil_list;
    video::g_screen_res_x = saved_screen_res_x;
    video::g_screen_res_y = saved_screen_res_y;
    video::screen_refresh_flag = saved_screen_refresh_flag;
}

}
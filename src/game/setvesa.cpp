#include "pch.h"

namespace setvesa {
    void __cdecl Add_Square_To_Scan_List_(int32_t left, int32_t top, int32_t right, int32_t bottom) {
        if (video::g_screen_res_x <= left || video::g_screen_res_y <= top || right <= 0 || bottom <= 0) {
            return;
        }

        if (left < 0) {
            left = 0;
        }

        if (top < 0) {
            top = 0;
        }

        if (video::g_screen_res_x <= right) {
            right = video::g_screen_res_x - 1;
        }

        if (video::g_screen_res_y <= bottom) {
            bottom = video::g_screen_res_y - 1;
        }

        left >>= 2;
        right >>= 2;

        if (right < left) {
            exitm::Exit_With_Message("Add Square to scan list failed..");
        }

        const int16_t min_scan_x = (int16_t)left;
        const int16_t max_scan_x = (int16_t)right;

        if (video::screen_refresh_flag == 2) {
            for (int32_t row = top; row <= bottom; ++row) {
                if (video::_refresh_scan_list[row].min_x == -1) {
                    video::_refresh_scan_list[row].min_x = min_scan_x;
                    video::_refresh_scan_list[row].max_x = max_scan_x;
                } else {
                    if (min_scan_x < video::_refresh_scan_list[row].min_x) {
                        video::_refresh_scan_list[row].min_x = min_scan_x;
                    }
                    if (video::_refresh_scan_list[row].max_x < max_scan_x) {
                        video::_refresh_scan_list[row].max_x = max_scan_x;
                    }
                }
            }
        } else {
            for (int32_t row = top; row <= bottom; ++row) {
                if (video::_refresh_scan_list[row].min_x == -1) {
                    video::_refresh_scan_list[row].min_x = min_scan_x;
                    video::_refresh_scan_list[row].max_x = max_scan_x;
                } else {
                    if (min_scan_x < video::_refresh_scan_list[row].min_x) {
                        video::_refresh_scan_list[row].min_x = min_scan_x;
                    }
                    if (video::_refresh_scan_list[row].max_x < max_scan_x) {
                        video::_refresh_scan_list[row].max_x = max_scan_x;
                    }
                }

                if (video::_refresh_stencil_list[row].min_x == -1) {
                    video::_refresh_stencil_list[row].min_x = min_scan_x;
                    video::_refresh_stencil_list[row].max_x = max_scan_x;
                } else {
                    if (min_scan_x < video::_refresh_stencil_list[row].min_x) {
                        video::_refresh_stencil_list[row].min_x = min_scan_x;
                    }
                    if (video::_refresh_stencil_list[row].max_x < max_scan_x) {
                        video::_refresh_stencil_list[row].max_x = max_scan_x;
                    }
                }
            }
        }
    }
}
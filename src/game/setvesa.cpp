#include "pch.h"

namespace setvesa {
    void __cdecl Add_Square_To_Scan_List_(int x1, int y1, int x2, int y2) {
        if (g_screen_res_x <= x1 || g_screen_res_y <= y1 || x2 <= 0 || y2 <= 0) {
            return;
        }

        if (x1 < 0) {
            x1 = 0;
        }

        if (y1 < 0) {
            y1 = 0;
        }

        if (g_screen_res_x <= x2) {
            x2 = g_screen_res_x - 1;
        }

        if (g_screen_res_y <= y2) {
            y2 = g_screen_res_y - 1;
        }

        x1 >>= 2;
        x2 >>= 2;

        if (x2 < x1) {
            exitm::Exit_With_Message(s_Add_Square_to_scan_list_failed__0055b270);
        }

        if (video::screen_refresh_flag == 2) {
            for (int16_t y = (int16_t)y1; y <= y2; ++y) {
                if (video::_refresh_scan_list[y].min_x == -1) {
                    video::_refresh_scan_list[y].min_x = (int16_t)x1;
                    video::_refresh_scan_list[y].max_x = (int16_t)x2;
                } else {
                    if (x1 < video::_refresh_scan_list[y].min_x) {
                        video::_refresh_scan_list[y].min_x = (int16_t)x1;
                    }
                    if (video::_refresh_scan_list[y].max_x < x2) {
                        video::_refresh_scan_list[y].max_x = (int16_t)x2;
                    }
                }
            }
        } else {
            for (int16_t y = (int16_t)y1; y <= y2; ++y) {
                if (video::_refresh_scan_list[y].min_x == -1) {
                    video::_refresh_scan_list[y].min_x = (int16_t)x1;
                    video::_refresh_scan_list[y].max_x = (int16_t)x2;
                } else {
                    if (x1 < video::_refresh_scan_list[y].min_x) {
                        video::_refresh_scan_list[y].min_x = (int16_t)x1;
                    }
                    if (video::_refresh_scan_list[y].max_x < x2) {
                        video::_refresh_scan_list[y].max_x = (int16_t)x2;
                    }
                }

                if (video::_refresh_stencil_list[y].min_x == -1) {
                    video::_refresh_stencil_list[y].min_x = (int16_t)x1;
                    video::_refresh_stencil_list[y].max_x = (int16_t)x2;
                } else {
                    if (x1 < video::_refresh_stencil_list[y].min_x) {
                        video::_refresh_stencil_list[y].min_x = (int16_t)x1;
                    }
                    if (video::_refresh_stencil_list[y].max_x < x2) {
                        video::_refresh_stencil_list[y].max_x = (int16_t)x2;
                    }
                }
            }
        }
    }
}
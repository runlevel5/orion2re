#include "deps.h"
#include "graphics.h"

namespace graphics {

    void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        if (x2 < x1) {
            strings::Swap_Short_(&x1, &x2);
        }
        if (y2 < y1) {
            strings::Swap_Short_(&y1, &y2);
        }

        if (animate::_clipping_flag == 1) {
            if (x1 >= video::_screen_window_x2 || x2 <= video::_screen_window_x1 ||
                y1 >= video::_screen_window_y2 || y2 <= video::_screen_window_y1) {
                platform::Unlock_Appropriate_Render_Buffer();
                return;
            }

            if (x1 < video::_screen_window_x1) x1 = video::_screen_window_x1;
            if (y1 < video::_screen_window_y1) y1 = video::_screen_window_y1;
            if (x2 > video::_screen_window_x2) x2 = video::_screen_window_x2;
            if (y2 > video::_screen_window_y2) y2 = video::_screen_window_y2;
        }

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(x1, y1, x2, y2);
        }

        int left_block = x1 / 4;
        int right_block = x2 / 4;

        if (right_block == left_block) {
            uint8_t* surface = reinterpret_cast<uint8_t*>(video::_current_video_page + (y1 * platform::g_screen_res_x) + x1);
            regions::Fill_Column_(color, surface, (x2 - x1) + 1, (y2 - y1) + 1);
        } else {
            uint32_t left_partial_width = 4 - (x1 & 3);
            uint32_t right_partial_width = (x2 & 3) + 1;
            int mid_blocks = (right_block - left_block) - 1;

            if (left_partial_width == 4) {
                left_partial_width = 0;
                mid_blocks++;
            }
            if (right_partial_width == 4) {
                right_partial_width = 0;
                mid_blocks++;
            }

            int y_offset = y1 * platform::g_screen_res_x;
            uint32_t height = (y2 - y1) + 1;

            if (left_partial_width == 0) {
                if (mid_blocks != 0) {
                    uint8_t* surface = reinterpret_cast<uint8_t*>(video::_current_video_page + (left_block * 4) + y_offset);
                    regions::Fill_Region_(color, surface, static_cast<uint8_t>(mid_blocks), height);
                }
            } else {
                uint8_t* col_surface = reinterpret_cast<uint8_t*>(video::_current_video_page + y_offset + x1);
                regions::Fill_Column_(color, col_surface, left_partial_width, height);

                uint8_t* reg_surface = reinterpret_cast<uint8_t*>(video::_current_video_page + (left_block * 4) + y_offset + 4);
                regions::Fill_Region_(color, reg_surface, static_cast<uint8_t>(mid_blocks), height);
            }

            if (right_partial_width != 0) {
                uint8_t* surface = reinterpret_cast<uint8_t*>(video::_current_video_page + (right_block * 4) + y_offset);
                regions::Fill_Column_(color, surface, right_partial_width, height);
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void Reset_Window_()
    {
        video::_screen_window_x1 = 0;
        video::_screen_window_y1 = 0;
        video::_screen_window_x2 = (short)platform::g_screen_res_x + -1;
        video::_screen_window_y2 = (short)platform::g_screen_res_y + -1;
        return;
    }

    void __cdecl Set_Window_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        if (x1 < 0) {
            x1 = 0;
        }

        if (y1 < 0) {
            y1 = 0;
        }

        if (static_cast<int32_t>(x2) >= platform::g_screen_res_x) {
            x2 = static_cast<int16_t>(platform::g_screen_res_x - 1);
        }

        if (static_cast<int32_t>(y2) >= platform::g_screen_res_y) {
            y2 = static_cast<int16_t>(platform::g_screen_res_y - 1);
        }

        video::_screen_window_x1 = x1;
        video::_screen_window_y1 = y1;
        video::_screen_window_x2 = x2;
        video::_screen_window_y2 = y2;
    }

    void __cdecl Save_Window_() {
        _old_screen_window_x1 = video::_screen_window_x1;
        _old_screen_window_y1 = video::_screen_window_y1;
        _old_screen_window_x2 = video::_screen_window_x2;
        _old_screen_window_y2 = video::_screen_window_y2;
    }

    void __cdecl Restore_Window_() {
        video::_screen_window_x1 = _old_screen_window_x1;
        video::_screen_window_y1 = _old_screen_window_y1;
        video::_screen_window_x2 = _old_screen_window_x2;
        video::_screen_window_y2 = _old_screen_window_y2;
    }

    void __cdecl Darken_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        Tint_Fill_(x1, y1, x2, y2, 0);
    }
}
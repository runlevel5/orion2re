#include "pch.h"

namespace line {

    /**
     * @brief Clips a line segment to the screen boundaries defined in video namespace.
     *
     * @param x1 Pointer to start X coordinate
     * @param y1 Pointer to start Y coordinate
     * @param x2 Pointer to end X coordinate
     * @param y2 Pointer to end Y coordinate
     * @return 0 if the line (or part of it) is visible, 1 if the line is completely clipped.
     */
    int16_t __cdecl Clip_Line_(int16_t* x1, int16_t* y1, int16_t* x2, int16_t* y2) {
        int16_t is_clipped = 0;

        if (*x2 == *x1) {
            // Vertical line optimization
            if (*x1 < video::_screen_window_x1 || *x1 > video::_screen_window_x2) {
                is_clipped = 1;
            } else {
                if (*y2 < *y1) {
                    int16_t temp = *y1;
                    *y1 = *y2;
                    *y2 = temp;
                }
                if (*y2 < video::_screen_window_y1 || *y1 > video::_screen_window_y2) {
                    is_clipped = 1;
                } else {
                    if (*y1 < video::_screen_window_y1) *y1 = video::_screen_window_y1;
                    if (*y2 > video::_screen_window_y2) *y2 = video::_screen_window_y2;
                }
            }
        } else {
            // X-clipping logic
            if (*x2 < *x1) {
                int16_t temp_x = *x1; *x1 = *x2; *x2 = temp_x;
                int16_t temp_y = *y1; *y1 = *y2; *y2 = temp_y;
            }

            int32_t dx = (int32_t)*x2 - (int32_t)*x1;
            int32_t dy = (int32_t)*y2 - (int32_t)*y1;

            if (*x1 < video::_screen_window_x1) {
                *y1 = *y1 + (int16_t)(((int32_t)video::_screen_window_x1 - (int32_t)*x1) * dy / dx);
                *x1 = video::_screen_window_x1;
            }

            if (*x1 > *x2 || *x1 > video::_screen_window_x2) {
                is_clipped = 1;
            } else {
                if (*x2 > video::_screen_window_x2) {
                    *y2 = *y1 + (int16_t)(((int32_t)video::_screen_window_x2 - (int32_t)*x1) * dy / dx);
                    *x2 = video::_screen_window_x2;
                }
                if (*x1 > *x2) is_clipped = 1;
            }
        }

        if (is_clipped) goto result_clipped;

        // Y-clipping logic
        if (*y1 == *y2) {
            // Horizontal line check (after X clipping)
            if (*y1 < video::_screen_window_y1 || *y1 > video::_screen_window_y2) {
                goto result_clipped;
            }
            if (*x2 < *x1) {
                int16_t temp = *x1; *x1 = *x2; *x2 = temp;
            }
            if (*x2 < video::_screen_window_x1 || *x1 > video::_screen_window_x2) {
                goto result_clipped;
            }
            if (*x1 < video::_screen_window_x1) *x1 = video::_screen_window_x1;
            if (*x2 > video::_screen_window_x2) *x2 = video::_screen_window_x2;
            return is_clipped;
        } else {
            if (*y2 < *y1) {
                int16_t temp_y = *y1; *y1 = *y2; *y2 = temp_y;
                int16_t temp_x = *x1; *x1 = *x2; *x2 = temp_x;
            }

            int32_t dx = (int32_t)*x2 - (int32_t)*x1;
            int32_t dy = (int32_t)*y2 - (int32_t)*y1;

            if (*y1 < video::_screen_window_y1) {
                *x1 = *x1 + (int16_t)(((int32_t)video::_screen_window_y1 - (int32_t)*y1) * dx / dy);
                *y1 = video::_screen_window_y1;
            }

            if (*y1 <= *y2) {
                if (*y2 > video::_screen_window_y2) {
                    *x2 = *x1 + (int16_t)(((int32_t)video::_screen_window_y2 - (int32_t)*y1) * dx / dy);
                    *y2 = video::_screen_window_y2;
                }
                if (*y1 <= *y2) return is_clipped;
            }
        }

    result_clipped:
        return 1;
    }

    void __cdecl Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color) {
        if (animate::_clipping_flag == 1) {
            if (Clip_Line_(&x1, &y1, &x2, &y2) == 1) {
                return;
            }
        }

        if (y2 < y1) {
            strings::Swap_Short_(&y1, &y2);
            strings::Swap_Short_(&x1, &x2);
        }

        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
        uint8_t* render_ptr = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(y1 * platform::g_screen_res_x + x1 + (uintptr_t)video::_current_video_page));

        int16_t dx = x2 - x1;
        int16_t x_step;
        if (dx < 0) {
            dx = -dx;
            x_step = -1;
        } else {
            x_step = 1;
        }

        int16_t dy = y2 - y1;

        // Vertical line case
        if (dx == 0) {
            int16_t count = dy + 1;
            do {
                *render_ptr = color;
                render_ptr += platform::g_screen_res_x;
                count--;
            } while (count != 0);

            if (video::screen_refresh_flag >= 2) {
                for (int16_t y = y1; y <= y2; ++y) {
                    video::Add_Dot_To_Scan_List_(x1, y);
                }
            }
            platform::Unlock_Appropriate_Render_Buffer();
            return;
        }

        // Horizontal line case
        if (dy == 0) {
            int16_t count = dx + 1;
            do {
                *render_ptr = color;
                render_ptr += x_step;
                count--;
            } while (count != 0);

            if (video::screen_refresh_flag >= 2) {
                int16_t rx1 = x1, rx2 = x2;
                if (rx2 < rx1) strings::Swap_Short_(&rx1, &rx2);
                setvesa::Add_Square_To_Scan_List_(rx1, y1, rx2, y1);
            }
            platform::Unlock_Appropriate_Render_Buffer();
            return;
        }

        // Diagonal line case (slope 1 or -1)
        if (dy == dx) {
            int16_t count = dx + 1;
            do {
                *render_ptr = color;
                render_ptr += (platform::g_screen_res_x + x_step);
                count--;
            } while (count != 0);

            if (video::screen_refresh_flag >= 2) {
                if (x1 < x2) {
                    for (int16_t y = y1; y <= y2; ++y) {
                        video::Add_Dot_To_Scan_List_(x1 - y1 + y, y);
                    }
                } else {
                    for (int16_t y = y1; y <= y2; ++y) {
                        video::Add_Dot_To_Scan_List_(y1 - y + x1, y);
                    }
                }
            }
            platform::Unlock_Appropriate_Render_Buffer();
            return;
        }

        // Bresenham-based line drawing
        if (dy < dx) {
            uint16_t run_len = dx / dy;
            int16_t rem = (dx % dy) * 2;
            int16_t error = (dx % dy) - (dy * 2);
            uint16_t initial_run = (run_len / 2) + 1;
            uint16_t final_run = initial_run;

            if (rem == 0) {
                if ((run_len & 1) == 0) initial_run--;
            }
            if ((run_len & 1) != 0) {
                error += dy;
            }

            Draw_Horizontal_Run_(&render_ptr, x_step, initial_run, color);
            if (video::screen_refresh_flag >= 2) {
                if (x_step > 0) {
                    setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + initial_run, y1);
                    x1 += initial_run;
                } else {
                    setvesa::Add_Square_To_Scan_List_(x1 - initial_run, y1, x1, y1);
                    x1 -= initial_run;
                }
                y1++;
            }

            for (int i = 0; i < dy - 1; ++i) {
                error += rem;
                uint16_t current_run = run_len;
                if (error > 0) {
                    current_run++;
                    error -= (dy * 2);
                }
                Draw_Horizontal_Run_(&render_ptr, x_step, current_run, color);
                if (video::screen_refresh_flag >= 2) {
                    if (x_step > 0) {
                        setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + current_run, y1);
                        x1 += current_run;
                    } else {
                        setvesa::Add_Square_To_Scan_List_(x1 - current_run, y1, x1, y1);
                        x1 -= current_run;
                    }
                    y1++;
                }
            }

            Draw_Horizontal_Run_(&render_ptr, x_step, final_run, color);
            if (video::screen_refresh_flag >= 2) {
                if (x_step > 0) {
                    setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + final_run, y1);
                } else {
                    setvesa::Add_Square_To_Scan_List_(x1 - final_run, y1, x1, y1);
                }
            }
        } else {
            uint16_t run_len = dy / dx;
            int16_t rem = (dy % dx) * 2;
            int16_t error = (dy % dx) - (dx * 2);
            uint16_t initial_run = (run_len / 2) + 1;
            uint16_t final_run = initial_run;

            if (rem == 0) {
                if ((run_len & 1) == 0) initial_run--;
            }
            if ((run_len & 1) != 0) {
                error += dx;
            }

            Draw_Vertical_Run_(&render_ptr, x_step, initial_run, color);
            if (video::screen_refresh_flag >= 2) {
                for (int16_t i = 0; i < initial_run; ++i) {
                    video::Add_Dot_To_Scan_List_(x1, y1 + i);
                }
                y1 += initial_run;
                x1 += x_step;
            }

            for (int i = 0; i < dx - 1; ++i) {
                error += rem;
                uint16_t current_run = run_len;
                if (error > 0) {
                    current_run++;
                    error -= (dx * 2);
                }
                Draw_Vertical_Run_(&render_ptr, x_step, current_run, color);
                if (video::screen_refresh_flag >= 2) {
                    for (int16_t j = 0; j < (int16_t)current_run; ++j) {
                        video::Add_Dot_To_Scan_List_(x1, y1 + j);
                    }
                    y1 += current_run;
                    x1 += x_step;
                }
            }

            Draw_Vertical_Run_(&render_ptr, x_step, final_run, color);
            if (video::screen_refresh_flag >= 2) {
                for (int16_t i = 0; i < (int16_t)final_run; ++i) {
                    video::Add_Dot_To_Scan_List_(x1, y1 + i);
                }
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Draw_Vertical_Run_(uint8_t** buf, int16_t param_2, int16_t count, uint8_t color) {
        uint8_t* current_ptr = *buf;

        if (count > 0) {
            int32_t remaining = static_cast<int32_t>(count);
            do {
                *current_ptr = color;
                current_ptr += platform::g_screen_res_x;
                remaining--;
            } while (remaining != 0);
        }

        // Update the caller's buffer pointer, applying the post-draw offset
        *buf = current_ptr + param_2;
    }

    void __cdecl Draw_Horizontal_Run_(uint8_t** buf, int16_t step_x, int16_t count, uint8_t color) {
        uint8_t* current_pos = *buf;
        if (count > 0) {
            int32_t remaining = static_cast<int32_t>(count);
            do {
                *current_pos = color;
                current_pos += step_x;
                remaining--;
            } while (remaining != 0);
        }

        // Advance buffer pointer to the next line based on screen resolution
        *buf = current_pos + platform::g_screen_res_x;
    }

} // namespace line
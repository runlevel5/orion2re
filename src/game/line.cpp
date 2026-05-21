#include "pch.h"

namespace line {

    void __cdecl Multi_Colored_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors, int16_t offset) {
        if (animate::_clipping_flag == 1) {
            if (line::Clip_Line_(&x1, &y1, &x2, &y2) == 1) {
                return;
            }
        }

        if (y2 < y1) {
            strings::Swap_Short_(&y1, &y2);
            strings::Swap_Short_(&x1, &x2);
        }

        uint8_t* local_ptr = video::_current_video_page + y1 * g_screen_res_x + x1;
        int16_t dx = x2 - x1;
        int16_t abs_dx = dx;
        int16_t x_dir = 1;

        if (dx < 0) {
            abs_dx = -dx;
            x_dir = -1;
        }

        int16_t dy = y2 - y1;

        if (abs_dx == 0) {
            int16_t count = dy + 1;
            while (count > 0) {
                *local_ptr = colors->colors[offset & 7];
                offset++;
                local_ptr += g_screen_res_x;
                count--;
            }
            if (video::screen_refresh_flag >= 2 && y1 <= y2) {
                for (int16_t cur_y = y1; cur_y <= y2; cur_y++) {
                    video::Add_Dot_To_Scan_List_(x1, cur_y);
                }
            }
            return;
        }

        if (dy == 0) {
            int16_t count = abs_dx + 1;
            while (count > 0) {
                *local_ptr = colors->colors[offset & 7];
                offset++;
                local_ptr += x_dir;
                count--;
            }
            if (video::screen_refresh_flag >= 2) {
                if (x2 < x1) {
                    strings::Swap_Short_(&x1, &x2);
                }
                setvesa::Add_Square_To_Scan_List_(x1, y1, x2, y1);
            }
            return;
        }

        if (abs_dx == dy) {
            int16_t count = abs_dx + 1;
            while (count > 0) {
                *local_ptr = colors->colors[offset & 7];
                offset++;
                local_ptr += g_screen_res_x + x_dir;
                count--;
            }
            if (video::screen_refresh_flag >= 2 && y1 <= y2) {
                if (x2 <= x1) {
                    for (int16_t cur_y = y1; cur_y <= y2; cur_y++) {
                        video::Add_Dot_To_Scan_List_(x1 - (cur_y - y1), cur_y);
                    }
                } else {
                    for (int16_t cur_y = y1; cur_y <= y2; cur_y++) {
                        video::Add_Dot_To_Scan_List_(x1 + (cur_y - y1), cur_y);
                    }
                }
            }
            return;
        }

        if (abs_dx > dy) {
            int16_t run_base = abs_dx / dy;
            int16_t rem2 = (abs_dx % dy) * 2;
            int16_t err = (abs_dx % dy) - 2 * dy;
            int16_t first_run = (run_base / 2) + 1;
            int16_t first_run_adj = first_run;

            if (rem2 == 0 && (run_base & 1) == 0) {
                first_run_adj--;
            }
            if ((run_base & 1) != 0) {
                err += dy;
            }

            line::Draw_Horizontal_Run_Multi_(&local_ptr, x_dir, first_run_adj, colors, &offset);

            if (video::screen_refresh_flag >= 2) {
                if (x_dir > 0) {
                    setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + first_run_adj, y1);
                    x1 += first_run_adj;
                } else {
                    setvesa::Add_Square_To_Scan_List_(x1 - first_run_adj, y1, x1, y1);
                    x1 -= first_run_adj;
                }
                y1++;
            }

            int count = dy - 1;
            for (int i = 0; i < count; i++) {
                int16_t run = run_base;
                err += rem2;
                if (err > 0) {
                    run++;
                    err -= 2 * dy;
                }
                line::Draw_Horizontal_Run_Multi_(&local_ptr, x_dir, run, colors, &offset);

                if (video::screen_refresh_flag >= 2) {
                    if (x_dir > 0) {
                        setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + run, y1);
                        x1 += run;
                    } else {
                        setvesa::Add_Square_To_Scan_List_(x1 - run, y1, x1, y1);
                        x1 -= run;
                    }
                    y1++;
                }
            }

            line::Draw_Horizontal_Run_Multi_(&local_ptr, x_dir, first_run, colors, &offset);

            if (video::screen_refresh_flag >= 2) {
                if (x_dir > 0) {
                    setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + first_run, y1);
                } else {
                    setvesa::Add_Square_To_Scan_List_(x1 - first_run, y1, x1, y1);
                }
            }
        } else {
            int16_t run_base = dy / abs_dx;
            int16_t rem2 = (dy % abs_dx) * 2;
            int16_t err = (dy % abs_dx) - 2 * abs_dx;
            int16_t first_run = (run_base / 2) + 1;
            int16_t first_run_adj = first_run;

            if (rem2 == 0 && (run_base & 1) == 0) {
                first_run_adj--;
            }
            if ((run_base & 1) != 0) {
                err += abs_dx;
            }

            line::Draw_Vertical_Run_Multi_(&local_ptr, x_dir, first_run_adj, colors, &offset);

            if (video::screen_refresh_flag >= 2) {
                for (int16_t i = 0; i < first_run_adj; i++) {
                    video::Add_Dot_To_Scan_List_(x1, y1 + i);
                }
                y1 += first_run_adj;
                x1 += x_dir;
            }

            int count = abs_dx - 1;
            for (int i = 0; i < count; i++) {
                int16_t run = run_base;
                err += rem2;
                if (err > 0) {
                    run++;
                    err -= 2 * abs_dx;
                }
                line::Draw_Vertical_Run_Multi_(&local_ptr, x_dir, run, colors, &offset);

                if (video::screen_refresh_flag >= 2) {
                    for (int16_t j = 0; j < run; j++) {
                        video::Add_Dot_To_Scan_List_(x1, y1 + j);
                    }
                    y1 += run;
                    x1 += x_dir;
                }
            }

            line::Draw_Vertical_Run_Multi_(&local_ptr, x_dir, first_run, colors, &offset);

            if (video::screen_refresh_flag >= 2) {
                for (int16_t i = 0; i < first_run; i++) {
                    video::Add_Dot_To_Scan_List_(x1, y1 + i);
                }
            }
        }
    }

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

    void __cdecl Plasma_Box_(int16_t x, int16_t y, uint16_t w, int16_t h, int16_t thickness,
                             int16_t val1, uint8_t val2, int32_t val3) {
        Create_Plasma_Line_(val1, val2, reinterpret_cast<const uint8_t*>(val3));

        if (thickness > 0) {
            for (int16_t i = 0; i < thickness; ++i) {
                int16_t y1 = y + i;
                int16_t x2 = x - i + w - 1;
                int16_t x1 = x + i;

                Plasma_Line_(x1, y1, x2, y1, replace::_graphics_buffer, 0);

                int16_t y2 = y - i + h - 1;
                Plasma_Line_(x2, y1, x2, y2, replace::_graphics_buffer, w);

                for (int j = 0; j < 511; ++j) {
                    uint8_t tmp = replace::_graphics_buffer[j];
                    replace::_graphics_buffer[j] = replace::_graphics_buffer[1022 - j];
                    replace::_graphics_buffer[1022 - j] = tmp;
                }

                Plasma_Line_(x1, y1, x1, y2, replace::_graphics_buffer, h + w);
                Plasma_Line_(x1, y2, x2, y2, replace::_graphics_buffer, h + w * 2);
            }
        }
    }

    void __cdecl Draw_Horizontal_Run_Multi_(uint8_t** dest_ptr, int16_t step, int16_t length, s_colors* colors, int16_t* color_index) {
        uint8_t* ptr = *dest_ptr;
        if (length > 0) {
            int32_t count = length;
            do {
                *ptr = colors->colors[*color_index & 7];
                ptr += step;
                (*color_index)++;
                count--;
            } while (count != 0);
        }
        *dest_ptr = ptr + g_screen_res_x;
    }

    void __cdecl Draw_Vertical_Run_Multi_(uint8_t** screen_ptr, int16_t offset, int16_t height, s_colors* colors, int16_t* color_idx) {
        uint8_t* dest = *screen_ptr;

        for (int16_t i = 0; i < height; ++i) {
            *dest = colors->colors[*color_idx & 7];
            (*color_idx)++;
            dest += g_screen_res_x;
        }

        *screen_ptr = dest + offset;
    }

    void __cdecl Plasma_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t* color_map, uint16_t color_index) {
        if (animate::_clipping_flag == 1) {
            if (line::Clip_Line_(&x1, &y1, &x2, &y2) == 1) {
                return;
            }
        }

        if (y1 > y2) {
            strings::Swap_Short_(&y1, &y2);
            strings::Swap_Short_(&x1, &x2);
        }

        uint8_t* ptr = video::_current_video_page + y1 * g_screen_res_x + x1;
        int16_t dx = x2 - x1;
        int16_t x_dir;

        if (dx >= 0) {
            x_dir = 1;
        } else {
            dx = -dx;
            x_dir = -1;
        }

        int16_t dy = y2 - y1;

        if (dx == 0) {
            if (dy >= 0) {
                int16_t count = dy + 1;
                do {
                    *ptr = color_map[color_index & 0x3ff];
                    color_index++;
                    ptr += g_screen_res_x;
                    count--;
                } while (count != 0);
            }

            if (video::screen_refresh_flag > 1) {
                if (y1 <= y2) {
                    int16_t y = y1;
                    do {
                        video::Add_Dot_To_Scan_List_(x1, y);
                        y++;
                    } while (y <= y2);
                }
            }
            return;
        }

        if (dy == 0) {
            if (dx >= 0) {
                int16_t count = dx + 1;
                do {
                    *ptr = color_map[color_index & 0x3ff];
                    color_index++;
                    ptr += x_dir;
                    count--;
                } while (count != 0);
            }

            if (video::screen_refresh_flag > 1) {
                if (x1 > x2) {
                    strings::Swap_Short_(&x2, &x1);
                }
                setvesa::Add_Square_To_Scan_List_(x1, y1, x2, y1);
            }
            return;
        }

        if (dx == dy) {
            if (dx >= 0) {
                int16_t count = dx + 1;
                do {
                    *ptr = color_map[color_index & 0x3ff];
                    color_index++;
                    ptr += g_screen_res_x + x_dir;
                    count--;
                } while (count != 0);
            }

            if (video::screen_refresh_flag > 1) {
                if (x1 >= x2) {
                    if (y1 <= y2) {
                        int16_t y = y1;
                        do {
                            video::Add_Dot_To_Scan_List_(x1 - (y - y1), y);
                            y++;
                        } while (y <= y2);
                    }
                } else {
                    if (y1 <= y2) {
                        int16_t y = y1;
                        do {
                            video::Add_Dot_To_Scan_List_(x1 + (y - y1), y);
                            y++;
                        } while (y <= y2);
                    }
                }
            }
            return;
        }

        if (dx > dy) {
            int16_t run_len = dx / dy;
            int16_t err_inc = (dx % dy) * 2;
            int16_t err = (dx % dy) - dy * 2;
            int16_t run_len_half = (run_len / 2) + 1;
            int16_t first_run = run_len_half;

            if (err_inc == 0) {
                if ((run_len & 1) == 0) {
                    first_run--;
                } else {
                    err += dy;
                }
            } else {
                if ((run_len & 1) != 0) {
                    err += dy;
                }
            }

            line::Draw_Horizontal_Run_Plasma_(&ptr, x_dir, first_run, color_map, &color_index);

            if (video::screen_refresh_flag > 1) {
                if (x_dir <= 0) {
                    setvesa::Add_Square_To_Scan_List_(x1 - first_run, y1, x1, y1);
                    x1 -= first_run;
                } else {
                    setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + first_run, y1);
                    x1 += first_run;
                }
                y1++;
            }

            int32_t num_runs = dy - 1;
            if (num_runs > 0) {
                for (int32_t i = 0; i < num_runs; i++) {
                    err += err_inc;
                    int16_t run = run_len;
                    if (err > 0) {
                        run++;
                        err -= dy * 2;
                    }

                    line::Draw_Horizontal_Run_Plasma_(&ptr, x_dir, run, color_map, &color_index);

                    if (video::screen_refresh_flag > 1) {
                        if (x_dir <= 0) {
                            setvesa::Add_Square_To_Scan_List_(x1 - run, y1, x1, y1);
                            x1 -= run;
                        } else {
                            setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + run, y1);
                            x1 += run;
                        }
                        y1++;
                    }
                }
            }

            line::Draw_Horizontal_Run_Plasma_(&ptr, x_dir, run_len_half, color_map, &color_index);

            if (video::screen_refresh_flag > 1) {
                if (x_dir <= 0) {
                    setvesa::Add_Square_To_Scan_List_(x1 - run_len_half, y1, x1, y1);
                } else {
                    setvesa::Add_Square_To_Scan_List_(x1, y1, x1 + run_len_half, y1);
                }
            }
        } else {
            int16_t run_len = dy / dx;
            int16_t err_inc = (dy % dx) * 2;
            int16_t err = (dy % dx) - dx * 2;
            int16_t run_len_half = (run_len / 2) + 1;
            int16_t first_run = run_len_half;

            if (err_inc == 0) {
                if ((run_len & 1) == 0) {
                    first_run--;
                } else {
                    err += dx;
                }
            } else {
                if ((run_len & 1) != 0) {
                    err += dx;
                }
            }

            line::Draw_Vertical_Run_Plasma_(&ptr, x_dir, first_run, color_map, &color_index);

            if (video::screen_refresh_flag > 1) {
                int16_t i = 0;
                if (first_run > 0) {
                    do {
                        video::Add_Dot_To_Scan_List_(x1, y1 + i);
                        i++;
                    } while (i < first_run);
                }
                y1 += first_run;
                x1 += x_dir;
            }

            int32_t num_runs = dx - 1;
            if (num_runs > 0) {
                for (int32_t i = 0; i < num_runs; i++) {
                    err += err_inc;
                    int16_t run = run_len;
                    if (err > 0) {
                        run++;
                        err -= dx * 2;
                    }

                    line::Draw_Vertical_Run_Plasma_(&ptr, x_dir, run, color_map, &color_index);

                    if (video::screen_refresh_flag > 1) {
                        int16_t j = 0;
                        if (run > 0) {
                            do {
                                video::Add_Dot_To_Scan_List_(x1, y1 + j);
                                j++;
                            } while (j < run);
                        }
                        y1 += run;
                        x1 += x_dir;
                    }
                }
            }

            line::Draw_Vertical_Run_Plasma_(&ptr, x_dir, run_len_half, color_map, &color_index);

            if (video::screen_refresh_flag > 1) {
                int16_t i = 0;
                if (run_len_half > 0) {
                    do {
                        video::Add_Dot_To_Scan_List_(x1, y1 + i);
                        i++;
                    } while (i < run_len_half);
                }
            }
        }
    }

    /**
     * @brief Draws a vertical line segment using cyclic plasma data and updates the pixel pointer.
     * 
     * @param pixel_ptr_ref Pointer to the current pixel location, updated after the run.
     * @param post_offset Offset added to the pixel pointer after the drawing loop.
     * @param length Number of pixels to draw vertically.
     * @param plasma_table 1024-byte table containing color/plasma values.
     * @param plasma_index_ptr Pointer to the current index in the plasma table (cyclic).
     */
    void __cdecl Draw_Vertical_Run_Plasma_(uint8_t** pixel_ptr_ref, int16_t post_offset, int16_t length, uint8_t* plasma_table, uint16_t* plasma_index_ptr) {
        uint8_t* current_pixels = *pixel_ptr_ref;

        if (length > 0) {
            int32_t count = (int32_t)length;
            do {
                // Look up color from plasma table using a 10-bit masked index
                uint16_t index = *plasma_index_ptr;
                *current_pixels = plasma_table[index & 0x3FF /* 1023 */];

                // Increment cyclic index and move to the next screen row
                *plasma_index_ptr = index + 1;
                current_pixels += g_screen_res_x;

                count--;
            } while (count != 0);
        }

        // Apply final offset (typically to move to the next column or segment) and update output pointer
        *pixel_ptr_ref = current_pixels + post_offset;
    }

    void __cdecl Draw_Horizontal_Run_Plasma_(uint8_t** dest_ptr, int16_t step, int16_t length, uint8_t* plasma_table, uint16_t* angle_ptr) {
        uint8_t* dest = *dest_ptr;

        if (length > 0) {
            int32_t count = (int32_t)length;
            do {
                *dest = plasma_table[*angle_ptr & 0x3FF];
                dest += step;
                (*angle_ptr)++;
                count--;
            } while (count != 0);
        }

        *dest_ptr = dest + g_screen_res_x;
    }

    void __cdecl Create_Plasma_Line_(int16_t seed, uint16_t start_val, const uint8_t* color_map) {
        random::Save_Random_Seed_();
        random::Set_Random_Seed_(seed);

        replace::_graphics_buffer[0] = static_cast<uint8_t>(start_val & 0x7);

        int8_t direction_state = static_cast<int8_t>(random::Random_(4)) - 1;
        int8_t step_dir = (direction_state == 0) ? -1 : 1;

        for (int i = 1; i < 1023; ++i) {
            int8_t val = replace::_graphics_buffer[i - 1];

            if (random::Random_(4) == 1) {
                direction_state = 1 - direction_state;
                step_dir = (direction_state == 0) ? -1 : 1;
            }

            if (random::Random_(4) == 1) {
                val += step_dir;
                if (random::Random_(4) == 1) {
                    val += step_dir;
                    if (random::Random_(4) == 1) {
                        val += step_dir;
                    }
                }
            }

            if (val < 0) {
                val = 0;
            } else if (val > 7) {
                val = 7;
            }

            replace::_graphics_buffer[i] = static_cast<uint8_t>(val);
        }

        for (int i = 1; i < 1023; ++i) {
            replace::_graphics_buffer[i] = color_map[replace::_graphics_buffer[i]];
        }

        random::Restore_Random_Seed_();
    }

} // namespace line
#include "pch.h"

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

    void __cdecl Box_(int16_t x, int16_t y, int16_t width, int16_t height, int16_t thickness, uint8_t color) {
        if (thickness <= 0) {
            return;
        }

        for (int i = 0; i < (int)thickness; ++i) {
            int16_t offset = (int16_t)i;
            int16_t x1 = x + offset;
            int16_t y1 = y + offset;
            int16_t x2 = x + width - offset - 1;
            int16_t y2 = y + height - offset - 1;

            // Draw the four sides of the rectangle for the current thickness layer
            line::Line_(x1, y1, x2, y1, color); // Top edge
            line::Line_(x1, y1, x1, y2, color); // Left edge
            line::Line_(x2, y1, x2, y2, color); // Right edge
            line::Line_(x1, y2, x2, y2, color); // Bottom edge
        }
    }

    void __cdecl Dot_(int16_t x, int16_t y, uint8_t color) {
        if (animate::_clipping_flag == 1) {
            if (x < video::_screen_window_x1 || y < video::_screen_window_y1 ||
                x > video::_screen_window_x2 || y > video::_screen_window_y2) {
                return;
            }
        }

        platform::g_screen_res_x = (int32_t)platform::Lock_Appropriate_Render_Buffer();

        if (video::screen_refresh_flag >= 2) {
            video::Add_Dot_To_Scan_List_(static_cast<int32_t>(x), static_cast<int32_t>(y));
        }

        uint8_t* framebuffer = video::_current_video_page;
        int32_t offset = static_cast<int32_t>(y) * platform::g_screen_res_x + static_cast<int32_t>(x);
        framebuffer[offset] = color;

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Tint_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t tint_color) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        if (x2 < x1) {
            strings::Swap_Short_(&x1, &x2);
        }
        if (y2 < y1) {
            strings::Swap_Short_(&y1, &y2);
        }

        if (animate::_clipping_flag == 1) {
            if (x1 < video::_screen_window_x1) {
                x1 = video::_screen_window_x1;
            }
            if (y1 < video::_screen_window_y1) {
                y1 = video::_screen_window_y1;
            }
            if (video::_screen_window_x2 < x2) {
                x2 = video::_screen_window_x2;
            }
            if (video::_screen_window_y2 < y2) {
                y2 = video::_screen_window_y2;
            }
        }

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(x1, y1, x2, y2);
        }

        int start_col = x1 / 4;
        int end_col = x2 / 4;

        if (start_col == end_col) {
            uint8_t* dest = video::_current_video_page + (y1 * platform::g_screen_res_x) + x1;
            int height = y2 - y1 + 1;
            int width = x2 - x1 + 1;
            uint8_t* color_map = (uint8_t*)&video::_glass_remap_colors + (tint_color * 256);
            regions::Tint_Fill_Column_(color_map, dest, width, height);
        } else {
            // Potential bug: original game completely ignores tint_color in this branch, defaulting to offset 0
            int start_pixels = 4 - (x1 & 3);
            int middle_blocks = end_col - start_col - 1;
            int height = y2 - y1 + 1;
            int end_pixels = x2 & 3;
            int offset_y = y1 * platform::g_screen_res_x;

            if (start_pixels == 4) {
                // Potential bug: start block is skipped if middle_blocks is 0
                if (middle_blocks != 0) {
                    regions::Tint_Fill_Region_(
                        (uint8_t*)&video::_glass_remap_colors,
                        video::_current_video_page + offset_y + start_col * 4,
                        middle_blocks + 1,
                        height
                    );
                }
            } else {
                regions::Tint_Fill_Column_(
                    (uint8_t*)&video::_glass_remap_colors,
                    video::_current_video_page + offset_y + x1,
                    start_pixels,
                    height
                );
                regions::Tint_Fill_Region_(
                    (uint8_t*)&video::_glass_remap_colors,
                    video::_current_video_page + offset_y + start_col * 4 + 4,
                    middle_blocks,
                    height
                );
            }

            // Potential bug: end_pixels logic misses the +1 offset, often skipping the final pixel at x2
            if (end_pixels != 0) {
                regions::Tint_Fill_Column_(
                    (uint8_t*)&video::_glass_remap_colors,
                    video::_current_video_page + offset_y + end_col * 4,
                    end_pixels,
                    height
                );
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Interlaced_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color1, int16_t color2) {
        int32_t start_col;
        int32_t end_col;
        int32_t left_rem;
        int32_t right_rem;
        int32_t mid_cols;
        int32_t offset;
        int32_t height;
        uint32_t fill_val;

        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
        fill_val = (color2 << 8) + color1;

        if (animate::_clipping_flag == 1) {
            if (x1 < video::_screen_window_x1) {
                x1 = video::_screen_window_x1;
            }
            if (y1 < video::_screen_window_y1) {
                y1 = video::_screen_window_y1;
            }
            if (x2 > video::_screen_window_x2) {
                x2 = video::_screen_window_x2;
            }
            if (y2 > video::_screen_window_y2) {
                y2 = video::_screen_window_y2;
            }
        }

        if (x2 < x1) {
            platform::Unlock_Appropriate_Render_Buffer();
            return;
        }

        if (y2 < y1) {
            platform::Unlock_Appropriate_Render_Buffer();
            return;
        }

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(x1, y1, x2, y2);
        }

        start_col = x1 / 4;
        end_col = x2 / 4;

        if (end_col == start_col) {
            regions::Interlaced_Fill_Column_(
                fill_val,
                video::_current_video_page + y1 * platform::g_screen_res_x + x1,
                x2 - x1 + 1,
                y2 - y1 + 1
            );
        } else {
            left_rem = 4 - (x1 & 3);
            right_rem = (x2 & 3) + 1;
            mid_cols = end_col - start_col - 1;

            if (left_rem == 4) {
                left_rem = 0;
                mid_cols++;
            }

            if (right_rem == 4) {
                right_rem = 0;
                mid_cols++;
            }

            offset = y1 * platform::g_screen_res_x;
            height = y2 - y1 + 1;

            if (left_rem == 0) {
                if (mid_cols != 0) {
                    regions::Interlaced_Fill_Region_(
                        fill_val,
                        video::_current_video_page + offset + start_col * 4,
                        mid_cols,
                        height
                    );
                }
            } else {
                regions::Interlaced_Fill_Column_(
                    fill_val,
                    video::_current_video_page + offset + x1,
                    left_rem,
                    height
                );
                regions::Interlaced_Fill_Region_(
                    fill_val,
                    video::_current_video_page + offset + start_col * 4 + 4,
                    mid_cols,
                    height
                );
            }

            if (right_rem != 0) {
                regions::Interlaced_Fill_Column_(
                    fill_val,
                    video::_current_video_page + offset + end_col * 4,
                    right_rem,
                    height
                );
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Tri_Interlaced_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color1, int16_t color2, int16_t color3) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        uint32_t color = (static_cast<uint32_t>(color3) << 16) + (static_cast<uint32_t>(color2) << 8) + static_cast<uint32_t>(color1);

        if (x1 > x2) {
            strings::Swap_Short_(&x1, &x2);
        }

        if (y1 > y2) {
            strings::Swap_Short_(&y1, &y2);
        }

        if (animate::_clipping_flag == 1) {
            if (x1 < video::_screen_window_x1) {
                x1 = video::_screen_window_x1;
            }
            if (y1 < video::_screen_window_y1) {
                y1 = video::_screen_window_y1;
            }
            if (x2 > video::_screen_window_x2) {
                x2 = video::_screen_window_x2;
            }
            if (y2 > video::_screen_window_y2) {
                y2 = video::_screen_window_y2;
            }
        }

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(x1, y1, x2, y2);
        }

        int32_t x1_blk = x1 / 4;
        int32_t x2_blk = x2 / 4;

        if (x1_blk == x2_blk) {
            int32_t y_offset = y1 * platform::g_screen_res_x;
            uint8_t* dest_ptr = video::_current_video_page + y_offset + x1;
            int32_t height = y2 - y1 + 1;
            int32_t width = x2 - x1 + 1;
            regions::Tri_Interlaced_Fill_Column_(color, dest_ptr, width, height);
        } else {
            int32_t left_cols = 4 - (x1 & 3);
            int32_t middle_blocks = x2_blk - x1_blk - 1;
            int32_t right_cols = (x2 & 3) + 1;

            if (left_cols == 4) {
                left_cols = 0;
                middle_blocks++;
            }

            if (right_cols == 4) {
                right_cols = 0;
                middle_blocks++;
            }

            int32_t height = y2 - y1 + 1;
            int32_t y_offset = y1 * platform::g_screen_res_x;

            if (left_cols != 0) {
                uint8_t* dest_ptr = video::_current_video_page + y_offset + x1;
                regions::Tri_Interlaced_Fill_Column_(color, dest_ptr, left_cols, height);

                uint8_t* region_ptr = video::_current_video_page + y_offset + x1_blk * 4 + 4;
                regions::Tri_Interlaced_Fill_Region_(color, region_ptr, middle_blocks, height);
            } else {
                if (middle_blocks != 0) {
                    uint8_t* region_ptr = video::_current_video_page + y_offset + x1_blk * 4;
                    regions::Tri_Interlaced_Fill_Region_(color, region_ptr, middle_blocks, height);
                }
            }

            if (right_cols != 0) {
                uint8_t* dest_ptr = video::_current_video_page + y_offset + x2_blk * 4;
                regions::Tri_Interlaced_Fill_Column_(color, dest_ptr, right_cols, height);
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Gray_Scale_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t* color_ptr, int32_t extra, int32_t param_7) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        if (x2 < x1) {
            strings::Swap_Short_(&x1, &x2);
        }
        if (y2 < y1) {
            strings::Swap_Short_(&y1, &y2);
        }

        if (animate::_clipping_flag == 1) {
            if (x1 < video::_screen_window_x1) {
                x1 = video::_screen_window_x1;
            }
            if (y1 < video::_screen_window_y1) {
                y1 = video::_screen_window_y1;
            }
            if (x2 > video::_screen_window_x2) {
                x2 = video::_screen_window_x2;
            }
            if (y2 > video::_screen_window_y2) {
                y2 = video::_screen_window_y2;
            }
        }

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(x1, y1, x2, y2);
        }

        replace::_palette_color_list = replace::_graphics_buffer;
        replace::Establish_Color_Gradient_(static_cast<int16_t>(extra), static_cast<int16_t>(param_7));

        int32_t gap = platform::g_screen_res_x - (x2 - x1) - 1;

        for (int i = 0; i < 256; ++i) {
            uint8_t color_idx = replace::_palette_color_list[i];
            replace::_palette_color_list[i] = color_ptr[color_idx];
        }

        uint8_t* dest_ptr = video::_current_video_page + (y1 * platform::g_screen_res_x) + x1;
        int32_t width = (x2 - x1) + 1;
        int32_t height = (y2 - y1) + 1;

        shear::Gray_Scale_Fill_Pixels_(dest_ptr, replace::_palette_color_list, gap, width, height);

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Gradient_Fill_(int x1, int y1, int x2, int y2, int mode, char *color_ptr, int extra, s_animation_header*pic, s_colors* colors) {
        switch (mode) {
        case 1:
            Fill_(static_cast<int16_t>(x1), static_cast<int16_t>(y1), static_cast<int16_t>(x2), static_cast<int16_t>(y2), static_cast<int8_t>(*color_ptr));
            break;
        case 3:
            Tint_Fill_(static_cast<int16_t>(x1), static_cast<int16_t>(y1), static_cast<int16_t>(x2), static_cast<int16_t>(y2), 0);
            break;
        case 15:
            Tint_Fill_(static_cast<int16_t>(x1), static_cast<int16_t>(y1), static_cast<int16_t>(x2), static_cast<int16_t>(y2), static_cast<int8_t>(*color_ptr));
            break;
        case 16:
            Gray_Scale_Fill_(static_cast<int16_t>(x1), static_cast<int16_t>(y1), static_cast<int16_t>(x2), static_cast<int16_t>(y2), reinterpret_cast<uint8_t*>(color_ptr), extra, 0);
            break;
        }
    }
}
#include "deps.h"

namespace clipped {
    // TODO: This can be greatly simplified
    void __cdecl Clipped_Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* sprite) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        int16_t clip_x_skip_count;
        int16_t draw_x_start;
        if (x < video::_screen_window_x1) {
            clip_x_skip_count = video::_screen_window_x1 - x;
            draw_x_start = video::_screen_window_x1;
        }
        else {
            clip_x_skip_count = 0;
            draw_x_start = x;
        }

        int16_t clip_y_skip_count;
        int16_t draw_y_start;
        if (y < video::_screen_window_y1) {
            clip_y_skip_count = video::_screen_window_y1 - y;
            draw_y_start = video::_screen_window_y1;
        }
        else {
            clip_y_skip_count = 0;
            draw_y_start = y;
        }

        int16_t draw_x_end = animate::_animation_header->width + x - 1;
        if (draw_x_end > video::_screen_window_x2) {
            draw_x_end = video::_screen_window_x2;
        }

        int16_t draw_y_end = animate::_animation_header->height + y - 1;
        if (draw_y_end > video::_screen_window_y2) {
            draw_y_end = video::_screen_window_y2;
        }

        int32_t draw_width = draw_x_end - draw_x_start + 1;
        int32_t draw_height = draw_y_end - draw_y_start + 1;

        int16_t current_sprite_y = *(int16_t*)(sprite + 2);
        int32_t line_base = (int32_t)x + (int32_t)draw_y_start * platform::g_screen_res_x;
        int32_t sprite_word_idx = 2;

        // Skip segments above the clipping window
        while (current_sprite_y < clip_y_skip_count) {
            int16_t count = *(int16_t*)&sprite[sprite_word_idx * 2];
            if (count == 0) {
                current_sprite_y += *(uint16_t*)&sprite[sprite_word_idx * 2 + 2];
            }
            else {
                sprite_word_idx += (count + 1) / 2;
            }
            sprite_word_idx += 2;
        }

        // Adjust buffer pointer if first visible segment starts below initial drawing line
        if (clip_y_skip_count < current_sprite_y) {
            line_base += (current_sprite_y - clip_y_skip_count) * platform::g_screen_res_x;
        }

        int16_t current_line_idx = current_sprite_y - clip_y_skip_count;
        int32_t win_start_abs = line_base + clip_x_skip_count;
        int32_t win_end_abs = win_start_abs + draw_width;
        int32_t cursor_abs = line_base;

        while (current_line_idx < draw_height) {
            int16_t count = *(int16_t*)&sprite[sprite_word_idx * 2];
            int16_t val = *(int16_t*)&sprite[sprite_word_idx * 2 + 2];
            sprite_word_idx += 2;

            if (count == 0) {
                // Vertical skip
                current_line_idx += val;
                // Note: Assembly uses hardcoded 640 for vertical skips
                line_base += (int32_t)val * platform::g_screen_res_x;
                win_start_abs = line_base + clip_x_skip_count;
                cursor_abs = line_base;
                win_end_abs = win_start_abs + draw_width;
            }
            else {
                // Horizontal segment
                cursor_abs += val;
                int32_t run_start_abs = cursor_abs;
                int32_t run_end_abs = run_start_abs + count;
                uint32_t pixel_ptr = sprite_word_idx * 2;

                if (run_start_abs < win_end_abs) {
                    // Clip left
                    if (run_start_abs < win_start_abs) {
                        int32_t skip_px = win_start_abs - run_start_abs;
                        if (count < skip_px) {
                            skip_px = count;
                        }
                        pixel_ptr += skip_px;
                        run_start_abs += skip_px;
                        cursor_abs += skip_px;
                    }

                    // Clip right
                    int32_t run_clipped_end = win_end_abs;
                    if (run_end_abs < win_end_abs) {
                        run_clipped_end = run_end_abs;
                    }

                    // Draw visible run
                    uint8_t* screen_ptr = (uint8_t*)(video::_current_video_page + run_start_abs);
                    while (run_start_abs < run_clipped_end) {
                        *screen_ptr++ = sprite[pixel_ptr++];
                        run_start_abs++;
                        cursor_abs++;
                    }
                }
                sprite_word_idx += (count + 1) / 2;
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Clipped_Draw_Stenciled_Animated_Sprite_(int16_t x, int16_t y, uint8_t* sprite) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        int16_t left_excess = 0;
        int16_t render_x_start = x;
        if (x < video::_screen_window_x1) {
            left_excess = video::_screen_window_x1 - x;
            render_x_start = video::_screen_window_x1;
        }

        int16_t top_excess = 0;
        int16_t render_y_start = y;
        if (y < video::_screen_window_y1) {
            top_excess = video::_screen_window_y1 - y;
            render_y_start = video::_screen_window_y1;
        }

        int16_t sprite_right_limit = x + animate::_animation_header->width - 1;
        if (sprite_right_limit > video::_screen_window_x2) {
            sprite_right_limit = video::_screen_window_x2;
        }

        int16_t sprite_bottom_limit = y + animate::_animation_header->height - 1;
        if (sprite_bottom_limit > video::_screen_window_y2) {
            sprite_bottom_limit = video::_screen_window_y2;
        }

        int16_t draw_width_limit = sprite_right_limit - render_x_start + 1;
        int16_t draw_height_limit = sprite_bottom_limit - render_y_start + 1;

        int32_t current_row_rel = *reinterpret_cast<uint16_t*>(sprite + 2);
        int32_t screen_y_offset = (int32_t)render_y_start * platform::g_screen_res_x + (int32_t)x;

        uint32_t sprite_word_idx = 2; // Offset in 16-bit words

        // Row skip loop: find the starting visible row
        while (current_row_rel < top_excess) {
            uint16_t count = *reinterpret_cast<uint16_t*>(sprite + sprite_word_idx * 2);
            uint16_t data = *reinterpret_cast<uint16_t*>(sprite + sprite_word_idx * 2 + 2);
            if (count == 0) {
                current_row_rel += data;
            } else {
                sprite_word_idx += (count + 1) / 2;
            }
            sprite_word_idx += 2;
        }

        // Adjust screen offset if we skipped rows
        if (current_row_rel > top_excess) {
            screen_y_offset += (current_row_rel - top_excess) * platform::g_screen_res_x;
        }

        int32_t current_row_offset = screen_y_offset;
        int32_t render_x_start_offset = screen_y_offset + left_excess;
        int32_t render_x_end_limit = screen_y_offset + left_excess + draw_width_limit;

        bool is_drawing_run = false;
        int32_t first_run_pixel_offset = current_row_offset;
        int32_t last_run_pixel_offset = current_row_offset;

        // Main drawing loop
        while (current_row_rel < draw_height_limit + top_excess) {
            sprite_word_idx += 2;
            uint16_t count = *reinterpret_cast<uint16_t*>(sprite + sprite_word_idx * 2 - 4);
            uint16_t x_offset = *reinterpret_cast<uint16_t*>(sprite + sprite_word_idx * 2 - 2);

            if (count == 0) {
                // Vertical row skip command
                current_row_rel += x_offset;
                if (is_drawing_run) {
                    // Record scanline coverage if row was processed
                    video::Add_Line_To_Scan_List_(
                        render_x_start_offset % 640,
                        first_run_pixel_offset / 640,
                        last_run_pixel_offset % 640
                    );
                }

                // Update row tracking variables
                int32_t skip_pixels = x_offset * 640;
                current_row_offset += skip_pixels;
                render_x_start_offset = current_row_offset + left_excess;
                render_x_end_limit = render_x_start_offset + draw_width_limit;

                is_drawing_run = false;
                first_run_pixel_offset = current_row_offset;
            } else {
                // Horizontal pixel run command
                int32_t run_screen_start = current_row_offset + x_offset;
                int32_t run_byte_idx = sprite_word_idx * 2;
                int32_t run_end_limit = run_screen_start + count;

                // Horizontal clipping
                if (run_screen_start < render_x_end_limit) {
                    if (run_screen_start < render_x_start_offset) {
                        int16_t clip_left = (int16_t)(render_x_start_offset - run_screen_start);
                        if (count < clip_left) {
                            clip_left = count;
                        }
                        run_byte_idx += clip_left;
                        run_screen_start += clip_left;
                    }

                    int32_t visible_run_end = run_screen_start + (render_x_end_limit - run_screen_start);
                    if (run_end_limit < visible_run_end) {
                        visible_run_end = run_end_limit;
                    }

                    // Copy pixels to video buffer
                    while (run_screen_start < visible_run_end) {
                        if (!is_drawing_run) {
                            is_drawing_run = true;
                            first_run_pixel_offset = run_screen_start;
                        }
                        video::_current_video_page[run_screen_start] = sprite[run_byte_idx];
                        run_byte_idx++;
                        run_screen_start++;
                        last_run_pixel_offset = run_screen_start - 1;
                    }
                }
                // Advance word index past pixel data
                sprite_word_idx += (count + 1) / 2;
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Clipped_Draw_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* pixels) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        s_animation_header* header = animate::_animation_header;
        int32_t sprite_width = (int32_t)header->width;
        int32_t sprite_height = (int32_t)header->height;

        // Calculate horizontal clipping
        int32_t x_end = (int32_t)x + sprite_width - 1;
        if (x_end > (int32_t)video::_screen_window_x2) {
            x_end = (int32_t)video::_screen_window_x2;
        }

        int32_t clamped_x_start = (video::_screen_window_x1 > x) ? (int32_t)video::_screen_window_x1 : (int32_t)x;
        int32_t visible_width = x_end - clamped_x_start + 1;

        // Calculate vertical clipping
        int32_t y_end = (int32_t)y + sprite_height - 1;
        if (y_end > (int32_t)video::_screen_window_y2) {
            y_end = (int32_t)video::_screen_window_y2;
        }

        int32_t clamped_y_start = (video::_screen_window_y1 > y) ? (int32_t)video::_screen_window_y1 : (int32_t)y;
        int32_t visible_height = y_end - clamped_y_start + 1;

        // Initial destination pointer based on unclipped (x, y)
        uint8_t* dst = video::_current_video_page + (uint32_t)y * platform::g_screen_res_x + (uint32_t)x;

        // Adjust pointers and tracking for clipping
        int32_t scan_x = (int32_t)x;
        if (x < video::_screen_window_x1) {
            int32_t left_clip = (int32_t)video::_screen_window_x1 - (int32_t)x;
            dst += left_clip;
            pixels += left_clip;
            scan_x = left_clip; // BUG: this should likely be video::_screen_window_x1
        }

        int32_t scan_y = (int32_t)y;
        if (y < video::_screen_window_y1) {
            int32_t top_clip = (int32_t)video::_screen_window_y1 - (int32_t)y;
            pixels += top_clip * sprite_width;
            dst += top_clip * platform::g_screen_res_x;
            scan_y = top_clip; // BUG: this should likely be video::_screen_window_y1
        }

        // Add dirty rect if requested
        if (video::screen_refresh_flag > 1) {
            setvesa::Add_Square_To_Scan_List_(
                scan_x, 
                scan_y, 
                scan_x + visible_width - 1, 
                scan_y + visible_height - 1
            );
        }

        // Draw clipped rows
        if (visible_height > 0) {
            for (int32_t i = 0; i < visible_height; ++i) {
                draw::Color_Stream_Copy_(pixels, dst, visible_width);
                pixels += sprite_width;
                dst += platform::g_screen_res_x;
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Clipped_Draw_Glassed_Bitmap_Sprite_(int32_t x, int32_t y, uint8_t* sprite_data) {
        int32_t x_start = x;
        if (x < (int32_t)video::_screen_window_x1) {
            x_start = (int32_t)video::_screen_window_x1;
        }

        int32_t x_end = (int32_t)animate::_animation_header->width + x;
        if ((int32_t)video::_screen_window_x2 <= x_end) {
            x_end = (int32_t)video::_screen_window_x2;
        }

        int32_t y_start = y;
        if (y < (int32_t)video::_screen_window_y1) {
            y_start = (int32_t)video::_screen_window_y1;
        }

        int32_t y_end = (int32_t)animate::_animation_header->height + y;
        if ((int32_t)video::_screen_window_y2 <= y_end) {
            y_end = (int32_t)video::_screen_window_y2;
        }

        int32_t draw_height = y_end - y_start;
        int32_t draw_width = x_end - x_start;

        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        uint8_t* screen_ptr = video::_current_video_page + (y * platform::g_screen_res_x) + x;

        if (x < (int32_t)video::_screen_window_x1) {
            int32_t diff_x = (int32_t)video::_screen_window_x1 - x;
            screen_ptr += diff_x;
            sprite_data += diff_x;
        }

        if (y < (int32_t)video::_screen_window_y1) {
            int32_t diff_y = (int32_t)video::_screen_window_y1 - y;
            screen_ptr += diff_y * platform::g_screen_res_x;
            sprite_data += diff_y * animate::_animation_header->width;
        }

        if (draw_height > 0) {
            do {
                if (draw_width > 0) {
                    uint8_t* current_sprite_row = sprite_data;
                    uint8_t* current_screen_row = screen_ptr;
                    int32_t count = draw_width;

                    do {
                        uint8_t pixel = *current_sprite_row;
                        if (pixel != 0) {
                            if (pixel >= 0xF0) {
                                pixel = draw::_glass_lookup_table[pixel][*current_screen_row];
                            }
                            *current_screen_row = pixel;
                        }
                        current_screen_row++;
                        current_sprite_row++;
                        count--;
                    } while (count != 0);
                }

                screen_ptr += platform::g_screen_res_x;
                sprite_data += animate::_animation_header->width;
                draw_height--;
            } while (draw_height > 0);
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    // This is updated version of Clipped_Draw_Animated_Sprite_ because original implementation was not
    // handling clipping correctly.
    void __cdecl Clipped_Draw_Glassed_Animated_Sprite_(int32_t x, int32_t y, uint8_t* sprite) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        int16_t clip_x_skip_count;
        int16_t draw_x_start;
        if (x < video::_screen_window_x1) {
            clip_x_skip_count = video::_screen_window_x1 - x;
            draw_x_start = video::_screen_window_x1;
        }
        else {
            clip_x_skip_count = 0;
            draw_x_start = x;
        }

        int16_t clip_y_skip_count;
        int16_t draw_y_start;
        if (y < video::_screen_window_y1) {
            clip_y_skip_count = video::_screen_window_y1 - y;
            draw_y_start = video::_screen_window_y1;
        }
        else {
            clip_y_skip_count = 0;
            draw_y_start = y;
        }

        int16_t draw_x_end = animate::_animation_header->width + x - 1;
        if (draw_x_end > video::_screen_window_x2) {
            draw_x_end = video::_screen_window_x2;
        }

        int16_t draw_y_end = animate::_animation_header->height + y - 1;
        if (draw_y_end > video::_screen_window_y2) {
            draw_y_end = video::_screen_window_y2;
        }

        int32_t draw_width = draw_x_end - draw_x_start + 1;
        int32_t draw_height = draw_y_end - draw_y_start + 1;

        int16_t current_sprite_y = *(int16_t*)(sprite + 2);
        int32_t line_base = x + (int32_t)draw_y_start * platform::g_screen_res_x;
        int32_t sprite_word_idx = 2;

        // Skip segments above the clipping window
        while (current_sprite_y < clip_y_skip_count) {
            int16_t count = *(int16_t*)&sprite[sprite_word_idx * 2];
            if (count == 0) {
                current_sprite_y += *(uint16_t*)&sprite[sprite_word_idx * 2 + 2];
            }
            else {
                sprite_word_idx += (count + 1) / 2;
            }
            sprite_word_idx += 2;
        }

        // Adjust buffer pointer if first visible segment starts below initial drawing line
        if (clip_y_skip_count < current_sprite_y) {
            line_base += (current_sprite_y - clip_y_skip_count) * platform::g_screen_res_x;
        }

        int16_t current_line_idx = current_sprite_y - clip_y_skip_count;
        int32_t win_start_abs = line_base + clip_x_skip_count;
        int32_t win_end_abs = win_start_abs + draw_width;
        int32_t cursor_abs = line_base + clip_x_skip_count;

        while (current_line_idx < draw_height) {
            int16_t count = *(int16_t*)&sprite[sprite_word_idx * 2];
            int16_t val = *(int16_t*)&sprite[sprite_word_idx * 2 + 2];
            sprite_word_idx += 2;

            if (count == 0) {
                // Vertical skip
                current_line_idx += val;
                // Note: Assembly uses hardcoded 640 for vertical skips
                line_base += (int32_t)val * platform::g_screen_res_x;
                win_start_abs = line_base + clip_x_skip_count;
                cursor_abs = line_base;
                win_end_abs = win_start_abs + draw_width;
            }
            else {
                // Horizontal segment
                cursor_abs += val;
                int32_t run_start_abs = cursor_abs;
                int32_t run_end_abs = run_start_abs + count;
                uint32_t pixel_ptr = sprite_word_idx * 2;

                if (run_start_abs < win_end_abs) {
                    // Clip left
                    if (run_start_abs < win_start_abs) {
                        int32_t skip_px = win_start_abs - run_start_abs;
                        if (count < skip_px) {
                            skip_px = count;
                        }
                        pixel_ptr += skip_px;
                        run_start_abs += skip_px;
                        cursor_abs += skip_px;
                    }

                    // Clip right
                    int32_t run_clipped_end = win_end_abs;
                    if (run_end_abs < win_end_abs) {
                        run_clipped_end = run_end_abs;
                    }

                    // Draw visible run
                    uint8_t* screen_ptr = (uint8_t*)(video::_current_video_page + run_start_abs);
                    while (run_start_abs < run_clipped_end) {
                        uint8_t pixel = sprite[pixel_ptr++];
                        if (pixel < 0xF0) {
                            *screen_ptr++ = pixel;
                        }
                        else {
                            uint8_t background_pixel = *screen_ptr;
                            *screen_ptr++ = draw::_glass_lookup_table[pixel][background_pixel];
                        }
                        run_start_abs++;
                        cursor_abs++;
                    }
                }
                sprite_word_idx += (count + 1) / 2;
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }
}
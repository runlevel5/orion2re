#include "pch.h"

namespace animate {
    short __cdecl Get_Full_Store_Flag_(s_animation_header* header) {
        _animation_header = header;
        return (header->flags & 0x04) != 0;
    }

    void __cdecl Draw_Palette_(s_animation_header* header) {
        _animation_header = header;

        if (header->flags & 0x10) {
            Set_Animation_Palette_();
        }
    }

    void __cdecl Draw_Screen_Background_On_(uint32_t* sprite) {
        int pitch = platform::Lock_Primary_Surface();
        int stride = (pitch + (pitch < 0 ? 3 : 0)) >> 2;
        uint32_t* vram = video::_video_memory;

        int sprite_idx = 0;

        for (int chunk_base = 0; chunk_base < 0x14000; chunk_base += 0x4000) {
            int current_pixel_pos = chunk_base;

            while (true) {
                int skip = static_cast<int>(sprite[sprite_idx]);
                int count = static_cast<int>(sprite[sprite_idx + 1]);
                sprite_idx += 2;

                if (skip == 0 && count == 0) {
                    break;
                }

                current_pixel_pos += skip;
                int end_sprite_idx = sprite_idx + count;

                while (sprite_idx < end_sprite_idx) {
                    if (current_pixel_pos >= 0x12C00) {
                        break;
                    }

                    int y = current_pixel_pos / 160;
                    int x = current_pixel_pos % 160;
                    vram[y * stride + x] = sprite[sprite_idx];

                    sprite_idx++;
                    current_pixel_pos++;
                }
            }
        }

        platform::Unlock_Primary_Surface();
    }

    void __cdecl Draw_Screen_Background_Off_(uint8_t* data) {
        uint32_t* sprite = reinterpret_cast<uint32_t*>(data);
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
        uint32_t* vpage = reinterpret_cast<uint32_t*>(video::_current_video_page);

        int sprite_idx = 0;
        for (int row_base = 0; row_base < 0x14000; row_base += 0x4000) {
            while (true) {
                uint32_t skip = sprite[sprite_idx];
                uint32_t count = sprite[sprite_idx + 1];
                sprite_idx += 2;

                if (skip == 0 && count == 0) {
                    break;
                }

                int dest_idx = row_base + skip;
                int sprite_end = sprite_idx + count;

                while (sprite_idx < sprite_end) {
                    if (dest_idx >= 0x12C00) {
                        break;
                    }

                    vpage[dest_idx] = sprite[sprite_idx];

                    dest_idx++;
                    sprite_idx++;
                }
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    int16_t __cdecl Get_Current_Frame_(s_animation_header* animation) {
        return animation->current_frame;
    }

    void __cdecl Set_Animation_Frame_(s_animation_header* anim, int32_t frame) {
        if (frame >= static_cast<int32_t>(anim->frame_count)) {
            frame = static_cast<int32_t>(anim->frame_count) - 1;
        }
        anim->current_frame = static_cast<int16_t>(frame);
    }

    int16_t __cdecl Get_Animation_Frame_Count_(s_animation_header* anim) {
        return anim->frame_count;
    }

    int16_t __cdecl Get_Width_(s_animation_header* anim) {
        return anim->width;
    }

    int16_t __cdecl Get_Height_(s_animation_header* animation) {
        return animation->height;
    }

    void __cdecl Reset_Animation_Frame_(s_animation_header* anim) {
        anim->current_frame = 0;
    }

    void __cdecl Draw_(int16_t x, int16_t y, s_animation_header* anim) {
        _animation_header = anim;

        if (anim->flags & 0x10) {
            Set_Animation_Palette_();
        }

        uint8_t* data = reinterpret_cast<uint8_t*>(anim) + anim->frame_offsets[anim->current_frame];

        int16_t x_end = anim->width + x - 1;
        int16_t y_end = anim->height + y - 1;
        bool needs_clipping = false;

        if (_clipping_flag == 1) {
            if (x > video::_screen_window_x2 || y > video::_screen_window_y2 ||
                x_end < video::_screen_window_x1 || y_end < video::_screen_window_y1) {
                goto update_frame;
            }

            if (x < video::_screen_window_x1 || y < video::_screen_window_y1 ||
                x_end > video::_screen_window_x2 || y_end > video::_screen_window_y2) {
                needs_clipping = true;
            }
        }

        if (needs_clipping) {
            if (_dot_stencil_flag == 0) {
                if (video::screen_refresh_flag > 1) {
                    setvesa::Add_Square_To_Scan_List_(x, y, x_end, y_end);
                }

                switch (anim->flags & 0x3) {
                case 0:
                    if (anim->flags & 0x8) clipped::Clipped_Draw_Glassed_Animated_Sprite_(x, y, data);
                    else clipped::Clipped_Draw_Animated_Sprite_(x, y, data);
                    break;
                case 1:
                    if (anim->flags & 0x8) clipped::Clipped_Draw_Glassed_Bitmap_Sprite_(x, y, data);
                    else clipped::Clipped_Draw_Bitmap_Sprite_(x, y, data);
                    break;
                case 2:
                    exitm::Exit_With_Message(s_Screen_Background_cant_be_clipp);
                    break;
                case 3:
                    exitm::Exit_With_Message(s_Refresh_Mask_cant_be_drawn);
                    break;
                }
            }
            else if ((anim->flags & 0x3) == 0) {
                clipped::Clipped_Draw_Stenciled_Animated_Sprite_(x, y, data);
            }
        }
        else {
            if (video::screen_refresh_flag > 1) {
                if (anim->width != 0 && anim->height != 0) {
                    setvesa::Add_Square_To_Scan_List_(x, y, x_end, y_end);
                }
            }

            uint8_t type = anim->flags & 0x3;
            bool glassed = (anim->flags & 0x8) != 0;

            if (!glassed) {
                switch (type) {
                case 0:
                    platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                    draw::Draw_Animated_Sprite_(x, y, data);
                    platform::Unlock_Appropriate_Render_Buffer();
                    break;
                case 1:
                    platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                    draw::Draw_Bitmap_Sprite_(x, y, data);
                    platform::Unlock_Appropriate_Render_Buffer();
                    break;
                case 2:
                    if (video::_copy_to_on_page_flag == 1) Draw_Screen_Background_On_(reinterpret_cast<uint32_t*>(data));
                    else Draw_Screen_Background_Off_(data);

                    if (video::screen_refresh_flag > 1) {
                        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                    }
                    break;
                case 3:
                    exitm::Exit_With_Message(s_Refresh_Mask_cant_be_drawn);
                    break;
                }
            }
            else {
                switch (type) {
                case 0:
                    Draw_Glassed_Animated_Sprite_(x, y, data);
                    break;
                case 1:
                    Draw_Glassed_Bitmap_Sprite_(x, y, data);
                    break;
                case 2:
                    if (video::_copy_to_on_page_flag == 1) Draw_Glassed_Screen_Background_On_(data);
                    else Draw_Glassed_Screen_Background_Off_(data);

                    if (video::screen_refresh_flag > 1) {
                        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                    }
                    break;
                case 3:
                    exitm::Exit_With_Message(s_Refresh_Mask_cant_be_drawn);
                    break;
                }
            }
        }

    update_frame:
        anim->current_frame++;
        if (anim->flags & 0x20) {
            if (anim->current_frame >= anim->frame_count) {
                anim->current_frame = static_cast<int16_t>(anim->loop_frame);
                return;
            }
        }
        else {
            if (anim->current_frame >= anim->frame_count) {
                anim->current_frame = static_cast<int16_t>(anim->loop_frame);
            }
        }
    }

    void __cdecl Draw_Glassed_Animated_Sprite_(int16_t x, int16_t y, uint8_t* sprite) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        int16_t current_y_offset = *reinterpret_cast<int16_t*>(&sprite[2]);
        uint32_t word_idx = 2;

        uint32_t dest_idx = (static_cast<int32_t>(y) + current_y_offset) * platform::g_screen_res_x + x;
        uint32_t line_start_idx = dest_idx;

        int32_t total_height = static_cast<int32_t>(_animation_header->height);
        uint8_t* video_ptr = video::_current_video_page;

        while (static_cast<int32_t>(current_y_offset) < total_height) {
            int16_t* cmd_stream = reinterpret_cast<int16_t*>(&sprite[word_idx * 2]);
            int16_t pixel_count = cmd_stream[0];
            int16_t skip_val = cmd_stream[1];
            word_idx += 2;

            if (pixel_count == 0) {
                current_y_offset += skip_val;
                line_start_idx += static_cast<int32_t>(skip_val) * 640;
                dest_idx = line_start_idx;
            }
            else {
                dest_idx += skip_val;
                uint8_t* pixel_data = &sprite[word_idx * 2];

                for (int i = 0; i < static_cast<int32_t>(pixel_count); ++i) {
                    uint8_t pixel = pixel_data[i];
                    if (pixel < 0xF0) {
                        video_ptr[dest_idx] = pixel;
                    }
                    else {
                        uint8_t background_pixel = video_ptr[dest_idx];
                        video_ptr[dest_idx] = draw::_glass_lookup_table[pixel][background_pixel];
                    }
                    dest_idx++;
                }

                word_idx += (static_cast<int32_t>(pixel_count) + 1) / 2;
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Draw_Glassed_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* sprite) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        s_animation_header* header = animate::_animation_header;
        int16_t sprite_w = header->width;
        int16_t sprite_h = header->height;

        int32_t screen_stride = platform::g_screen_res_x - (int32_t)sprite_w;
        uint8_t* v_page = video::_current_video_page;
        uint32_t screen_offset = (int32_t)y * platform::g_screen_res_x + (int32_t)x;
        uint32_t sprite_idx = 0;

        if (sprite_h > 0) {
            int32_t rows_remaining = (int32_t)sprite_h;
            do {
                if (sprite_w > 0) {
                    int32_t cols_remaining = (int32_t)sprite_w;
                    do {
                        uint8_t pixel = sprite[sprite_idx];
                        if (pixel != 0) {
                            if (pixel < 0xF0) {
                                v_page[screen_offset] = pixel;
                            }
                            else {
                                uint8_t existing_pixel = v_page[screen_offset];
                                v_page[screen_offset] = draw::_glass_lookup_table[pixel][existing_pixel];
                            }
                        }
                        sprite_idx++;
                        screen_offset++;
                        cols_remaining--;
                    } while (cols_remaining != 0);
                }
                screen_offset += screen_stride;
                rows_remaining--;
            } while (rows_remaining != 0);
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Draw_Glassed_Screen_Background_Off_(uint8_t* sprite) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        uint32_t sprite_idx = 0;

        for (uint32_t page_offset = 0; page_offset < 0x50000; page_offset += 0x10000) {
            uint32_t current_out_pos = page_offset;

            while (true) {
                int8_t skip_count = static_cast<int8_t>(sprite[sprite_idx]);
                int8_t run_length = static_cast<int8_t>(sprite[sprite_idx + 1]);
                sprite_idx += 2;

                if (run_length == 0) {
                    break;
                }

                current_out_pos += skip_count;

                uint32_t bytes_to_copy = static_cast<uint32_t>(run_length) * 4;

                for (uint32_t i = 0; i < bytes_to_copy; ++i) {
                    video::_current_video_page[current_out_pos++] = sprite[sprite_idx++];
                }
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Draw_Glassed_Screen_Background_On_(uint8_t* sprite) {
        int32_t pitch = static_cast<int32_t>(platform::Lock_Primary_Surface());
        uint32_t sprite_idx = 0;

        for (int32_t base_pixel_offset = 0; base_pixel_offset < 0x50000; base_pixel_offset += 0x10000) {
            int32_t current_pixel = base_pixel_offset;

            while (true) {
                int8_t skip = static_cast<int8_t>(sprite[sprite_idx]);
                int8_t count = static_cast<int8_t>(sprite[sprite_idx + 1]);
                sprite_idx += 2;

                if (count == 0) {
                    break;
                }

                current_pixel += skip;
                uint32_t pixel_run_end = sprite_idx + (count * 4);

                while (sprite_idx < pixel_run_end) {
                    uint8_t color = sprite[sprite_idx];

                    int32_t x = current_pixel % 640;
                    int32_t y = current_pixel / 640;
                    // TYPE MISMATCH: video::_video_memory is uint32_t* but used as uint8_t* here
                    uint8_t* dst = reinterpret_cast<uint8_t*>(video::_video_memory) + (y * pitch) + x;

                    if (color < 0xF0) {
                        *dst = color;
                    }
                    else {
                        *dst = draw::_glass_lookup_table[color][*dst];
                    }

                    current_pixel++;
                    sprite_idx++;
                }
            }
        }

        platform::Unlock_Primary_Surface();
    }
}
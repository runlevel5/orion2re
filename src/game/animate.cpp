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
                        video_ptr[dest_idx] = video::_glass_remap_colors[pixel - 0xF0][background_pixel];
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
                                v_page[screen_offset] = video::_glass_remap_colors[pixel - 0xF0][existing_pixel];
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
                        *dst = video::_glass_remap_colors[color - 0xF0][*dst];
                    }

                    current_pixel++;
                    sprite_idx++;
                }
            }
        }

        platform::Unlock_Primary_Surface();
    }

    void __cdecl Set_Animation_Palette_() {
        uint8_t* pal_hdr = reinterpret_cast<uint8_t*>(
            &_animation_header->frame_offsets[_animation_header->frame_count + 1]
            );
        _palette_header = reinterpret_cast<s_palette_header*>(pal_hdr);

        int16_t start_index = reinterpret_cast<int16_t*>(pal_hdr)[0];
        int16_t count = reinterpret_cast<int16_t*>(pal_hdr)[1];

        if (count > 0) {
            s_palette_entry* dst = &video::_current_palette[start_index];
            uint32_t* src = reinterpret_cast<uint32_t*>(pal_hdr + 4);

            for (int i = 0; i < count; ++i) {
                uint32_t src_val = src[i];
                uint32_t& dst_val = reinterpret_cast<uint32_t&>(dst[i]);

                if (dst_val != src_val) {
                    dst_val = src_val + 1;
                    dst[i].changed = 1;
                }
            }
        }
    }

    void __cdecl Draw_Animated_Sprite_Back_Diff_(int16_t x, int16_t y, uint8_t* frame_data) {
        g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
        platform::Lock_Offscreen_Surface_();

        int32_t current_y = *reinterpret_cast<uint16_t*>(frame_data + 2);
        int32_t draw_y = y + current_y;
        int32_t data_offset_words = 2;
        int32_t draw_x = x;
        int32_t screen_offset = draw_y * g_screen_res_x + x;
        int32_t height = _animation_header->height;

        while (current_y < height) {
            int32_t count = *reinterpret_cast<int16_t*>(frame_data + data_offset_words * 2);
            int32_t skip = *reinterpret_cast<int16_t*>(frame_data + data_offset_words * 2 + 2);
            data_offset_words += 2;

            if (count == 0) {
                draw_x = x;
                draw_y += skip;
                current_y += skip;
                screen_offset += skip * 640;
            } else {
                draw_x += skip;
                screen_offset += skip;

                setvesa::Add_Square_To_Scan_List_(draw_x, draw_y, draw_x + count, draw_y);

                int32_t end_screen_offset = screen_offset + count;
                if (screen_offset < end_screen_offset) {
                    draw_x += count;
                    int32_t data_byte_offset = data_offset_words * 2;
                    do {
                        uint8_t pixel = frame_data[data_byte_offset++];
                        platform::_offscreen_surface[screen_offset] = pixel;
                        video::_current_video_page[screen_offset] = pixel;
                        screen_offset++;
                    } while (screen_offset < end_screen_offset);
                }

                data_offset_words += (count + 1) / 2;
            }
        }

        platform::Unlock_Offscreen_Surface_();
        platform::Unlock_Appropriate_Render_Buffer();
    }

    void __cdecl Remap_Draw_(int16_t x, int16_t y, s_animation_header* anim) {
        _animation_header = anim;
        int16_t x2 = anim->width + x - 1;
        int16_t y2 = anim->height + y - 1;
        bool clipped = false;

        if (_clipping_flag == 1) {
            if (video::_screen_window_x2 < x) return;
            if (video::_screen_window_y2 < y) return;
            if (x2 < video::_screen_window_x1) return;
            if (y2 < video::_screen_window_y1) return;

            if (x < video::_screen_window_x1 ||
                y < video::_screen_window_y1 ||
                video::_screen_window_x2 < x2 ||
                video::_screen_window_y2 < y2) {

                clipped = true;
                if (video::_screen_window_x2 < x2) {
                    x2 = video::_screen_window_x2;
                }
                if (video::_screen_window_y2 < y2) {
                    y2 = video::_screen_window_y2;
                }
            }
        }

        uint8_t* frame_ptr = reinterpret_cast<uint8_t*>(anim) + anim->frame_offsets[anim->current_frame];

        if (clipped) {
            if (_dot_stencil_flag == 0) {
                if (video::screen_refresh_flag >= 2) {
                    setvesa::Add_Square_To_Scan_List_(x, y, x2, y2);
                }

                switch (anim->flags & 3) {
                case 0:
                    Remap_Clipped_Draw_Animated_Sprite_(x, y, frame_ptr);
                    return; // Note: Returns directly without advancing frame
                case 1:
                    exitm::Exit_With_Message(s_Remap_draw_not_support_bitmaps_00557fc4);
                    break;
                case 2:
                    exitm::Exit_With_Message(s_Remap_draw_not_support_screen_00557fa4);
                    break;
                case 3:
                    exitm::Exit_With_Message(s_Remap_draw_not_support_map_00557f88);
                    break;
                }
            }
        }
        else {
            if (video::screen_refresh_flag >= 2) {
                setvesa::Add_Square_To_Scan_List_(x, y, x + anim->width - 1, y + anim->height - 1);
            }

            switch (anim->flags & 3) {
            case 0:
                Remap_Draw_Animated_Sprite_(x, y, frame_ptr);
                break;
            case 1:
                exitm::Exit_With_Message(s_Remap_draw_not_support_bitmaps_00557fc4);
                break;
            case 2:
                exitm::Exit_With_Message(s_Remap_draw_not_support_screen_00557fa4);
                break;
            case 3:
                exitm::Exit_With_Message(s_Remap_draw_not_support_map_00557f88);
                break;
            }
        }

        anim->current_frame++;

        if ((anim->flags & 0x20) == 0) {
            if (anim->current_frame >= anim->frame_count) {
                anim->current_frame = static_cast<int16_t>(anim->loop_frame);
            }
        }
        else {
            int32_t loop32 = static_cast<int8_t>(anim->loop_frame);
            int32_t curr32 = anim->current_frame;

            if (loop32 < curr32) {
                if (loop32 - curr32 == -1) {
                    anim->current_frame = 0;
                    return;
                }
                if (curr32 >= anim->frame_count) {
                    anim->current_frame = anim->frame_count - 1;
                    return;
                }
            }
        }
    }

    void __cdecl Draw_No_Glass_(int16_t x, int16_t y, s_animation_header* header) {
        _animation_header = header;

        int16_t x2 = x + header->width - 1;
        int16_t y2 = y + header->height - 1;

        if ((header->flags & 0x10) != 0) {
            Set_Animation_Palette_();
        }

        uint8_t* sprite = reinterpret_cast<uint8_t*>(header) + header->frame_offsets[header->current_frame];

        bool is_clipped = false;

        if (_clipping_flag != 0 && (x > video::_screen_window_x2 || y > video::_screen_window_y2 ||
                                    x2 < video::_screen_window_x1 || y2 < video::_screen_window_y1)) {
            // Out of bounds, skip drawing entirely
        } else {
            if (_clipping_flag != 0) {
                if (x < video::_screen_window_x1 || y < video::_screen_window_y1 ||
                    x2 > video::_screen_window_x2 || y2 > video::_screen_window_y2) {
                    is_clipped = true;
                }
            }

            if (!is_clipped) {
                if (video::screen_refresh_flag > 1) {
                    setvesa::Add_Square_To_Scan_List_(x, y, x2, y2);
                }

                if ((header->flags & 0x8) != 0) {
                    switch (header->flags & 3) {
                    case 0:
                        Draw_No_Glassed_Animated_Sprite_(x, y, sprite);
                        break;
                    case 1:
                        Draw_No_Glassed_Bitmap_Sprite_(x, y, sprite);
                        break;
                    case 2:
                        if (video::_copy_to_on_page_flag == 1) {
                            Draw_Glassed_Screen_Background_On_(sprite);
                        } else {
                            Draw_Glassed_Screen_Background_Off_(sprite);
                        }
                        if (video::screen_refresh_flag > 1) {
                            setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                        }
                        break;
                    case 3:
                        exitm::Exit_With_Message(s_Refresh_Mask_cant_be_drawn);
                        break;
                    }
                } else {
                    switch (header->flags & 3) {
                    case 0:
                        draw::Draw_Animated_Sprite_(x, y, sprite);
                        break;
                    case 1:
                        draw::Draw_Bitmap_Sprite_(x, y, sprite);
                        break;
                    case 2:
                        if (video::_copy_to_on_page_flag == 1) {
                            Draw_Screen_Background_On_(reinterpret_cast<uint32_t*>(sprite));
                        } else {
                            Draw_Screen_Background_Off_(sprite);
                        }
                        if (video::screen_refresh_flag > 1) {
                            setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                        }
                        break;
                    case 3:
                        exitm::Exit_With_Message(s_Refresh_Mask_cant_be_drawn);
                        break;
                    }
                }
            } else {
                if (_dot_stencil_flag != 0) {
                    if ((header->flags & 3) == 0) {
                        clipped::Clipped_Draw_Stenciled_Animated_Sprite_(x, y, sprite);
                    }
                } else {
                    if (video::screen_refresh_flag > 1) {
                        setvesa::Add_Square_To_Scan_List_(x, y, x2, y2);
                    }

                    switch (header->flags & 3) {
                    case 0:
                        if ((header->flags & 0x8) != 0) {
                            clipped::Clipped_Draw_Glassed_Animated_Sprite_(x, y, sprite);
                        } else {
                            clipped::Clipped_Draw_Animated_Sprite_(x, y, sprite);
                        }
                        break;
                    case 1:
                        if ((header->flags & 0x8) != 0) {
                            clipped::Clipped_Draw_Glassed_Bitmap_Sprite_(x, y, sprite);
                        } else {
                            clipped::Clipped_Draw_Bitmap_Sprite_(x, y, sprite);
                        }
                        break;
                    case 2:
                        exitm::Exit_With_Message(s_Screen_Background_cant_be_clipp);
                        break;
                    case 3:
                        exitm::Exit_With_Message(s_Refresh_Mask_cant_be_drawn);
                        break;
                    }
                }
            }
        }

        _animation_header->current_frame++;

        if ((_animation_header->flags & 0x20) != 0) {
            int8_t loop_frame = static_cast<int8_t>(_animation_header->loop_frame);
            if (loop_frame < _animation_header->current_frame) {
                if (loop_frame - _animation_header->current_frame == -1) {
                    _animation_header->current_frame = 0;
                    return;
                }
                if (_animation_header->current_frame >= _animation_header->frame_count) {
                    _animation_header->current_frame = _animation_header->frame_count - 1;
                    return;
                }
            }
        } else {
            if (_animation_header->current_frame >= _animation_header->frame_count) {
                _animation_header->current_frame = static_cast<int16_t>(_animation_header->loop_frame);
            }
        }
    }
}
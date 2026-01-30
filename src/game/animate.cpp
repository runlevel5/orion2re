#include "deps.h"

namespace animate {
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
                    if (video::_copy_to_on_page_flag == 1) Draw_Screen_Background_On_(data);
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
}
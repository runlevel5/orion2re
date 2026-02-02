#include "deps.h"

namespace file_ani {

    // Suggested name: Draw_File_Animation
    void __cdecl Draw_File_Animation_() {
        animate::_animation_header = init::_io_buffer;
        
        if (animate::_animation_header->flags & 0x10) {
            animate::Set_Animation_Palette_();
        }

        int16_t current_idx = animate::_animation_header->current_frame;
        uint32_t frame_start_offset = animate::_animation_header->frame_offsets[current_idx];
        uint32_t frame_end_offset = animate::_animation_header->frame_offsets[current_idx + 1];
        uint32_t frame_size = frame_end_offset - frame_start_offset;

        if (allocate::Get_Max_Block_Space_(file_ani::_file_animation_buffer) < (int32_t)frame_size) {
            exitm::Exit_With_Message(s_File_Animation_Frame_Too_Large_0055b804);
        }

        uint8_t* sprite_ptr = file_ani::_file_animation_buffer;
        _fseek((FILE*)file_ani::_file_animation_fptr, file_ani::_file_animation_start + frame_start_offset, 0);
        _fread(sprite_ptr, frame_size, 1, (FILE*)file_ani::_file_animation_fptr);

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(0, 0, animate::_animation_header->width - 1, animate::_animation_header->height - 1);
        }

        uint8_t type = animate::_animation_header->flags & 0x3;
        bool is_glassed = (animate::_animation_header->flags & 0x8) != 0;

        if (!is_glassed) {
            switch (type) {
                case 0:
                    platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                    draw::Draw_Animated_Sprite_(0, 0, sprite_ptr);
                    platform::Unlock_Appropriate_Render_Buffer();
                    break;
                case 1:
                    platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                    draw::Draw_Bitmap_Sprite_(0, 0, sprite_ptr);
                    platform::Unlock_Appropriate_Render_Buffer();
                    break;
                case 2:
                    if (video::_copy_to_on_page_flag != 1) {
                        animate::Draw_Screen_Background_Off_(sprite_ptr);
                    }
                    if (video::screen_refresh_flag >= 2) {
                        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                    }
                    break;
                case 3:
                    exitm::Exit_With_Message(animate::s_Refresh_Mask_cant_be_drawn);
                    break;
            }
        } else {
            switch (type) {
                case 0:
                    animate::Draw_Glassed_Animated_Sprite_(0, 0, sprite_ptr);
                    break;
                case 1:
                    animate::Draw_Glassed_Bitmap_Sprite_(0, 0, sprite_ptr);
                    break;
                case 2:
                    if (video::_copy_to_on_page_flag != 1) {
                        animate::Draw_Glassed_Screen_Background_Off_(sprite_ptr);
                    }
                    if (video::screen_refresh_flag >= 2) {
                        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                    }
                    break;
                case 3:
                    exitm::Exit_With_Message(animate::s_Refresh_Mask_cant_be_drawn);
                    break;
            }
        }

        animate::_animation_header->current_frame++;
        if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
            animate::_animation_header->current_frame = (int16_t)animate::_animation_header->loop_frame;
        }
    }

}
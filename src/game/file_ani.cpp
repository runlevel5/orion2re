#include "pch.h"

namespace file_ani {

    uint8_t* _file_animation_buffer;
    uint32_t _file_animation_open;
    uint32_t _file_animation_length;
    uint32_t _file_animation_end;
    FILE* _file_animation_fptr;
    uint32_t _file_animation_opened_flag[3];
    FILE* _file_animation[3];
    uint32_t _file_animation_start_multi[3];
    uint32_t _file_animation_end_multi[3];
    uint32_t* _file_lbx_header;
    uint32_t _file_animation_length_multi[3];
    uint32_t _file_animation_start;
    uint8_t* _file_animation_header;

    int16_t __cdecl Get_File_Animation_Frame_() {
        _file_animation_header = (uint8_t*)init::_io_buffer;
        return *(int16_t*)(_file_animation_header + 4);
    }

    void __cdecl Set_File_Animation_Buffer_(void* buffer) {
        _file_animation_buffer = (uint8_t*)buffer;
    }

    void __cdecl Open_File_Animation_(const char* filename, int16_t entry_index) {
        _file_lbx_header = (uint32_t*)init::_io_buffer;

        if (entry_index < 0) {
            farload::Error_Handler_(filename, 4, entry_index);
        }

        if (_file_animation_open == 1) {
            fclose(_file_animation_fptr);
        }

        _file_animation_fptr = fopen_case(filename, "rb");
        _file_animation_open = 1;

        if (_file_animation_fptr == nullptr) {
            if (farload::_secondary_drive_path[0] == '\0') {
                farload::Error_Handler_(filename, 1, entry_index);
            }

            char full_path[60];
            char* dst = full_path;
            const char* src = farload::_secondary_drive_path;
            while ((*dst++ = *src++) != '\0');

            dst--;
            src = filename;
            while ((*dst++ = *src++) != '\0');

            _file_animation_fptr = fopen_case(full_path, "rb");
            if (_file_animation_fptr == nullptr) {
                farload::Error_Handler_(full_path, 1, entry_index);
            }
        }

        fread(_file_lbx_header, 0x804, 1, _file_animation_fptr);

        if (*(uint16_t*)(((uint8_t*)_file_lbx_header) + 2) != 0xFEAD) {
            farload::Error_Handler_(filename, 7, entry_index);
        }

        if (*(int16_t*)_file_lbx_header <= entry_index) {
            farload::Error_Handler_(filename, 8, entry_index);
        }

        _file_animation_start = _file_lbx_header[entry_index + 2];
        _file_animation_end = _file_lbx_header[entry_index + 3];
        _file_animation_length = _file_animation_end - _file_animation_start;

        fseek(_file_animation_fptr, _file_animation_start, 0);

        fread(_file_lbx_header, 0x2000, 1, _file_animation_fptr);
    }

    void __cdecl thunk_Draw_File_Animation_() {
        Draw_File_Animation_();
    }

    void __cdecl Draw_File_Animation_() {
        animate::_animation_header = (s_animation_header*)init::_io_buffer;
        
        if ((animate::_animation_header->flags & ANIMATION_FLAG_HAS_PALETTE) != 0) {
            animate::Set_Animation_Palette_();
        }
        
        int16_t current_frame = animate::_animation_header->current_frame;
        const uint32_t* frame_offsets = Animation_Frame_Offsets_(animate::_animation_header);
        uint32_t frame_offset = frame_offsets[current_frame];
        uint32_t next_frame_offset = frame_offsets[current_frame + 1];
        
        int32_t file_offset = _file_animation_start + frame_offset;
        uint32_t frame_size = next_frame_offset - frame_offset;
        
        int32_t max_block_space = allocate::Get_Max_Block_Space_(_file_animation_buffer);
        if (max_block_space < (int32_t)frame_size) {
            exitm::Exit_With_Message("File Animation Frame Too Large");
        }
        
        fseek(_file_animation_fptr, file_offset, 0);
        fread(_file_animation_buffer, frame_size, 1, _file_animation_fptr);
        
        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(
                0, 
                0, 
                animate::_animation_header->width - 1, 
                animate::_animation_header->height - 1
            );
        }
        
        uint8_t flags = animate::_animation_header->flags;
        if ((flags & ANIMATION_FLAG_GLASSED) != 0) {
            switch (flags & ANIMATION_FLAG_DRAW_MODE_MASK) {
                case ANIMATION_DRAW_MODE_ANIMATED_SPRITE:
                    animate::Draw_Glassed_Animated_Sprite_(0, 0, _file_animation_buffer);
                    break;
                case ANIMATION_DRAW_MODE_BITMAP:
                    animate::Draw_Glassed_Bitmap_Sprite_(0, 0, _file_animation_buffer);
                    break;
                case ANIMATION_DRAW_MODE_SCREEN_BACKGROUND:
                    animate::Draw_Glassed_Screen_Background_Off_(_file_animation_buffer);
                    if (video::screen_refresh_flag >= 2) {
                        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                    }
                    break;
                case ANIMATION_DRAW_MODE_REFRESH_MASK:
                default:
                    exitm::Exit_With_Message("Refresh Mask can't be drawn");
                    break;
            }
        } else {
            switch (flags & ANIMATION_FLAG_DRAW_MODE_MASK) {
                case ANIMATION_DRAW_MODE_ANIMATED_SPRITE:
                    video::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                    draw::Draw_Animated_Sprite_(0, 0, _file_animation_buffer);
                    platform::Unlock_Appropriate_Render_Buffer();
                    break;
                case ANIMATION_DRAW_MODE_BITMAP:
                    video::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                    draw::Draw_Bitmap_Sprite_(0, 0, _file_animation_buffer);
                    platform::Unlock_Appropriate_Render_Buffer();
                    break;
                case ANIMATION_DRAW_MODE_SCREEN_BACKGROUND:
                    animate::Draw_Screen_Background_Off_(_file_animation_buffer);
                    if (video::screen_refresh_flag >= 2) {
                        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                    }
                    break;
                case ANIMATION_DRAW_MODE_REFRESH_MASK:
                default:
                    exitm::Exit_With_Message("Refresh Mask can't be drawn");
                    break;
            }
        }
        
        animate::_animation_header->current_frame++;
        
        if ((animate::_animation_header->flags & 0x20) == 0) {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                animate::_animation_header->current_frame = animate::_animation_header->loop_frame;
            }
        } else {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                animate::_animation_header->current_frame = animate::_animation_header->loop_frame;
            }
        }
    }

    void __cdecl Draw_File_Animation_Stencil_() {
        uint32_t saved_refresh_flag = video::screen_refresh_flag;
        animate::_animation_header = init::_io_buffer;
        video::screen_refresh_flag = 2;

        if ((int32_t)animate::_animation_header->current_frame - (int32_t)animate::_animation_header->frame_count == -1) {
            video::screen_refresh_flag = saved_refresh_flag;
        }

        if (animate::_animation_header->flags & ANIMATION_FLAG_HAS_PALETTE) {
            animate::Set_Animation_Palette_();
        }

        int16_t current_frame = animate::_animation_header->current_frame;
        const uint32_t* frame_offsets = Animation_Frame_Offsets_(animate::_animation_header);
        uint32_t frame_offset = frame_offsets[current_frame];
        uint32_t next_frame_offset = frame_offsets[current_frame + 1];
        uint32_t frame_size = next_frame_offset - frame_offset;

        int32_t available_space = allocate::Get_Max_Block_Space_(_file_animation_buffer);
        if (available_space < (int32_t)frame_size) {
            exitm::Exit_With_Message("File Animation Frame Too Large");
        }

        fseek(_file_animation_fptr, _file_animation_start + frame_offset, 0);
        fread(_file_animation_buffer, frame_size, 1, _file_animation_fptr);

        if (animate::_animation_header->flags & ANIMATION_FLAG_GLASSED) {
            exitm::Exit_With_Message("Differentail File Animation Can not be glassed");
        }

        switch (animate::_animation_header->flags & ANIMATION_FLAG_DRAW_MODE_MASK) {
        case ANIMATION_DRAW_MODE_ANIMATED_SPRITE:
            animate::Draw_Animated_Sprite_Back_Diff_(0, 0, _file_animation_buffer);
            break;
        case ANIMATION_DRAW_MODE_BITMAP:
            exitm::Exit_With_Message("Differentail File Animation Can not be bitmap");
            break;
        case ANIMATION_DRAW_MODE_SCREEN_BACKGROUND:
            exitm::Exit_With_Message("Differentail File Animation Can not be masked");
            break;
        case ANIMATION_DRAW_MODE_REFRESH_MASK:
            exitm::Exit_With_Message("Refresh Mask can't be drawn");
            break;
        }

        animate::_animation_header->current_frame++;

        if (animate::_animation_header->flags & 0x20) {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                int16_t loop_frame = animate::_animation_header->loop_frame;
                if (loop_frame - animate::_animation_header->frame_count != -1) {
                    animate::_animation_header->current_frame = loop_frame;
                } else {
                    animate::_animation_header->current_frame = 0;
                }
                video::screen_refresh_flag = saved_refresh_flag;
                return;
            }
        } else {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                animate::_animation_header->current_frame = animate::_animation_header->frame_count - 1;
            }
        }

        video::screen_refresh_flag = saved_refresh_flag;
    }

    int16_t __cdecl Get_File_Animation_Total_Frames_() {
        if (!_file_animation_open) {
            const char* message = HAROLD::H_Message_(206);
            exitm::Exit_With_Message(message);
        }

        return init::_io_buffer->frame_count - 1;
    }

    void __cdecl Set_File_Animation_Frame_(int16_t frame_index) {
        _file_animation_header = (uint8_t*)init::_io_buffer;

        int16_t max_frames = init::_io_buffer->frame_count;
        if (frame_index >= max_frames) {
            frame_index = max_frames - 1;
        }

        init::_io_buffer->current_frame = frame_index;
    }

    void __cdecl Open_Multi_File_Animation_(const char* filename, int16_t entry_index, int16_t animation_slot) {
        if (animation_slot >= 3) {
            exitm::Exit_With_Message("Max 3 file animations");
        }

        int32_t slot = animation_slot;
        _file_lbx_header = (uint32_t*)(((uint8_t*)init::_io_buffer) + slot * 0xAAA);

        if (entry_index < 0) {
            farload::Error_Handler_(filename, 4, entry_index);
        }

        if (_file_animation_opened_flag[slot] == 1) {
            fclose(_file_animation[slot]);
        }

        _file_animation[slot] = fopen_case(filename, "rb");

        if (_file_animation[slot] == nullptr) {
            if (farload::_secondary_drive_path[0] == '\0') {
                farload::Error_Handler_(filename, 1, entry_index);
            }

            char full_path[60];
            snprintf(full_path, sizeof(full_path), "%s%s", farload::_secondary_drive_path, filename);

            _file_animation[slot] = fopen_case(full_path, "rb");

            if (_file_animation[slot] == nullptr) {
                farload::Error_Handler_(full_path, 1, entry_index);
            }
        }

        _file_animation_opened_flag[slot] = 1;

        fread(_file_lbx_header, 0x804, 1, _file_animation[slot]);

        uint16_t* header16 = (uint16_t*)_file_lbx_header;
        if (header16[1] != 0xFEAD) {
            farload::Error_Handler_(filename, 7, entry_index);
        }

        if ((int16_t)header16[0] <= entry_index) {
            farload::Error_Handler_(filename, 8, entry_index);
        }

        uint32_t* header32 = (uint32_t*)_file_lbx_header;
        uint32_t start_offset = header32[entry_index + 2];
        uint32_t end_offset = header32[entry_index + 3];

        _file_animation_start_multi[slot] = start_offset;
        _file_animation_end_multi[slot] = end_offset;
        _file_animation_length_multi[slot] = end_offset - start_offset;

        fseek(_file_animation[slot], start_offset, 0);

        fread(_file_lbx_header, 0xAAA, 1, _file_animation[slot]);
    }

    void __cdecl Draw_Multi_File_Animation_Stencil_(int16_t anim_idx) {
        animate::_animation_header = (s_animation_header*)(
            ((uint8_t*)init::_io_buffer) + anim_idx * 2730
        );

        if (animate::_animation_header->frame_count >= 678) {
            exitm::Exit_With_Message("Too many frames for multi file animation");
        }

        if (animate::_animation_header->flags & ANIMATION_FLAG_HAS_PALETTE) {
            animate::Set_Animation_Palette_();
        }

        uint32_t start_multi = _file_animation_start_multi[anim_idx];
        int16_t current_frame = animate::_animation_header->current_frame;
        const uint32_t* frame_offsets = Animation_Frame_Offsets_(animate::_animation_header);
        uint32_t frame_offset = frame_offsets[current_frame];
        uint32_t next_frame_offset = frame_offsets[current_frame + 1];

        uint32_t frame_size = next_frame_offset - frame_offset;
        uint32_t read_offset = start_multi + frame_offset;

        if ((int32_t)allocate::Get_Max_Block_Space_(_file_animation_buffer) < (int32_t)frame_size) {
            exitm::Exit_With_Message("File Animation Frame Too Large");
        }

        fseek(_file_animation[anim_idx], read_offset, 0);
        fread(_file_animation_buffer, frame_size, 1, _file_animation[anim_idx]);

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(
                0, 
                0, 
                animate::_animation_header->width - 1, 
                animate::_animation_header->height - 1
            );
        }

        switch (animate::_animation_header->flags & ANIMATION_FLAG_DRAW_MODE_MASK) {
            case ANIMATION_DRAW_MODE_ANIMATED_SPRITE:
                animate::Draw_Animated_Sprite_Back_Diff_(0, 0, _file_animation_buffer);
                break;
            case ANIMATION_DRAW_MODE_BITMAP:
                exitm::Exit_With_Message("Differentail File Animation Can not be bitmap");
                break;
            case ANIMATION_DRAW_MODE_SCREEN_BACKGROUND:
                exitm::Exit_With_Message("Differentail File Animation Can not be masked");
                break;
            case ANIMATION_DRAW_MODE_REFRESH_MASK:
                exitm::Exit_With_Message("Refresh Mask can't be drawn");
                break;
        }

        animate::_animation_header->current_frame++;

        if (animate::_animation_header->flags & 0x20) {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                if (((int32_t)animate::_animation_header->loop_frame - animate::_animation_header->frame_count) == -1) {
                    animate::_animation_header->current_frame = 0;
                } else {
                    animate::_animation_header->current_frame = animate::_animation_header->loop_frame;
                }
            }
        } else {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                animate::_animation_header->current_frame = animate::_animation_header->frame_count - 1;
            }
        }
    }

    void __cdecl Draw_Multi_File_Animation_(int16_t anim_idx) {
        animate::_animation_header = (s_animation_header*)(
            ((uint8_t*)init::_io_buffer) + (anim_idx * 0xAAA)
        );

        if (animate::_animation_header->frame_count >= 0x2A6) {
            exitm::Exit_With_Message("Too many frames for multi file animation");
        }

        if ((animate::_animation_header->flags & ANIMATION_FLAG_HAS_PALETTE) != 0) {
            animate::Set_Animation_Palette_();
        }

        uint32_t start_offset = _file_animation_start_multi[anim_idx];
        int16_t current_frame = animate::_animation_header->current_frame;

        const uint32_t* frame_offsets = Animation_Frame_Offsets_(animate::_animation_header);
        uint32_t frame_offset = frame_offsets[current_frame];
        uint32_t next_frame_offset = frame_offsets[current_frame + 1];

        uint32_t read_offset = start_offset + frame_offset;
        uint32_t frame_size = next_frame_offset - frame_offset;

        int32_t max_space = allocate::Get_Max_Block_Space_(_file_animation_buffer);
        if (max_space < (int32_t)frame_size) {
            exitm::Exit_With_Message("File Animation Frame Too Large");
        }

        uint8_t* sprite = _file_animation_buffer;
        fseek(_file_animation[anim_idx], read_offset, 0);
        fread(sprite, frame_size, 1, _file_animation[anim_idx]);

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(
                0, 0,
                animate::_animation_header->width - 1,
                animate::_animation_header->height - 1
            );
        }

        uint8_t mode = animate::_animation_header->flags & ANIMATION_FLAG_DRAW_MODE_MASK;
        switch (mode) {
        case ANIMATION_DRAW_MODE_ANIMATED_SPRITE:
            draw::Draw_Animated_Sprite_(0, 0, sprite);
            break;
        case ANIMATION_DRAW_MODE_BITMAP:
            draw::Draw_Bitmap_Sprite_(0, 0, sprite);
            break;
        case ANIMATION_DRAW_MODE_SCREEN_BACKGROUND:
            animate::Draw_Screen_Background_Off_(sprite);
            if (video::screen_refresh_flag >= 2) {
                setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
            }
            break;
        case ANIMATION_DRAW_MODE_REFRESH_MASK:
            exitm::Exit_With_Message("Refresh Mask can't be drawn");
            break;
        }

        animate::_animation_header->current_frame++;

        if ((animate::_animation_header->flags & 0x20) != 0) {
            if (animate::_animation_header->frame_count <= animate::_animation_header->current_frame) {
                animate::_animation_header->current_frame = animate::_animation_header->loop_frame;
            }
        } else {
            if (animate::_animation_header->frame_count <= animate::_animation_header->current_frame) {
                animate::_animation_header->current_frame = animate::_animation_header->loop_frame;
            }
        }
    }

    int16_t __cdecl Get_Multi_File_Animation_Frame_(int16_t anim_idx) {
        _file_animation_header = ((uint8_t*)init::_io_buffer) + anim_idx * 0xAAA;
        return *(int16_t*)(_file_animation_header + 4);
    }

    void __cdecl Set_Multi_File_Animation_Frame_(int16_t anim_idx, int16_t frame_index) {
        _file_animation_header = ((uint8_t*)init::_io_buffer) + anim_idx * 0xAAA;

        int16_t max_frames = *(int16_t*)(_file_animation_header + 6);
        if (max_frames <= frame_index) {
            frame_index = max_frames - 1;
        }

        *(int16_t*)(_file_animation_header + 4) = frame_index;
    }

    void __cdecl Draw_Multi_File_Animation_Palette_(int32_t anim_idx) {
        animate::_animation_header = (s_animation_header*)(
            ((uint8_t*)init::_io_buffer) + anim_idx * 2730
        );

        if (animate::_animation_header->frame_count >= 678) {
            exitm::Exit_With_Message("Too many frames for multi file animation");
        }

        if ((animate::_animation_header->flags & ANIMATION_FLAG_HAS_PALETTE) != 0) {
            animate::Set_Animation_Palette_();
        }
    }

    void __cdecl Reset_File_Animation_Buffer_() {
        platform::Lock_Offscreen_Surface_();
        _file_animation_buffer = platform::_offscreen_surface;
        platform::Unlock_Offscreen_Surface_();
    }
}
#include "pch.h"

namespace file_ani {

    int16_t __cdecl Get_File_Animation_Frame_() {
        _file_animation_header = reinterpret_cast<uint8_t*>(init::_io_buffer);
        return *reinterpret_cast<int16_t*>(_file_animation_header + 4);
    }

    void __cdecl Set_File_Animation_Buffer_(void* buffer) {
        _file_animation_buffer = (uint8_t*)buffer;
    }

    void __cdecl Open_File_Animation_(const char* filename, int16_t entry_index) {
        _file_lbx_header = reinterpret_cast<uint32_t*>(init::_io_buffer);

        if (entry_index < 0) {
            farload::Error_Handler_(filename, 4, entry_index);
        }

        if (_file_animation_open) {
            fclose(_file_animation_fptr);
        }

        _file_animation_fptr = fopen(filename, s_rb_0055117c);
        _file_animation_open = true;

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

            _file_animation_fptr = fopen(full_path, s_rb_0055117c);
            if (_file_animation_fptr == nullptr) {
                farload::Error_Handler_(full_path, 1, entry_index);
            }
        }

        fread(_file_lbx_header, 0x804, 1, _file_animation_fptr);

        if (*reinterpret_cast<uint16_t*>((reinterpret_cast<uint8_t*>(_file_lbx_header) + 2)) != 0xFEAD) {
            farload::Error_Handler_(filename, 7, entry_index);
        }

        if (static_cast<int16_t>(*reinterpret_cast<uint16_t*>(_file_lbx_header)) <= entry_index) {
            farload::Error_Handler_(filename, 8, entry_index);
        }

        _file_animation_start = _file_lbx_header[entry_index + 2];
        _file_animation_end = _file_lbx_header[entry_index + 3];
        _file_animation_length = _file_animation_end - _file_animation_start;

        fseek(_file_animation_fptr, _file_animation_start, 0);

        fread(_file_lbx_header, 0x2000, 1, _file_animation_fptr);
    }

    void __cdecl Draw_File_Animation_() {
        animate::_animation_header = reinterpret_cast<s_animation_header*>(init::_io_buffer);
        
        if ((animate::_animation_header->flags & 0x10) != 0) {
            animate::Set_Animation_Palette_();
        }
        
        int16_t current_frame = animate::_animation_header->current_frame;
        uint32_t frame_offset = animate::_animation_header->frame_offsets[current_frame];
        uint32_t next_frame_offset = animate::_animation_header->frame_offsets[current_frame + 1];
        
        int32_t file_offset = _file_animation_start + frame_offset;
        uint32_t frame_size = next_frame_offset - frame_offset;
        
        int32_t max_block_space = allocate::Get_Max_Block_Space_(_file_animation_buffer);
        if (max_block_space < static_cast<int32_t>(frame_size)) {
            exitm::Exit_With_Message(s_File_Animation_Frame_Too_Large_0055b804);
        }
        
        fseek(_file_animation_fptr, file_offset, 0); // SEEK_SET
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
        if ((flags & 0x08) != 0) {
            switch (flags & 3) {
                case 0:
                    animate::Draw_Glassed_Animated_Sprite_(0, 0, _file_animation_buffer);
                    break;
                case 1:
                    animate::Draw_Glassed_Bitmap_Sprite_(0, 0, _file_animation_buffer);
                    break;
                case 2:
                    if (video::_copy_to_on_page_flag != 1) {
                        animate::Draw_Glassed_Screen_Background_Off_(_file_animation_buffer);
                    }
                    if (video::screen_refresh_flag >= 2) {
                        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                    }
                    break;
                case 3:
                default:
                    exitm::Exit_With_Message(s_Refresh_Mask_can_t_be_drawn_0055b10c);
                    break;
            }
        } else {
            switch (flags & 3) {
                case 0:
                    platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                    draw::Draw_Animated_Sprite_(0, 0, _file_animation_buffer);
                    platform::Unlock_Appropriate_Render_Buffer();
                    break;
                case 1:
                    platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                    draw::Draw_Bitmap_Sprite_(0, 0, _file_animation_buffer);
                    platform::Unlock_Appropriate_Render_Buffer();
                    break;
                case 2:
                    if (video::_copy_to_on_page_flag != 1) {
                        animate::Draw_Screen_Background_Off_(_file_animation_buffer);
                    }
                    if (video::screen_refresh_flag >= 2) {
                        setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                    }
                    break;
                case 3:
                default:
                    exitm::Exit_With_Message(s_Refresh_Mask_can_t_be_drawn_0055b10c);
                    break;
            }
        }
        
        animate::_animation_header->current_frame++;
        
        // This identical branching mimics the original compiled code accurately
        if ((animate::_animation_header->flags & 0x20) == 0) {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                animate::_animation_header->current_frame = static_cast<int16_t>(static_cast<int8_t>(animate::_animation_header->loop_frame));
            }
        } else {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                animate::_animation_header->current_frame = static_cast<int16_t>(static_cast<int8_t>(animate::_animation_header->loop_frame));
            }
        }
    }

    void __cdecl Draw_File_Animation_Stencil_() {
        uint32_t saved_refresh_flag = video::screen_refresh_flag;
        animate::_animation_header = init::_io_buffer;
        video::screen_refresh_flag = 2;

        if ((int)animate::_animation_header->current_frame - (int)animate::_animation_header->frame_count == -1) {
            video::screen_refresh_flag = saved_refresh_flag;
        }

        if (animate::_animation_header->flags & 0x10) {
            animate::Set_Animation_Palette_();
        }

        uint32_t current_frame_idx = animate::_animation_header->current_frame;
        uint32_t frame_offset = animate::_animation_header->frame_offsets[current_frame_idx];
        uint32_t next_frame_offset = animate::_animation_header->frame_offsets[current_frame_idx + 1];
        size_t frame_size = next_frame_offset - frame_offset;

        int32_t available_space = allocate::Get_Max_Block_Space_(_file_animation_buffer);
        if (available_space < (int32_t)frame_size) {
            exitm::Exit_With_Message(s_File_Animation_Frame_Too_Large_0055b804);
        }

        fseek(_file_animation_fptr, _file_animation_start + frame_offset, 0);
        fread(_file_animation_buffer, frame_size, 1, _file_animation_fptr);

        if (animate::_animation_header->flags & 0x08) {
            exitm::Exit_With_Message(s_Differential_Error_8C8);
        }

        switch (animate::_animation_header->flags & 0x03) {
        case 0:
            animate::Draw_Animated_Sprite_Back_Diff_(0, 0, _file_animation_buffer);
            break;
        case 1:
            exitm::Exit_With_Message(s_Differential_Error_898);
            break;
        case 2:
            exitm::Exit_With_Message(s_Differential_Error_868);
            break;
        case 3:
            exitm::Exit_With_Message(animate::s_Refresh_Mask_cant_be_drawn);
            break;
        }

        animate::_animation_header->current_frame++;

        if (animate::_animation_header->flags & 0x20) {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                int16_t loop_frame = (int16_t)(int8_t)animate::_animation_header->loop_frame;
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
        _file_animation_header = reinterpret_cast<uint8_t*>(init::_io_buffer);

        int16_t max_frames = init::_io_buffer->frame_count;
        if (frame_index >= max_frames) {
            frame_index = max_frames - 1;
        }

        init::_io_buffer->current_frame = frame_index;
    }

    void __cdecl Open_Multi_File_Animation_(const char* filename, int16_t file_index, int16_t animation_slot) {
        if (animation_slot >= 3) {
            exitm::Exit_With_Message("Max 3 file animations");
        }

        int32_t slot = animation_slot;
        _file_lbx_header = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(init::_io_buffer) + slot * 0xAAA);

        if (file_index < 0) {
            farload::Error_Handler_(filename, 4, file_index);
        }

        if (_file_animation_opened_flag[slot] == 1) {
            fclose(_file_animation[slot]);
        }

        _file_animation[slot] = fopen(filename, "rb");

        if (_file_animation[slot] == nullptr) {
            if (farload::_secondary_drive_path[0] == '\0') {
                farload::Error_Handler_(filename, 1, file_index);
            }

            char local_3c[60];
            _sprintf(local_3c, "%s%s", farload::_secondary_drive_path, filename);

            _file_animation[slot] = fopen(local_3c, "rb");

            if (_file_animation[slot] == nullptr) {
                farload::Error_Handler_(local_3c, 1, file_index);
            }
        }

        _file_animation_opened_flag[slot] = 1;

        fread(_file_lbx_header, 0x804, 1, _file_animation[slot]);

        uint16_t* header16 = reinterpret_cast<uint16_t*>(_file_lbx_header);
        if (header16[1] != 0xFEAD) {
            farload::Error_Handler_(filename, 7, file_index);
        }

        if (static_cast<int16_t>(header16[0]) <= file_index) {
            farload::Error_Handler_(filename, 8, file_index);
        }

        uint32_t* header32 = reinterpret_cast<uint32_t*>(_file_lbx_header);
        uint32_t start_offset = header32[file_index + 2];
        uint32_t end_offset = header32[file_index + 3];

        _file_animation_start_multi[slot] = start_offset;
        _file_animation_end_multi[slot] = end_offset;
        _file_animation_length_multi[slot] = end_offset - start_offset;

        fseek(_file_animation[slot], start_offset, 0);

        fread(_file_lbx_header, 0xAAA, 1, _file_animation[slot]);
    }

    void __cdecl Draw_Multi_File_Animation_Stencil_(int16_t anim_idx) {
        animate::_animation_header = reinterpret_cast<s_animation_header*>(
            reinterpret_cast<uint8_t*>(init::_io_buffer) + anim_idx * 2730
        );

        if (animate::_animation_header->frame_count >= 678) {
            exitm::Exit_With_Message(s_Too_many_frames_for_multi_file_a);
        }

        if (animate::_animation_header->flags & 0x10) {
            animate::Set_Animation_Palette_();
        }

        uint32_t start_multi = _file_animation_start_multi[anim_idx];
        int16_t current_frame = animate::_animation_header->current_frame;
        uint32_t frame_offset = animate::_animation_header->frame_offsets[current_frame];
        uint32_t next_frame_offset = animate::_animation_header->frame_offsets[current_frame + 1];

        uint32_t frame_size = next_frame_offset - frame_offset;
        uint32_t read_offset = start_multi + frame_offset;

        if (static_cast<int32_t>(allocate::Get_Max_Block_Space_(_file_animation_buffer)) < static_cast<int32_t>(frame_size)) {
            exitm::Exit_With_Message(s_File_Animation_Frame_Too_Large);
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

        switch (animate::_animation_header->flags & 3) {
            case 0:
                animate::Draw_Animated_Sprite_Back_Diff_(0, 0, _file_animation_buffer);
                break;
            case 1:
                exitm::Exit_With_Message(s_Differentail_File_Animation_Can_n_bitmap);
                break;
            case 2:
                exitm::Exit_With_Message(s_Differentail_File_Animation_Can_n_masked);
                break;
            case 3:
                exitm::Exit_With_Message(s_Refresh_Mask_can_t_be_drawn);
                break;
        }

        animate::_animation_header->current_frame++;

        if (animate::_animation_header->flags & 0x20) {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                if ((static_cast<int32_t>(static_cast<int8_t>(animate::_animation_header->loop_frame)) - animate::_animation_header->frame_count) == -1) {
                    animate::_animation_header->current_frame = 0;
                } else {
                    animate::_animation_header->current_frame = static_cast<int16_t>(static_cast<int8_t>(animate::_animation_header->loop_frame));
                }
            }
        } else {
            if (animate::_animation_header->current_frame >= animate::_animation_header->frame_count) {
                animate::_animation_header->current_frame = animate::_animation_header->frame_count - 1;
            }
        }
    }

    void __cdecl Draw_Multi_File_Animation_(int16_t anim_idx) {
        animate::_animation_header = reinterpret_cast<s_animation_header*>(
            reinterpret_cast<uint8_t*>(init::_io_buffer) + (anim_idx * 0xAAA)
        );

        if (animate::_animation_header->frame_count >= 0x2A6) {
            exitm::Exit_With_Message(s_Too_many_frames_for_multi_file_a_0055b83c);
        }

        if ((animate::_animation_header->flags & 0x10) != 0) {
            animate::Set_Animation_Palette_();
        }

        uint32_t start_offset = _file_animation_start_multi[anim_idx];
        int16_t current_frame = animate::_animation_header->current_frame;

        uint32_t frame_offset = animate::_animation_header->frame_offsets[current_frame];
        uint32_t next_frame_offset = animate::_animation_header->frame_offsets[current_frame + 1];

        uint32_t read_offset = start_offset + frame_offset;
        uint32_t frame_size = next_frame_offset - frame_offset;

        int32_t max_space = allocate::Get_Max_Block_Space_(_file_animation_buffer);
        if (max_space < static_cast<int32_t>(frame_size)) {
            exitm::Exit_With_Message(s_File_Animation_Frame_Too_Large_0055b804);
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

        uint8_t mode = animate::_animation_header->flags & 3;
        switch (mode) {
        case 0:
            draw::Draw_Animated_Sprite_(0, 0, sprite);
            break;
        case 1:
            draw::Draw_Bitmap_Sprite_(0, 0, sprite);
            break;
        case 2:
            if (video::_copy_to_on_page_flag != 1) {
                animate::Draw_Screen_Background_Off_(sprite);
            }
            if (video::screen_refresh_flag >= 2) {
                setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
            }
            break;
        case 3:
            exitm::Exit_With_Message(s_Refresh_Mask_can_t_be_drawn_0055b10c);
            break;
        }

        animate::_animation_header->current_frame++;

        if ((animate::_animation_header->flags & 0x20) != 0) {
            if (animate::_animation_header->frame_count <= animate::_animation_header->current_frame) {
                animate::_animation_header->current_frame = static_cast<int16_t>(static_cast<int8_t>(animate::_animation_header->loop_frame));
            }
        } else {
            if (animate::_animation_header->frame_count <= animate::_animation_header->current_frame) {
                animate::_animation_header->current_frame = static_cast<int16_t>(static_cast<int8_t>(animate::_animation_header->loop_frame));
            }
        }
    }

    int16_t __cdecl Get_Multi_File_Animation_Frame_(int16_t index) {
        _file_animation_header = reinterpret_cast<uint8_t*>(init::_io_buffer) + index * 0xAAA;
        return *reinterpret_cast<int16_t*>(_file_animation_header + 4);
    }

    void __cdecl Set_Multi_File_Animation_Frame_(int16_t anim_idx, int16_t frame) {
        _file_animation_header = reinterpret_cast<uint8_t*>(init::_io_buffer) + anim_idx * 0xAAA;

        int16_t max_frames = *reinterpret_cast<int16_t*>(_file_animation_header + 6);
        if (max_frames <= frame) {
            frame = max_frames - 1;
        }

        *reinterpret_cast<int16_t*>(_file_animation_header + 4) = frame;
    }

    void __cdecl Draw_Multi_File_Animation_Palette_(int32_t index) {
        animate::_animation_header = reinterpret_cast<s_animation_header*>(
            reinterpret_cast<uint8_t*>(init::_io_buffer) + index * 2730
        );

        if (animate::_animation_header->frame_count >= 678) {
            exitm::Exit_With_Message(s_Too_many_frames_for_multi_file_a);
        }

        if ((animate::_animation_header->flags & 0x10) != 0) {
            animate::Set_Animation_Palette_();
        }
    }

    void __cdecl Reset_File_Animation_Buffer_() {
        platform::Lock_Offscreen_Surface_();
        _file_animation_buffer = platform::_offscreen_surface;
        platform::Unlock_Offscreen_Surface_();
    }
}
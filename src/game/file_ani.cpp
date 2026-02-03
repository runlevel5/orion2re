#include "deps.h"

namespace file_ani {

    void __cdecl Set_File_Animation_Buffer_(void* buffer) {
        _file_animation_buffer = (uint8_t*)buffer;
    }

    void __cdecl Open_File_Animation_(const char* filename, int16_t entry_index) {
        _file_lbx_header = reinterpret_cast<uint32_t*>(init::_io_buffer);

        if (entry_index < 0) {
            farload::Error_Handler_(filename, 4, entry_index);
        }

        if (_file_animation_open) {
            _fclose(_file_animation_fptr);
        }

        _file_animation_fptr = _wfopen(filename, s_rb_0055117c);
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

            _file_animation_fptr = _wfopen(full_path, s_rb_0055117c);
            if (_file_animation_fptr == nullptr) {
                farload::Error_Handler_(full_path, 1, entry_index);
            }
        }

        _fread(_file_lbx_header, 0x804, 1, _file_animation_fptr);

        if (*reinterpret_cast<uint16_t*>((reinterpret_cast<uint8_t*>(_file_lbx_header) + 2)) != 0xFEAD) {
            farload::Error_Handler_(filename, 7, entry_index);
        }

        if (static_cast<int16_t>(*reinterpret_cast<uint16_t*>(_file_lbx_header)) <= entry_index) {
            farload::Error_Handler_(filename, 8, entry_index);
        }

        _file_animation_start = _file_lbx_header[entry_index + 2];
        _file_animation_end = _file_lbx_header[entry_index + 3];
        _file_animation_length = _file_animation_end - _file_animation_start;

        _fseek(_file_animation_fptr, _file_animation_start, 0);

        _fread(_file_lbx_header, 0x2000, 1, _file_animation_fptr);
    }

    void __cdecl Draw_File_Animation_() {
        s_animation_header* header = init::_io_buffer;
        animate::_animation_header = header;

        if (header->flags & 0x10) {
            animate::Set_Animation_Palette_();
        }

        int32_t current_frame = header->current_frame;
        uint32_t offset = _file_animation_start + header->frame_offsets[current_frame];
        uint32_t next_offset = header->frame_offsets[current_frame + 1];
        uint32_t frame_size = next_offset - header->frame_offsets[current_frame];

        if (allocate::Get_Max_Block_Space_(_file_animation_buffer) < (int32_t)frame_size) {
            exitm::Exit_With_Message(s_File_Animation_Frame_Too_Large);
        }

        uint8_t* sprite = _file_animation_buffer;
        _fseek(_file_animation_fptr, offset, 0 /* SEEK_SET */);

        _fread(sprite, frame_size, 1, _file_animation_fptr);

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(0, 0, header->width - 1, header->height - 1);
        }

        uint8_t flags = header->flags;
        uint8_t type = flags & 0x03;

        if (flags & 0x08) { // Glassed rendering
            switch (type) {
            case 0:
                animate::Draw_Glassed_Animated_Sprite_(0, 0, sprite);
                break;
            case 1:
                animate::Draw_Glassed_Bitmap_Sprite_(0, 0, sprite);
                break;
            case 2:
                if (video::_copy_to_on_page_flag != 1) {
                    animate::Draw_Glassed_Screen_Background_Off_(sprite);
                }
                if (video::screen_refresh_flag >= 2) {
                    setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                }
                break;
            default:
                exitm::Exit_With_Message(s_Refresh_Mask_cant_be_drawn);
                break;
            }
        } else { // Normal rendering
            switch (type) {
            case 0:
                platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                draw::Draw_Animated_Sprite_(0, 0, sprite);
                platform::Unlock_Appropriate_Render_Buffer();
                break;
            case 1:
                platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();
                draw::Draw_Bitmap_Sprite_(0, 0, sprite);
                platform::Unlock_Appropriate_Render_Buffer();
                break;
            case 2:
                if (video::_copy_to_on_page_flag != 1) {
                    animate::Draw_Screen_Background_Off_(sprite);
                }
                if (video::screen_refresh_flag >= 2) {
                    setvesa::Add_Square_To_Scan_List_(0, 0, 0x27F, 0x1DF);
                }
                break;
            default:
                exitm::Exit_With_Message(s_Refresh_Mask_cant_be_drawn);
                break;
            }
        }

        header->current_frame++;
        if (header->current_frame >= header->frame_count) {
            header->current_frame = (int16_t)(int8_t)header->loop_frame;
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

        _fseek(_file_animation_fptr, _file_animation_start + frame_offset, 0);
        _fread(_file_animation_buffer, frame_size, 1, _file_animation_fptr);

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

}
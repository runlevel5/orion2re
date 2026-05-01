#include "pch.h"

namespace TEXTBOX {
    void __cdecl Get_Help_Lbx_Name_(char* help_lbx_name) {
        switch (MOX::_settings.language) {
            case 1: /* German */
                strcpy(help_lbx_name, s_GER_HELP_LBX);
                break;
            case 2: /* French */
                strcpy(help_lbx_name, s_FRE_HELP_LBX);
                break;
            case 3: /* Spanish */
                strcpy(help_lbx_name, s_SPA_HELP_LBX);
                break;
            case 4: /* Italian */
                strcpy(help_lbx_name, s_ITA_HELP_LBX);
                break;
            default: /* English / Default */
                strcpy(help_lbx_name, s_HELP_LBX);
                break;
        }
    }
    void __cdecl Box_Centered_On_XY_(int16_t* x_ptr, int16_t* y_ptr, int16_t height, int32_t flag) {
        int16_t total_height = height + 85;
        int centered_y;

        if (MOX::_current_screen == 0) {
            *x_ptr = 84;
            centered_y = (400 - total_height) / 2 + 21;
            if (centered_y < 6) {
                centered_y = 5;
            }
        } else {
            *x_ptr = 130;
            centered_y = (480 - total_height) / 2;
        }
        *y_ptr = (int16_t)centered_y;

        s_animation_header* header_top = (s_animation_header*)buffer::Buffer_Reload_(s_textbox_lbx, 0, MOX::_global_cache_seg);
        remap::Calculate_Picture_Remap_Colors_(header_top);

        graphics::Tri_Interlaced_Fill_(
            *x_ptr + 5,
            *y_ptr + 5,
            *x_ptr + 375,
            *y_ptr + total_height - 5,
            remap::color_2,
            remap::color_3,
            remap::color_4
        );

        int16_t y_mid = *y_ptr + 42;
        animate::Remap_Draw_(*x_ptr, *y_ptr, header_top);

        s_animation_header* header_mid = (s_animation_header*)buffer::Buffer_Reload_(s_textbox_lbx, 1, MOX::_global_cache_seg);
        int16_t y_bot = height + y_mid;

        int16_t window_status[4];
        MISC::Save_Window_Status_(window_status);

        graphics::Set_Window_(*x_ptr, y_mid, *x_ptr + 380, y_bot);
        video::Clip_On_();
        animate::Remap_Draw_(*x_ptr, y_mid, header_mid);
        MISC::Restore_Window_Status_(*(uint32_t*)window_status);

        s_animation_header* header_bot = (s_animation_header*)buffer::Buffer_Reload_(s_textbox_lbx, 2, MOX::_global_cache_seg);
        animate::Remap_Draw_(*x_ptr, y_bot, header_bot);

        _bottom_y = y_bot + 43;
    }
    void __cdecl Text_Box_Startup_(int32_t size) {
        HAROLD::Save_Mouse_List_();
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        _old_help_list_active = fields::_help_list_active;
        fields::_help_list_active = 0;

        MOX2::Get_Auto_Function_(&_textbox_af);
        fields::Deactivate_Auto_Function_();

        video::Set_Refresh_Stencil_();

        MOX2::Execute_Auto_Function_(&_textbox_af);

        _old_draw_fields_flag = fields::_draw_fields_flag;
        fields::Enable_Draw_All_Fields_();
        fields::Draw_Visible_Fields_();
        fields::Disable_Draw_All_Fields_();

        FIELDSAV::Save_Field_Stats_(&_textbox_field_save[_field_save_ix]);
        _field_save_ix++;

        int32_t remaining_space = allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
        if ((remaining_space - 0x200) < size) {
            swap::Save_Block_To_Drive_((uint8_t*)MOX::_screen_seg, size + 0x200);
            _textbox_seg = (uint8_t*)allocate::Allocate_First_Block_(MOX::_screen_seg, size + 0x180);
            _reload_saved_block = 1;
        } else {
            allocate::Push_Block_(MOX::_screen_seg);
            _reload_saved_block = 0;
            _textbox_seg = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size + 0x180);
        }

        s_animation_header* pic = (s_animation_header*)buffer::Buffer_Reload_(s_textbox_lbx, 0, MOX::_global_cache_seg);
        remap::Calculate_Picture_Remap_Colors_(pic);
    }

    void __cdecl Text_Box_Get_Input_(int16_t ticks) {
        if (ticks != 0) {
            fields::Timed_Wait_For_Input_(static_cast<int32_t>(ticks));
            fields::Clear_Mouse_Buffer_();
            fields::Set_Input_Delay_(3);
            return;
        }

        // Wait for mouse button release
        while (mouse::Mouse_Button_() != 0) {
            timer::Mark_Time_();
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }

        fields::Clear_Mouse_Buffer_();

        // Wait for any keyboard or mouse input
        do {
            timer::Mark_Time_();
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        } while (fields::Get_Input_() == 0);
    }

    void __cdecl Text_Box_Cleanup_() {
        RUSS::Mox_Sync_Update_();
        _field_save_ix--;
        FIELDSAV::Restore_Field_Stats_(&_textbox_field_save[_field_save_ix]);
        fields::Enable_Draw_All_Fields_();

        if (_reload_saved_block != 0) {
            swap::Restore_Block_From_Drive_();
        } else {
            allocate::Pop_Block_(MOX::_screen_seg);
        }

        MOX2::Put_Auto_Function_(&_textbox_af);
        fields::Activate_Auto_Function_();
        fields::Clear_Mouse_Buffer_();
        fields::Set_Input_Delay_(3);

        fields::_help_list_active = _old_help_list_active;
        fields::_draw_fields_flag = _old_draw_fields_flag;

        Restore_Mouse_List_();
    }

    void __cdecl Do_Text_Box_(const char* title, const char* text, int16_t input_flag) {
        s_colors colors_1;
        s_animation_header *anim;
        s_colors colors_2;
        uint8_t color_2 = 0;
        uint8_t color_1 = 0;
        int16_t font_spacing;
        int16_t title_x;
        int16_t window_status;
        int16_t font_style;
        uint16_t width;
        int16_t y = 0;
        int16_t x = 0;

        Get_Mox_Text_Box_Base_Colors_(0, &color_1, &color_2);
        Text_Box_Startup_(60000);

        colors_1 = *MISC::Font_Colors2_(5, color_1, color_2);
        colors_2 = *MISC::Font_Colors2_(3, color_1, color_2);

        fonts::Set_Font_Style_(3, &colors_1);

        uint16_t max_height = (uint16_t)FMTPARA::Get_Formatted_Paragraph_Max_Height_(339, const_cast<char*>(text));

        if (FMTPARA::Get_Last_Formatted_Paragraph_Max_Width_() < 339) {
            width = (uint16_t)FMTPARA::Get_Last_Formatted_Paragraph_Max_Width_();
        } else {
            width = 339;
        }

        Box_Centered_On_XY_(&x, &y, max_height, title != nullptr);

        anim = reinterpret_cast<s_animation_header*>(
            farload::Far_Reload_Next_(s_textbox_lbx_00557444, 3, _textbox_seg)
        );

        if (title != nullptr) {
            fonts::Set_Remap_Font_Style_(5, &colors_1);
            int16_t title_y_base = y;
            title_x = x + 190;
            fonts::Get_String_Width_(title);
            MISC::Set_Fitted_Font_Style_(const_cast<char*>(title), &colors_1, 5, 2, -1, 380, 3);
            font_style = MISC::Get_Font_Style_();
            font_spacing = MISC::Get_Font_Spacing_();
            colors_1 = *MISC::Font_Colors2_(font_style, color_1, color_2);
            fonts::Set_Remap_Font_Style_(font_style, &colors_1);
            fonts::Set_Font_Spacing_(font_spacing);
            fonts::Print_Centered_(title_x, title_y_base + 11, title);
        }

        fonts::Set_Remap_Font_Style_(3, &colors_2);

        int16_t print_x = x + 20;
        int16_t print_y = y + 42;

        if (title == nullptr) {
            print_x = x + 8 + (362 - width) / 2;
            print_y = y + 12 + ((max_height + 43) - max_height) / 2;
        }

        MISC::Save_Window_Status_(&window_status);

        graphics::Set_Window_(x + 20, y + 42, x + 359, y + 418);

        video::Clip_On_();

        FMTPARA::Print_Formatted_Paragraph_(print_x, print_y, width, const_cast<char*>(text), 0);

        MISC::Restore_Window_Status_((uint32_t)window_status);

        video::Copy_Off_To_Back_();

        fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551120, 41);

        Text_Box_Get_Input_(input_flag);

        animate::Set_Animation_Frame_(anim, 1);
        animate::Remap_Draw_(x + 158, _bottom_y - 27, anim);

        Text_Box_Cleanup_();

        MOX::_FULL_DRAW_SCREEN_FLAG = 1;
    }

    void __cdecl Text_Box_(const char* title, const char* text) {
        Do_Text_Box_(title, text, 0);
    }

    void __cdecl Timed_Text_Box_(const char* title, const char* message, int16_t duration) {
        Do_Text_Box_(title, message, duration);
    }

    void __cdecl Draw_Help_Entry_(int16_t param_1) {
        _box_x = 0;
        _box_y = 0;

        uint8_t base_color_g = 0;
        uint8_t base_color_b = 0;

        Text_Box_Startup_(0x11B34);
        Get_Mox_Text_Box_Base_Colors_(0, &base_color_g, &base_color_b);

        s_colors* color1_ptr = MISC::Font_Colors2_(4, base_color_g, base_color_b);
        s_colors color_1 = *color1_ptr;

        s_colors* color2_ptr = MISC::Font_Colors2_(2, base_color_g, base_color_b);
        s_colors color_2 = *color2_ptr;

        fonts::Set_Remap_Font_Style_(4, &color_1);
        fonts::Get_Font_Height_();
        fonts::Set_Remap_Font_Style_(2, &color_2);

        allocate::Push_Block_(_textbox_seg);
        s_lbx_stream_header* lbx_header = (s_lbx_stream_header*)allocate::Allocate_Next_Block_(_textbox_seg, 0x104C);

        uint16_t count = 0;
        uint32_t help_idx = static_cast<uint16_t>(param_1);
        uint32_t total_lbx_size = 0;

        char help_lbx_name[20];
        Get_Help_Lbx_Name_(help_lbx_name);

        s_help_record records[9];
        bool has_anim[9];

        do {
            allocate::Push_Block_(_textbox_seg);
            void* record_data = farload::Far_Reload_Next_Data_(help_lbx_name, 0, _textbox_seg, help_idx, 1, 0x57B);
            
            memcpy(&records[count], record_data, 1403);

            FMTPARA::Get_Formatted_Paragraph_Max_Height_(0x153, records[count].body);
            allocate::Pop_Block_(_textbox_seg);

            has_anim[count] = (records[count].anim_lbx[0] != '\0');
            if (has_anim[count]) {
                lbx::LBX_init_(lbx_header, records[count].anim_lbx);
                uint16_t rec_idx = static_cast<uint16_t>(records[count].anim_info & 0xFFFF);
                total_lbx_size += lbx::LBX_getRecordLength_(lbx_header, rec_idx) + 0x400;
                lbx::LBX_uninit_(lbx_header);
            }

            uint32_t next_idx = records[count].next_help_idx;
            if (next_idx == 0xFFFFFFFF) {
                help_idx++;
            } else {
                help_idx = next_idx;
            }
            
            count++;
        } while (help_idx != 0 && count <= 8);

        allocate::Pop_Block_(_textbox_seg);

        if (allocate::Get_Remaining_Block_Space_(_textbox_seg) < (int32_t)total_lbx_size) {
            swap::Save_Block_To_Drive_(_textbox_seg, total_lbx_size + 0x11B34);
            _textbox_seg = (uint8_t*)allocate::Allocate_First_Block_(MOX::_screen_seg, total_lbx_size + 0x11B34);
            _reload_saved_block = 1;
        }

        s_animation_header* textbox_lbx_ptr = static_cast<s_animation_header*>(farload::Far_Reload_Next_(s_textbox_lbx_00557444, 3, _textbox_seg));

        tmp_block = (uint32_t)reinterpret_cast<uintptr_t>(allocate::Allocate_Next_Block_(_textbox_seg, 0x30D4));

        int16_t title_y[9];
        uint8_t anim_frames[9];
        int16_t draw_x[9];
        int16_t format_width[9];
        void* anim_ptrs[9];
        int16_t body_y[9];
        int16_t draw_y[9];

        int16_t current_y = 11;

        for (int i = 0; i < count; ++i) {
            title_y[i] = current_y;
            format_width[i] = 0x153;
            int16_t bmp_y_val = 0;

            if (has_anim[i]) {
                uint32_t anim_info = records[i].anim_info;
                uint16_t rec_idx = static_cast<uint16_t>(anim_info & 0xFFFF);
                uint8_t frame = (anim_info >> 16) & 0xFF;

                anim_frames[i] = frame;
                anim_ptrs[i] = farload::Far_Reload_Next_(records[i].anim_lbx, rec_idx, _textbox_seg);
                animate::Set_Animation_Frame_(static_cast<s_animation_header*>(anim_ptrs[i]), frame);
                bitmap::Draw_Picture_To_Bitmap_(reinterpret_cast<s_animation_header*>(tmp_block), static_cast<s_animation_header*>(anim_ptrs[i]));

                int16_t bmp_w, bmp_h, bmp_x;
                bitmap::Get_Bitmap_Actual_Size_(reinterpret_cast<s_animation_header*>(tmp_block), &bmp_w, &bmp_h, &bmp_x, &bmp_y_val);

                draw_x[i] = 354 - bmp_x - bmp_w;
                draw_y[i] = current_y - bmp_h;
                format_width[i] = 333 - bmp_x;
            }

            fonts::Set_Font_Style_(4, &color_1);

            int16_t title_height = 0;
            if (records[i].title[0] != '\x14') {
                title_height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(format_width[i], records[i].title);
            }

            if (bmp_y_val > 0 && title_height < bmp_y_val) {
                title_height = bmp_y_val;
            }

            current_y += title_height + 4;
            if (i == 0 && current_y < 42) {
                current_y = 42;
            }

            body_y[i] = current_y;

            fonts::Set_Font_Style_(2, &color_2);
            int16_t body_height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(0x153, records[i].body);
            
            current_y += body_height + 8;
        }

        int16_t box_x, box_y;
        Box_Centered_On_XY_(&box_x, &box_y, current_y - 42, 0);

        for (int i = 0; i < count; ++i) {
            if (records[i].title[0] != '\x14') {
                fonts::Set_Remap_Font_Style_(4, &color_1);
                FMTPARA::Print_Formatted_Paragraph_(
                    box_x + 20,
                    title_y[i] + box_y,
                    format_width[i],
                    records[i].title,
                    0
                );
                fonts::Set_Remap_Font_Style_(2, &color_2);
            }
            FMTPARA::Print_Formatted_Paragraph_(
                box_x + 20,
                body_y[i] + box_y,
                0x153,
                records[i].body,
                0
            );

            if (has_anim[i]) {
                animate::Set_Animation_Frame_(static_cast<s_animation_header*>(anim_ptrs[i]), anim_frames[i]);
                animate::Draw_(
                    draw_x[i] + box_x,
                    draw_y[i] + box_y,
                    static_cast<s_animation_header*>(anim_ptrs[i])
                );
            }
        }

        video::Copy_Off_To_Back_();
        fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551120, 41);
        Text_Box_Get_Input_(0);

        animate::Set_Animation_Frame_(textbox_lbx_ptr, 1);
        animate::Remap_Draw_(
            box_x + 158,
            _bottom_y - 27,
            textbox_lbx_ptr
        );

        Text_Box_Cleanup_();
        MOX::_FULL_DRAW_SCREEN_FLAG = 1;
    }
}